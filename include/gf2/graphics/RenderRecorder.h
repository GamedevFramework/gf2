// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SCENE_RECORDER_H
#define GF_SCENE_RECORDER_H

#include <cstdint>

#include <vector>

#include <gf2/core/Mat4.h>
#include <gf2/core/Rect.h>

#include "Buffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"

namespace gf {
  class Renderer;

  class GF_GRAPHICS_API RenderRecorder {
  public:
    RenderRecorder(Renderer* renderer);

#ifdef _MSC_VER
    // why?
    RenderRecorder(const RenderRecorder&) = delete;
    RenderRecorder(RenderRecorder&&) noexcept = default;
    ~RenderRecorder() = default;

    RenderRecorder& operator=(const RenderRecorder&) = delete;
    RenderRecorder& operator=(RenderRecorder&&) noexcept = default;
#endif

    void update_view(const Mat4F& view_matrix, const RectF& viewport);
    void record(const RenderObject& object);

    void sort();
    void clear();

  private:
    friend class BasicSceneManager;

    struct RenderPart {
      uint32_t view_matrix_buffer_index;
      RectF viewport;
      std::vector<RenderObject> objects;
    };

    Renderer* m_renderer;
    std::vector<RenderPart> m_parts;

    uint32_t m_next_view_matrix_index = 0;
    std::vector<Buffer> m_view_matrix_buffers;
  };

}

#endif // GF_SCENE_RECORDER_H
