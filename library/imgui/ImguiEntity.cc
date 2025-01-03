// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/imgui/ImguiEntity.h>

#include <cstdint>

#include <type_traits>

#include <imgui.h>

#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  static_assert(std::is_same_v<ImDrawIdx, uint16_t>);

  ImguiEntity::ImguiEntity()
  : m_vertices(BufferType::Host, BufferUsage::Vertex)
  , m_indices(BufferType::Host, BufferUsage::Index)
  {
    m_vertices.set_debug_name("[gf2] Imgui Vertex Buffer");
    m_indices.set_debug_name("[gf2] Imgui Index Buffer");
  }

  void ImguiEntity::set_draw_data(const ImDrawData* data)
  {
    const ImGuiIO& io = ImGui::GetIO();
    auto* render_manager = static_cast<RenderManager*>(io.BackendRendererUserData);

    // initialize

    m_objects.clear();

    // merge all buffers

    std::vector<ImDrawVert> vertices;
    vertices.reserve(data->TotalVtxCount);

    std::vector<ImDrawIdx> indices;
    indices.reserve(data->TotalIdxCount);

    for (int i = 0; i < data->CmdListsCount; ++i) {
      const ImDrawList* list = data->CmdLists[i];
      vertices.insert(vertices.end(), list->VtxBuffer.Data, list->VtxBuffer.Data + list->VtxBuffer.Size);
      indices.insert(indices.end(), list->IdxBuffer.Data, list->IdxBuffer.Data + list->IdxBuffer.Size);
    }

    for (auto& vertex : vertices) {
      const ImColor raw_sgrb_color(vertex.col);
      const Color srgb_color(raw_sgrb_color.Value.x, raw_sgrb_color.Value.y, raw_sgrb_color.Value.z, raw_sgrb_color.Value.w);
      const Color linear_color = gf::srgb_to_linear(srgb_color);
      const ImColor raw_linear_color(linear_color.r, linear_color.g, linear_color.b, linear_color.a);
      vertex.col = raw_linear_color;
    }

    assert(vertices.size() == static_cast<std::size_t>(data->TotalVtxCount));
    assert(indices.size() == static_cast<std::size_t>(data->TotalIdxCount));

    // create or update buffer if necessary

    m_vertices.update(vertices.data(), vertices.size(), render_manager);
    m_indices.update(indices.data(), indices.size(), render_manager);

    // compute commands

    std::ptrdiff_t indices_offset = 0;
    std::ptrdiff_t vertices_offset = 0;
    const ImVec2 position = data->DisplayPos;
    const ImVec2 scale = data->FramebufferScale;

    for (int i = 0; i < data->CmdListsCount; ++i) {
      const ImDrawList* list = data->CmdLists[i];

      for (int j = 0; j < list->CmdBuffer.Size; ++j) {
        const ImDrawCmd* command = &list->CmdBuffer[j];

        if (command->UserCallback != nullptr) {
          command->UserCallback(list, command);
        } else {
          ImguiObject object = {};

          // compute scissor

          const Vec2F min((command->ClipRect.x - position.x) * scale.x, (command->ClipRect.y - position.y) * scale.y);
          const Vec2F max((command->ClipRect.z - position.x) * scale.x, (command->ClipRect.w - position.y) * scale.y);
          object.scissor = RectI::from_min_max(min, max);

          object.scissor.offset = gf::max(object.scissor.offset, { 0, 0 }); // because VUID-vkCmdSetScissor-x-00595

          // count, first, offset

          object.size = command->ElemCount;
          object.first = command->IdxOffset + indices_offset;
          object.offset = command->VtxOffset + vertices_offset;

          // texture

          object.texture = reinterpret_cast<Texture*>(static_cast<uintptr_t>(command->TextureId)); // NOLINT

          m_objects.push_back(object);
        }
      }

      indices_offset += list->IdxBuffer.Size;
      vertices_offset += list->VtxBuffer.Size;
    }
  }

  void ImguiEntity::render(RenderRecorder& recorder)
  {
    for (auto& raw_object : m_objects) {
      recorder.update_scissor(raw_object.scissor);

      RenderObject object = {};
      object.priority = priority();
      object.geometry.pipeline = RenderPipelineType::Imgui;
      object.geometry.vertices = &m_vertices.buffer();
      object.geometry.indices = &m_indices.buffer();
      object.geometry.first = raw_object.first;
      object.geometry.size = raw_object.size;
      object.geometry.offset = raw_object.offset;
      object.geometry.texture = raw_object.texture;
      object.transform = Identity3F;

      recorder.record(object);
    }
  }

}
