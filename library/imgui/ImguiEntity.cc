// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/imgui/ImguiEntity.h>
// clang-format on

#include <cstdint>

#include <type_traits>

#include <imgui.h>

#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  static_assert(std::is_same_v<ImDrawIdx, uint16_t>);

  void ImguiEntity::set_draw_data(const ImDrawData* data)
  {
    const ImGuiIO& io = ImGui::GetIO();
    auto* renderer = static_cast<Renderer*>(io.BackendRendererUserData);

    // initialize

    m_lists.clear();

    m_current_buffer = 1 - m_current_buffer;
    auto& current_vertices = m_vertices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    auto& current_indices = m_indices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

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

    const std::size_t vertices_size = data->TotalVtxCount * sizeof(ImDrawVert);

    if (vertices_size > current_vertices.size()) {
      current_vertices = Buffer(BufferType::Host, BufferUsage::Vertex, vertices.data(), vertices.size(), renderer);
    } else {
      current_vertices.update(vertices.data(), vertices.size(), renderer);
    }

    const std::size_t indices_size = data->TotalIdxCount * sizeof(ImDrawIdx);

    if (indices_size > current_indices.size()) {
      current_indices = Buffer(BufferType::Host, BufferUsage::Index, indices.data(), indices.size(), renderer);
    } else {
      current_indices.update(indices.data(), indices.size(), renderer);
    }

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
          CommandList list = {};

          // compute scissor

          const Vec2F min((command->ClipRect.x - position.x) * scale.x, (command->ClipRect.y - position.y) * scale.y);
          const Vec2F max((command->ClipRect.z - position.x) * scale.x, (command->ClipRect.w - position.y) * scale.y);
          list.scissor = RectI::from_min_max(min, max);

          // count, first, offset

          list.count = command->ElemCount;
          list.first = command->IdxOffset + indices_offset;
          list.offset = command->VtxOffset + vertices_offset;

          // texture

          list.texture = static_cast<Texture*>(command->TextureId);

          m_lists.push_back(list);
        }
      }

      indices_offset += list->IdxBuffer.Size;
      vertices_offset += list->VtxBuffer.Size;
    }
  }

  void ImguiEntity::render(RenderRecorder& recorder)
  {
    for (auto& command_list : m_lists) {
      recorder.update_scissor(command_list.scissor);

      RenderObject object = {};
      object.priority = INT32_MAX;
      object.geometry.pipeline = RenderPipelineType::Imgui;
      object.geometry.vertices = &m_vertices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
      object.geometry.indices = &m_indices[m_current_buffer]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
      object.geometry.first = command_list.first;
      object.geometry.count = command_list.count;
      object.geometry.offset = command_list.offset;
      object.geometry.texture = command_list.texture;
      object.transform = Identity3F;

      recorder.record(object);
    }
  }

}
