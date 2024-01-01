// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_IMGUI_ENTITY_H
#define GF_IMGUI_ENTITY_H

#include <gf2/core/Rect.h>
#include <gf2/core/Transform.h>
#include <gf2/core/Vec2.h>

#include <gf2/graphics/Buffer.h>
#include <gf2/graphics/Entity.h>
#include <gf2/graphics/Texture.h>

#include "ImguiApi.h"

struct ImDrawData;

namespace gf {

  class GF_IMGUI_API ImguiEntity : public Entity {
  public:
    void set_draw_data(const ImDrawData* data);

    void render(RenderRecorder& recorder) override;

  private:
    static constexpr std::size_t ImguiFramesInFlight = 3;

    std::size_t m_current_buffer = 0;
    Buffer m_vertices[ImguiFramesInFlight];
    Buffer m_indices[ImguiFramesInFlight];

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
