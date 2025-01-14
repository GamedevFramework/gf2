// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_IMGUI_ENTITY_H
#define GF_IMGUI_ENTITY_H

#include <gf2/core/Rect.h>
#include <gf2/core/Transform.h>
#include <gf2/core/Vec2.h>

#include <gf2/graphics/DynamicBuffer.h>
#include <gf2/graphics/Entity.h>
#include <gf2/graphics/Texture.h>

#include "ImguiApi.h"

struct ImDrawData;

namespace gf {

  class GF_IMGUI_API ImguiEntity : public HudEntity {
  public:
    ImguiEntity();

    void set_draw_data(const ImDrawData* data);

    void render(RenderRecorder& recorder) override;

  private:
    DynamicBuffer m_vertices;
    DynamicBuffer m_indices;

    struct ImguiObject {
      RectI scissor = {};
      std::size_t size = 0;
      std::size_t first = 0;
      std::ptrdiff_t offset = 0;
      const Texture* texture = nullptr;
    };

    std::vector<ImguiObject> m_objects;
  };

}

#endif // GF_IMGUI_ENTITY_H
