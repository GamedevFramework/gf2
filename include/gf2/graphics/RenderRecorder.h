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
  class RenderManager;

  class GF_GRAPHICS_API RenderRecorder {
  public:
    RenderRecorder(RenderManager* render_manager);

#ifdef _MSC_VER
    // why?
    RenderRecorder(const RenderRecorder&) = delete;
    RenderRecorder(RenderRecorder&&) noexcept = default;
    ~RenderRecorder() = default;

    RenderRecorder& operator=(const RenderRecorder&) = delete;
    RenderRecorder& operator=(RenderRecorder&&) noexcept = default;
#endif

    void update_view(const Mat4F& view_matrix, const RectF& viewport);
    void update_scissor(const RectI& scissor);
    void record(const RenderObject& object);

    void sort();
    void clear();

  private:
    friend class BasicSceneManager;

    enum class RecordType : uint8_t {
      View,
      Scissor,
      Object,
    };

    struct Record {
      RecordType type;
      std::size_t index;
    };

    struct ViewRecord {
      uint32_t view_matrix_buffer_index;
      RectF viewport;
    };

    struct ScissorRecord {
      RectI scissor;
    };

    RenderManager* m_render_manager;

    std::vector<Record> m_records;

    std::vector<ViewRecord> m_views;
    std::vector<ScissorRecord> m_scissors;
    std::vector<RenderObject> m_objects;

    uint32_t m_next_view_matrix_index = 0;
    std::vector<Buffer> m_view_matrix_buffers;
  };

}

#endif // GF_SCENE_RECORDER_H
