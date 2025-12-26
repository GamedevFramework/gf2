// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SCENE_RECORDER_H
#define GF_SCENE_RECORDER_H

#include <cassert>
#include <cstdint>

#include <vector>

#include <gf2/core/Color.h>
#include <gf2/core/Mat4.h>
#include <gf2/core/Portability.h>
#include <gf2/core/Rect.h>

#include "GpuBuffer.h"
#include "GraphicsApi.h"
#include "RenderObject.h"
#include "TextEffect.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API RenderRecorder {
  public:
    RenderRecorder(RenderManager* render_manager);

    GF_FIX_DUMB_MSVC(RenderRecorder)

    void update_view(const Mat3F& view_matrix, const RectF& viewport);
    void update_scissor(const RectI& scissor);
    void update_text_effect(const TextEffect& effect);
    void record(const RenderObject& object);

    void sort();
    void clear();

    template<typename ViewFunc, typename ScissorFunc, typename TextFunc, typename ObjectFunc>
    void iterate_records(ViewFunc on_view, ScissorFunc on_scissor, TextFunc on_text, ObjectFunc on_object)
    {
      for (const Record& record : m_records) {
        switch (record.type) {
          case RecordType::View:
            {
              assert(record.index < m_views.size());
              ViewRecord& view = m_views[record.index];
              on_view(&m_view_matrix_buffers[view.view_matrix_buffer_index], view.viewport);
            }
            break;
          case RecordType::Scissor:
            {
              assert(record.index < m_scissors.size());
              ScissorRecord& scissor = m_scissors[record.index];
              on_scissor(scissor.scissor);
            }
            break;
          case RecordType::Text:
            {
              assert(record.index < m_texts.size());
              TextRecord& text = m_texts[record.index];
              on_text(&m_text_effect_buffers[text.text_effect_buffer_index]);
            }
            break;
          case RecordType::Object:
            {
              assert(record.index < m_objects.size());
              ObjectRecord& object = m_objects[record.index];
              on_object(object.object, &m_model_matrix_buffers[object.model_matrix_index]);
            }
            break;
        }
      }
    }

  private:
    friend class BasicSceneManager;

    enum class RecordType : uint8_t {
      View,
      Scissor,
      Text,
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

    struct TextRecord {
      uint32_t text_effect_buffer_index = 0;
    };

    struct ObjectRecord {
      RenderObject object;
      uint32_t model_matrix_index = 0;
    };

    RenderManager* m_render_manager;

    std::vector<Record> m_records;

    std::vector<ViewRecord> m_views;
    std::vector<ScissorRecord> m_scissors;
    std::vector<TextRecord> m_texts;
    std::vector<ObjectRecord> m_objects;

    uint32_t m_next_view_matrix_index = 0;
    std::vector<GpuBuffer> m_view_matrix_buffers;

    uint32_t m_next_text_effect_index = 0;
    std::vector<GpuBuffer> m_text_effect_buffers;

    uint32_t m_next_model_matrix_index = 0;
    std::vector<GpuBuffer> m_model_matrix_buffers;
  };

}

#endif // GF_SCENE_RECORDER_H
