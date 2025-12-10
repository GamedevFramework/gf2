// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_IMGUI_ENTITY_H
#define GF_IMGUI_ENTITY_H

#include <memory>
#include <vector>

#include <gf2/core/Rect.h>
#include <gf2/core/Transform.h>
#include <gf2/core/Vec2.h>

#include <gf2/graphics/Entity.h>
#include <gf2/graphics/GpuDynamicBuffer.h>
#include <gf2/graphics/GpuTexture.h>

#include "ImguiApi.h"

struct ImDrawData;
struct ImTextureData;

namespace gf {
  class RenderManager;

  class GF_IMGUI_API ImguiEntity : public HudEntity {
  public:
    ImguiEntity();

    void set_draw_data(const ImDrawData* data);

    void render(RenderRecorder& recorder) override;

  private:
    void update_texture(ImTextureData* texture_data, RenderManager* render_manager);

    std::vector<std::unique_ptr<GpuTexture>> m_textures;
    std::vector<std::unique_ptr<GpuTexture>> m_unused_textures;

    GpuDynamicBuffer m_vertices;
    GpuDynamicBuffer m_indices;

    struct ImguiObject {
      RectI scissor = {};
      std::size_t count = 0;
      std::size_t first = 0;
      std::ptrdiff_t offset = 0;
      GpuTexture* texture = nullptr;
    };

    std::vector<ImguiObject> m_objects;
  };

}

#endif // GF_IMGUI_ENTITY_H
