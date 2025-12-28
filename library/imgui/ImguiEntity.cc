// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/imgui/ImguiEntity.h>

#include <algorithm>
#include <cassert>
#include <cstdint>

#include <algorithm>
#include <memory>
#include <type_traits>

#include <imgui.h>

#include <gf2/graphics/RenderManager.h>
#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  static_assert(std::is_same_v<ImDrawIdx, uint16_t>);

  ImguiEntity::ImguiEntity()
  : m_vertices(GpuBufferUsage::Vertex)
  , m_indices(GpuBufferUsage::Index)
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

    // update textures

    m_unused_textures.clear(); // these textures are not used anymore

    if (data->Textures != nullptr) {
      for (ImTextureData* texture_data : *data->Textures) {
        if (texture_data->Status != ImTextureStatus_OK) {
          update_texture(texture_data, render_manager);
        }
      }
    }

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
          // compute scissor

          const Vec2F min((command->ClipRect.x - position.x) * scale.x, (command->ClipRect.y - position.y) * scale.y);
          const Vec2F max((command->ClipRect.z - position.x) * scale.x, (command->ClipRect.w - position.y) * scale.y);

          if (max.x <= min.x || max.y <= min.y) {
            continue;
          }

          ImguiObject object = {};

          object.scissor = RectI::from_min_max(min, max);

          object.scissor.offset = gf::max(object.scissor.offset, { 0, 0 }); // because VUID-vkCmdSetScissor-x-00595

          // count, first, offset

          object.count = command->ElemCount;
          object.first = command->IdxOffset + indices_offset;
          object.offset = command->VtxOffset + vertices_offset;

          // texture

          object.texture = reinterpret_cast<GpuTexture*>(static_cast<uintptr_t>(command->GetTexID())); // NOLINT

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
      object.geometry.count = raw_object.count;
      object.geometry.offset = raw_object.offset;
      object.geometry.texture = raw_object.texture;
      object.transform = Identity3F;

      recorder.record(object);
    }
  }

  void ImguiEntity::update_texture(ImTextureData* data, RenderManager* render_manager)
  {
    assert(data->Format == ImTextureFormat_RGBA32);
    using namespace gf::operators;

    switch (data->Status) {
      case ImTextureStatus_WantCreate:
        {
          // Log::debug("Creating a texture for imgui.");

          const Vec2I size = { data->Width, data->Height };
          auto texture = std::make_unique<GpuTexture>(size, GpuTextureUsage::Sampler, GpuTextureFormat::R8G8B8A8_UNorm, render_manager);
          texture->update(static_cast<std::size_t>(data->GetSizeInBytes()), static_cast<const uint8_t*>(data->GetPixels()), render_manager);

          data->SetTexID(reinterpret_cast<uintptr_t>(texture.get())); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
          data->SetStatus(ImTextureStatus_OK);
          data->BackendUserData = texture.get();

          m_textures.push_back(std::move(texture));
        }
        break;

      case ImTextureStatus_WantUpdates:
        {
          // Log::debug("Updating a texture for imgui.");

          auto* texture = static_cast<GpuTexture*>(data->BackendUserData);
          texture->update(static_cast<std::size_t>(data->GetSizeInBytes()), static_cast<const uint8_t*>(data->GetPixels()), render_manager);

          data->SetStatus(ImTextureStatus_OK);
        }
        break;

      case ImTextureStatus_WantDestroy:
        {
          // Log::debug("Destroying a texture for imgui.");

          auto* texture = static_cast<GpuTexture*>(data->BackendUserData);

          auto iterator = std::ranges::find_if(m_textures, [texture](const std::unique_ptr<GpuTexture>& other) { return other.get() == texture; });
          assert(iterator != m_textures.end());
          m_unused_textures.push_back(std::move(*iterator)); // do not remove now, texture may still be in use for the current frame
          m_textures.erase(iterator);

          data->SetTexID(ImTextureID_Invalid);
          data->SetStatus(ImTextureStatus_Destroyed);
        }
        break;

      case ImTextureStatus_OK:
      case ImTextureStatus_Destroyed:
        assert(false);
        break;
    }
  }

}
