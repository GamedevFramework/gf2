// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/SceneManager.h>
// clang-format on

#include <cassert>

#include <gf2/core/Clock.h>

#include <gf2/graphics/Scene.h>

#include "gf2/graphics/RenderObject.h"
#include "gf2/graphics/RenderRecorder.h"

namespace gf {

  /*
   * BasicSceneManager
   */

  BasicSceneManager::BasicSceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints)
  : m_window(title, size, hints)
  , m_renderer(&m_window)
  {
  }

  void BasicSceneManager::render_objects(RenderCommandBuffer command_buffer, const RenderRecorder& recorder)
  {
    for (const auto& part : recorder.m_parts) {
      command_buffer.set_viewport(part.viewport);

      RectI scissor = {};
      scissor.offset = part.viewport.offset;
      scissor.extent = part.viewport.extent;
      command_buffer.set_scissor(scissor);

      auto camera_descriptor = m_renderer.allocate_descriptor_for_layout(m_renderer.camera_descriptor());
      camera_descriptor.write(0, &recorder.m_view_matrix_buffers[part.view_matrix_buffer_index]);
      command_buffer.bind_descriptor(m_renderer.default_pipeline_layout(), 0, camera_descriptor);

      const RenderPipeline* last_pipeline = nullptr;
      RenderObject last_object = {};

      for (const auto& object : part.objects) {
        // pipeline

        if (last_pipeline == nullptr || object.geometry.pipeline != last_object.geometry.pipeline) {
          switch (object.geometry.pipeline) {
            case RenderPipelineType::Default:
              last_pipeline = m_renderer.default_pipeline();
              break;
            case RenderPipelineType::Text:
              last_pipeline = m_renderer.text_pipeline();
              break;
            default:
              assert(false);
              break;
          }

          command_buffer.bind_pipeline(last_pipeline);
        }

        // sampler

        if (object.geometry.texture != nullptr && object.geometry.texture != last_object.geometry.texture) {
          auto sampler_descriptor = m_renderer.allocate_descriptor_for_layout(m_renderer.sampler_descriptor());
          sampler_descriptor.write(0, object.geometry.texture);
          command_buffer.bind_descriptor(m_renderer.default_pipeline_layout(), 1, sampler_descriptor);
        }

        // model matrix

        command_buffer.push_constant(m_renderer.default_pipeline_layout(), gf::ShaderStage::Vertex, &object.transform);

        assert(object.geometry.vertices != nullptr);

        if (object.geometry.vertices != last_object.geometry.vertices) {
          command_buffer.bind_vertex_buffer(object.geometry.vertices);
        }

        assert(object.geometry.indices != nullptr);

        if (object.geometry.indices != last_object.geometry.indices) {
          command_buffer.bind_index_buffer(object.geometry.indices);
        }

        command_buffer.draw_indexed(object.geometry.count, object.geometry.first);

        last_object = object;
      }
    }
  }

  /*
   * SingleSceneManager
   */

  SingleSceneManager::SingleSceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints)
  : BasicSceneManager(title, size, hints)
  {
  }

  void SingleSceneManager::run()
  {
    if (m_scene == nullptr) {
      return;
    }

    Clock clock;
    RenderRecorder recorder(renderer());

    m_scene->update_framebuffer_size(window()->surface_size());

    while (!window()->should_close()) {
      // update

      while (auto event = gf::Event::poll()) {
        switch (event->type) {
          case gf::EventType::Quit:
            window()->close();
            break;

          case gf::EventType::WindowResized:
            {
              auto surface_size = window()->surface_size();
              renderer()->update_surface_size(surface_size);
              m_scene->update_framebuffer_size(surface_size);
            }
            break;

          default:
            // do nothing
            break;
        }

        m_scene->process_event(*event);
      }

      // update

      const Time time = clock.restart();
      m_scene->update(time);

      // render

      if (auto maybe_command_buffer = renderer()->begin_command_buffer(); maybe_command_buffer) {
        recorder.clear();
        m_scene->render(recorder);
        recorder.sort();

        auto command_buffer = *maybe_command_buffer;
        auto target = renderer()->current_render_target();
        auto render_command_buffer = command_buffer.begin_rendering(target, m_scene->clear_color());

        render_objects(render_command_buffer, recorder);

        command_buffer.end_rendering(render_command_buffer);
        renderer()->end_command_buffer(command_buffer);
      }
    }

    renderer()->wait_idle();
  }

  void SingleSceneManager::set_scene(Scene* scene)
  {
    assert(scene != nullptr);

    if (m_scene != nullptr) {
      m_scene->set_rank(SceneRank::None);
    }

    scene->set_rank(SceneRank::Top);
    m_scene = scene;
  }

  /*
   * SceneManager
   */

  SceneManager::SceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints)
  : BasicSceneManager(title, size, hints)
  {
  }

  void SceneManager::run()
  {
  }

  void SceneManager::push_scene(Scene* scene)
  {
    assert(scene != nullptr);
    m_scenes_changed = true;

    if (!m_curr_scenes.empty()) {
      m_curr_scenes.back()->set_rank(SceneRank::Active);
    }

    m_curr_scenes.push_back(scene);
    m_curr_scenes.back()->set_rank(SceneRank::Top);
  }

  void SceneManager::push_scenes(Span<Scene*> scenes)
  {
    m_scenes_changed = true;

    if (!m_curr_scenes.empty()) {
      m_curr_scenes.back()->set_rank(SceneRank::Active);
    }

    for (auto* scene : scenes) {
      assert(scene != nullptr);
      m_curr_scenes.push_back(scene);
      scene->set_rank(SceneRank::Active);
    }

    m_curr_scenes.back()->set_rank(SceneRank::Top);
  }

  void SceneManager::pop_scene()
  {
    m_scenes_changed = true;
    assert(!m_curr_scenes.empty());
    m_curr_scenes.back()->set_rank(SceneRank::None);
    m_curr_scenes.pop_back();

    if (!m_curr_scenes.empty()) {
      m_curr_scenes.back()->set_rank(SceneRank::Top);
    }
  }

  void SceneManager::pop_all_scenes()
  {
    m_scenes_changed = true;
    assert(!m_curr_scenes.empty());

    for (auto* scene : m_curr_scenes) {
      scene->set_rank(SceneRank::None);
    }

    m_curr_scenes.clear();
  }

}