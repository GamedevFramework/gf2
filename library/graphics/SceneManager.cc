// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/SceneManager.h>

#include <cassert>
#include <cstdlib>

#include <algorithm>

#include <gf2/core/Clock.h>

#include <gf2/graphics/Events.h>
#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/Vertex.h>
#include "gf2/graphics/Descriptor.h"

namespace gf {

  namespace {

    const uint32_t default_vert_shader_code[] = {
#include "default.vert.h"
    };

    const uint32_t default_frag_shader_code[] = {
#include "default.frag.h"
    };

    const uint32_t text_frag_shader_code[] = {
#include "text.frag.h"
    };

    const uint32_t fullscreen_vert_shader_code[] = {
#include "fullscreen.vert.h"
    };

    const uint32_t fullscreen_frag_shader_code[] = {
#include "fullscreen.frag.h"
    };

    struct ImguiVertex {
      Vec2F pos;
      Vec2F uv;
      uint32_t col;

      static VertexInput compute_input()
      {
        VertexInput input;

        input.bindings.push_back({ 0, sizeof(ImguiVertex) });

        input.attributes.push_back({ 0, 0, Format::Vec2F, offsetof(ImguiVertex, pos) });
        input.attributes.push_back({ 1, 0, Format::Vec2F, offsetof(ImguiVertex, uv) });
        input.attributes.push_back({ 2, 0, Format::Color8U, offsetof(ImguiVertex, col) });

        return input;
      }
    };

  }

  /*
   * BasicSceneManager
   */

  BasicSceneManager::BasicSceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints)
  : m_window(title, size, hints)
  , m_render_manager(&m_window)
  , m_white(Image({ 1, 1 }, White), &m_render_manager)
  {
    m_white.set_debug_name("[gf2] White Default 1x1 Texture");
    build_default_pipelines();
  }

  const RenderPipeline* BasicSceneManager::render_pipeline(RenderPipelineType type) const
  {
    switch (type) {
      case RenderPipelineType::Default:
        return &m_default_pipeline;
      case RenderPipelineType::Text:
        return &m_text_pipeline;
      case RenderPipelineType::Imgui:
        return &m_imgui_pipeline;
      default:
        assert(false);
        break;
    }

    return nullptr;
  }

  void BasicSceneManager::render_records(RenderCommandBuffer command_buffer, const RenderRecorder& recorder)
  {
    m_last_pipeline = nullptr;
    m_last_object = {};

    for (const auto& record : recorder.m_records) {
      if (record.type == RenderRecorder::RecordType::View) {
        assert(record.index < recorder.m_views.size());
        auto view = recorder.m_views[record.index];

        command_buffer.set_viewport(view.viewport);

        RectI scissor = {};
        scissor.offset = view.viewport.offset;
        scissor.extent = view.viewport.extent;
        command_buffer.set_scissor(scissor);

        auto camera_descriptor = m_render_manager.allocate_descriptor_for_layout(&m_camera_descriptor_layout);
        camera_descriptor.write(0, &recorder.m_view_matrix_buffers[view.view_matrix_buffer_index]);
        command_buffer.bind_descriptor(&m_default_pipeline_layout, 0, camera_descriptor);

      } else if (record.type == RenderRecorder::RecordType::Scissor) {
        assert(record.index < recorder.m_scissors.size());
        command_buffer.set_scissor(recorder.m_scissors[record.index].scissor);
      } else if (record.type == RenderRecorder::RecordType::Text) {
        assert(record.index < recorder.m_texts.size());
        auto text = recorder.m_texts[record.index];

        auto text_descriptor = m_render_manager.allocate_descriptor_for_layout(&m_text_descriptor_layout);
        text_descriptor.write(0, &recorder.m_text_effect_buffers[text.text_effect_buffer_index]);
        command_buffer.bind_descriptor(&m_text_pipeline_layout, 2, text_descriptor);
      } else if (record.type == RenderRecorder::RecordType::Object) {
        assert(record.index < recorder.m_objects.size());
        render_object(command_buffer, recorder.m_objects[record.index]);
      }
    }
  }

  void BasicSceneManager::render_object(RenderCommandBuffer command_buffer, RenderObject object)
  {
    // pipeline

    if (m_last_pipeline == nullptr || object.geometry.pipeline != m_last_object.geometry.pipeline) {
      m_last_pipeline = render_pipeline(object.geometry.pipeline);
      command_buffer.bind_pipeline(m_last_pipeline);
    }

    // sampler

    if (object.geometry.texture == nullptr) {
      object.geometry.texture = &m_white;
    }

    if (object.geometry.texture != m_last_object.geometry.texture) {
      auto sampler_descriptor = m_render_manager.allocate_descriptor_for_layout(&m_sampler_descriptor_layout);
      sampler_descriptor.write(0, object.geometry.texture);
      command_buffer.bind_descriptor(&m_default_pipeline_layout, 1, sampler_descriptor);
    }

    // model matrix

    command_buffer.push_constant(&m_default_pipeline_layout, ShaderStage::Vertex, object.transform);

    assert(object.geometry.vertices != nullptr);

    if (object.geometry.vertices != m_last_object.geometry.vertices) {
      command_buffer.bind_vertex_buffer(object.geometry.vertices);
    }

    assert(object.geometry.indices != nullptr);

    if (object.geometry.indices != m_last_object.geometry.indices) {
      command_buffer.bind_index_buffer(object.geometry.indices);
    }

    command_buffer.draw_indexed(object.geometry.size, object.geometry.first, object.geometry.offset);

    m_last_object = object;
  }

  void BasicSceneManager::build_default_pipelines()
  {
    // view matrix descriptor

    const DescriptorBinding camera_binding[] = {
      { 0, DescriptorType::Buffer, ShaderStage::Vertex }
    };

    m_camera_descriptor_layout = DescriptorLayout(camera_binding, render_manager());

    // sampler descriptor

    const DescriptorBinding sampler_binding[] = {
      { 0, DescriptorType::Sampler, ShaderStage::Fragment }
    };

    m_sampler_descriptor_layout = DescriptorLayout(sampler_binding, render_manager());

    // text_descriptor_layout

    const DescriptorBinding text_binding[] = {
      { 0, DescriptorType::Buffer, ShaderStage::Fragment }
    };

    m_text_descriptor_layout = DescriptorLayout(text_binding, render_manager());

    // default pipeline layout

    RenderPipelineLayoutBuilder default_pipeline_layout_builder;

    default_pipeline_layout_builder.set_push_constant_parameters(ShaderStage::Vertex, sizeof(float) * 16)
        .add_descriptor_layout(&m_camera_descriptor_layout)
        .add_descriptor_layout(&m_sampler_descriptor_layout);

    m_default_pipeline_layout = default_pipeline_layout_builder.build(render_manager());
    m_default_pipeline_layout.set_debug_name("[gf2] Default Pipeline Layout");

    // text pipeline layout

    RenderPipelineLayoutBuilder text_pipeline_layout_builder;

    text_pipeline_layout_builder.set_push_constant_parameters(ShaderStage::Vertex, sizeof(float) * 16)
        .add_descriptor_layout(&m_camera_descriptor_layout)
        .add_descriptor_layout(&m_sampler_descriptor_layout)
        .add_descriptor_layout(&m_text_descriptor_layout);

    m_text_pipeline_layout = text_pipeline_layout_builder.build(render_manager());
    m_text_pipeline_layout.set_debug_name("[gf2] Text Pipeline Layout");

    // fullscreen pipeline layout

    RenderPipelineLayoutBuilder fullscreen_pipeline_layout_builder;
    fullscreen_pipeline_layout_builder.add_descriptor_layout(&m_sampler_descriptor_layout);
    m_fullscreen_pipeline_layout = fullscreen_pipeline_layout_builder.build(render_manager());
    m_fullscreen_pipeline_layout.set_debug_name("[gf2] Fullscreen Pipeline Layout");

    // default pipeline

    RenderPipelineBuilder default_pipeline_builder;

    default_pipeline_builder.set_vertex_input(Vertex::compute_input())
        .add_shader(ShaderStage::Vertex, gf::span(default_vert_shader_code))
        .add_shader(ShaderStage::Fragment, gf::span(default_frag_shader_code))
        .set_pipeline_layout(&m_default_pipeline_layout);

    m_default_pipeline = default_pipeline_builder.build(render_manager());
    m_default_pipeline.set_debug_name("[gf2] Default Pipeline");

    // text pipeline

    RenderPipelineBuilder text_pipeline_builder;

    text_pipeline_builder.set_vertex_input(Vertex::compute_input())
        .add_shader(ShaderStage::Vertex, gf::span(default_vert_shader_code))
        .add_shader(ShaderStage::Fragment, gf::span(text_frag_shader_code))
        .set_pipeline_layout(&m_text_pipeline_layout);

    m_text_pipeline = text_pipeline_builder.build(render_manager());
    m_text_pipeline.set_debug_name("[gf2] Text Pipeline");

    // fullscreen pipeline

    RenderPipelineBuilder fullscreen_pipeline_builder;

    fullscreen_pipeline_builder.add_shader(ShaderStage::Vertex, gf::span(fullscreen_vert_shader_code))
        .add_shader(ShaderStage::Fragment, gf::span(fullscreen_frag_shader_code))
        .set_pipeline_layout(&m_fullscreen_pipeline_layout);

    m_fullscreen_pipeline = fullscreen_pipeline_builder.build(render_manager());
    m_fullscreen_pipeline.set_debug_name("[gf2] Fullscreen Pipeline");

    // imgui pipeline

    RenderPipelineBuilder imgui_pipeline_builder;

    imgui_pipeline_builder.set_vertex_input(ImguiVertex::compute_input())
        .add_shader(ShaderStage::Vertex, gf::span(default_vert_shader_code))
        .add_shader(ShaderStage::Fragment, gf::span(default_frag_shader_code))
        .set_pipeline_layout(&m_default_pipeline_layout);

    m_imgui_pipeline = imgui_pipeline_builder.build(render_manager());
    m_imgui_pipeline.set_debug_name("[gf2] Imgui Pipeline");
  }

  /*
   * SingleSceneManager
   */

  SingleSceneManager::SingleSceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints)
  : BasicSceneManager(title, size, hints)
  {
  }

  int SingleSceneManager::run()
  {
    if (m_scene == nullptr) {
      return EXIT_FAILURE;
    }

    Clock clock;
    RenderRecorder recorder(render_manager());

    m_scene->set_surface_size(window()->surface_size());

    while (!window()->should_close()) {
      // update

      while (auto event = Events::poll()) {
        switch (event->type()) {
          case EventType::Quit:
            window()->close();
            break;

          case EventType::WindowPixelSizeChanged:
            {
              auto surface_size = event->from<EventType::WindowPixelSizeChanged>().size;
              render_manager()->update_surface_size(surface_size);
              m_scene->set_surface_size(surface_size);
            }
            break;

          default:
            // do nothing
            break;
        }

        m_scene->process_event(*event);
      }

      m_scene->handle_actions();

      // update

      const Time time = clock.restart();
      m_scene->update(time);

      // render

      if (auto maybe_command_buffer = render_manager()->begin_command_buffer(); maybe_command_buffer) {
        recorder.clear();
        m_scene->render(recorder);
        recorder.sort();

        auto command_buffer = *maybe_command_buffer;
        auto target = render_manager()->current_render_target();
        auto render_command_buffer = command_buffer.begin_rendering(target, m_scene->clear_color());

        render_records(render_command_buffer, recorder);

        command_buffer.end_rendering(render_command_buffer);
        render_manager()->end_command_buffer(command_buffer);
      }
    }

    render_manager()->wait_idle();
    return EXIT_SUCCESS;
  }

  void SingleSceneManager::set_scene(BasicScene* scene)
  {
    if (scene == nullptr) {
      return;
    }

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

  int SceneManager::run()
  {
    Clock clock;
    RenderRecorder recorder(render_manager());

    while (!m_curr_scenes.empty() && !window()->should_close()) {
      std::vector<BasicScene*> scenes = m_curr_scenes; // make a copy to avoid iterator invalidation
      m_scenes_changed = false;
      auto surface_size = window()->surface_size();

      std::for_each(scenes.begin(), scenes.end(), [surface_size](auto* scene) { scene->set_surface_size(surface_size); });

      auto* current_scene = scenes.back();
      current_scene->show();
      current_scene->resume();

      while (!m_scenes_changed && !window()->should_close()) {
        // update

        while (auto event = Events::poll()) {
          switch (event->type()) {
            case EventType::Quit:
              window()->close();
              break;

            case EventType::WindowPixelSizeChanged:
              surface_size = event->from<EventType::WindowPixelSizeChanged>().size;
              render_manager()->update_surface_size(surface_size);
              std::for_each(scenes.begin(), scenes.end(), [surface_size](auto* scene) { scene->set_surface_size(surface_size); });
              break;

            default:
              // do nothing
              break;
          }

          const Event& actual_event = *event;
          std::for_each(scenes.begin(), scenes.end(), [&actual_event](auto* scene) { scene->process_event(actual_event); });
        }

        std::for_each(scenes.begin(), scenes.end(), [](auto* scene) { scene->handle_actions(); });

        // update

        const Time time = clock.restart();
        std::for_each(scenes.begin(), scenes.end(), [time](auto* scene) { scene->update(time); });

        // render

        if (auto maybe_command_buffer = render_manager()->begin_command_buffer(); maybe_command_buffer) {
          recorder.clear();
          std::for_each(scenes.begin(), scenes.end(), [&recorder](auto* scene) { scene->render(recorder); });
          recorder.sort();

          auto command_buffer = *maybe_command_buffer;
          auto target = render_manager()->current_render_target();
          auto render_command_buffer = command_buffer.begin_rendering(target, current_scene->clear_color());

          render_records(render_command_buffer, recorder);

          command_buffer.end_rendering(render_command_buffer);
          render_manager()->end_command_buffer(command_buffer);
        }
      }
    }

    render_manager()->wait_idle();
    return EXIT_SUCCESS;
  }

  void SceneManager::push_scene(BasicScene* scene)
  {
    if (scene == nullptr) {
      return;
    }

    m_scenes_changed = true;

    if (!m_curr_scenes.empty()) {
      m_curr_scenes.back()->set_rank(SceneRank::Active);
    }

    m_curr_scenes.push_back(scene);
    m_curr_scenes.back()->set_rank(SceneRank::Top);
  }

  void SceneManager::push_scenes(Span<BasicScene*> scenes)
  {
    m_scenes_changed = true;

    if (!m_curr_scenes.empty()) {
      m_curr_scenes.back()->set_rank(SceneRank::Active);
    }

    for (auto* scene : scenes) {
      if (scene == nullptr) {
        continue;
      }

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
