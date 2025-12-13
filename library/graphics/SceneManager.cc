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

namespace gf {

  namespace {

    const uint8_t default_vert_shader_code[] = {
#include "default.vert.h"
    };

    const uint8_t default_frag_shader_code[] = {
#include "default.frag.h"
    };

    const uint8_t text_frag_shader_code[] = {
#include "text.frag.h"
    };

    const uint8_t fullscreen_vert_shader_code[] = {
#include "fullscreen.vert.h"
    };

    const uint8_t fullscreen_frag_shader_code[] = {
#include "fullscreen.frag.h"
    };

    struct ImguiVertex {
      Vec2F pos;
      Vec2F uv;
      uint32_t col;

      static GpuVertexInput compute_input()
      {
        GpuVertexInput input;

        input.buffer_descriptions.push_back({ 0, sizeof(ImguiVertex) });

        input.attributes.push_back({ 0, 0, GpuVertexElementFormat::Float2, offsetof(ImguiVertex, pos) });
        input.attributes.push_back({ 1, 0, GpuVertexElementFormat::Float2, offsetof(ImguiVertex, uv) });
        input.attributes.push_back({ 2, 0, GpuVertexElementFormat::UByte4_Norm, offsetof(ImguiVertex, col) });

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
    compile_shaders();
    build_default_pipelines();
  }

  GpuGraphicsPipeline* BasicSceneManager::render_pipeline(RenderPipelineType type)
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

  void BasicSceneManager::render_records(GpuRenderPass render_pass, RenderRecorder& recorder)
  {
    m_last_pipeline = nullptr;
    m_last_object = {};

    recorder.iterate_records(
      [&](GpuBuffer* view_matrix, RectF viewport) {
        render_pass.set_viewport(viewport);

        RectI scissor = {};
        scissor.offset = viewport.offset;
        scissor.extent = viewport.extent;
        render_pass.set_scissor(scissor);

        render_pass.bind_storage_buffer(GpuShaderStage::Vertex, 0, view_matrix);
      },
      [&](RectI scissor) {
        render_pass.set_scissor(scissor);
      },
      [&](GpuBuffer* text_effect) {
        // see https://github.com/libsdl-org/SDL/issues/13018
        // to understand why index is 0 and not 1
        render_pass.bind_storage_buffer(GpuShaderStage::Fragment, 0, text_effect);
      },
      [&](RenderObject& object, GpuBuffer* model_matrix) {
        // pipeline

        if (m_last_pipeline == nullptr || object.geometry.pipeline != m_last_object.geometry.pipeline) {
          m_last_pipeline = render_pipeline(object.geometry.pipeline);
          render_pass.bind_pipeline(m_last_pipeline);
        }

        // sampler

        if (object.geometry.texture == nullptr) {
          object.geometry.texture = &m_white;
        }

        if (object.geometry.texture != m_last_object.geometry.texture) {
          render_pass.bind_texture(GpuShaderStage::Fragment, 0, object.geometry.texture);
        }

        // model matrix

        render_pass.bind_storage_buffer(GpuShaderStage::Vertex, 1, model_matrix);

        assert(object.geometry.vertices != nullptr);

        if (object.geometry.vertices != m_last_object.geometry.vertices) {
          render_pass.bind_vertex_buffer(object.geometry.vertices);
        }

        assert(object.geometry.indices != nullptr);

        if (object.geometry.indices != m_last_object.geometry.indices) {
          render_pass.bind_index_buffer(object.geometry.indices);
        }

        render_pass.draw_indexed(object.geometry.count, object.geometry.first, object.geometry.offset);

        m_last_object = object;
      }
    );
  }

  void BasicSceneManager::compile_shaders()
  {
    GpuDevice* device = m_render_manager.device();

    // default vertex shader

    GpuShaderInput default_vertex_shader_input = {};
    default_vertex_shader_input.storage_buffers = 2;

    m_default_vertex_shader = GpuShader(GpuShaderStage::Vertex, gf::span(default_vert_shader_code), default_vertex_shader_input, device);

    // default fragment shader

    GpuShaderInput default_fragment_shader_input = {};
    default_fragment_shader_input.samplers = 1;

    m_default_fragment_shader = GpuShader(GpuShaderStage::Fragment, gf::span(default_frag_shader_code), default_fragment_shader_input, device);

    // text fragment shader

    GpuShaderInput text_fragment_shader_input = {};
    text_fragment_shader_input.samplers = 1;
    text_fragment_shader_input.storage_buffers = 1;

    m_text_fragment_shader = GpuShader(GpuShaderStage::Fragment, gf::span(text_frag_shader_code), text_fragment_shader_input, device);

    // fullscreen vertex shader

    GpuShaderInput fullscreen_vertex_shader_input = {};

    m_fullscreen_vertex_shader = GpuShader(GpuShaderStage::Vertex, gf::span(fullscreen_vert_shader_code), fullscreen_vertex_shader_input, device);

    // fullscreen fragment shader

    GpuShaderInput fullscreen_fragment_shader_input = {};
    fullscreen_fragment_shader_input.samplers = 1;

    m_fullscreen_fragment_shader = GpuShader(GpuShaderStage::Fragment, gf::span(fullscreen_frag_shader_code), fullscreen_fragment_shader_input, device);
  }

  void BasicSceneManager::build_default_pipelines()
  {
    // default pipeline

    GpuGraphicsPipelineBuilder default_pipeline_builder;

    default_pipeline_builder.set_vertex_input(Vertex::compute_input())
        .add_shader(GpuShaderStage::Vertex, &m_default_vertex_shader)
        .add_shader(GpuShaderStage::Fragment, &m_default_fragment_shader)
    ;

    m_default_pipeline = default_pipeline_builder.build(render_manager());
    // m_default_pipeline.set_debug_name("[gf2] Default Pipeline");

    // text pipeline

    GpuGraphicsPipelineBuilder text_pipeline_builder;

    text_pipeline_builder.set_vertex_input(Vertex::compute_input())
        .add_shader(GpuShaderStage::Vertex, &m_default_vertex_shader)
        .add_shader(GpuShaderStage::Fragment, &m_text_fragment_shader)
    ;

    m_text_pipeline = text_pipeline_builder.build(render_manager());
    // m_text_pipeline.set_debug_name("[gf2] Text Pipeline");

    // fullscreen pipeline

    GpuGraphicsPipelineBuilder fullscreen_pipeline_builder;

    fullscreen_pipeline_builder.add_shader(GpuShaderStage::Vertex, &m_fullscreen_vertex_shader)
        .add_shader(GpuShaderStage::Fragment, &m_fullscreen_fragment_shader)
    ;

    m_fullscreen_pipeline = fullscreen_pipeline_builder.build(render_manager());
    // m_fullscreen_pipeline.set_debug_name("[gf2] Fullscreen Pipeline");

    // imgui pipeline

    GpuGraphicsPipelineBuilder imgui_pipeline_builder;

    imgui_pipeline_builder.set_vertex_input(ImguiVertex::compute_input())
        .add_shader(GpuShaderStage::Vertex, &m_default_vertex_shader)
        .add_shader(GpuShaderStage::Fragment, &m_default_fragment_shader)
    ;

    m_imgui_pipeline = imgui_pipeline_builder.build(render_manager());
    // m_imgui_pipeline.set_debug_name("[gf2] Imgui Pipeline");
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

    Vec2I surface_size = window()->surface_size();
    Vec2I window_size = window()->size();

    m_scene->set_surface_size(surface_size);
    m_scene->set_window_size(window_size);

    while (!window()->should_close()) {
      // events

      while (std::optional<Event> event = Events::poll()) {
        switch (event->type()) {
          case EventType::Quit:
            window()->close();
            break;

          case EventType::WindowResized:
            window_size = event->from<EventType::WindowResized>().size;
            m_scene->set_window_size(window_size);
            break;

          case EventType::WindowPixelSizeChanged:
            surface_size = event->from<EventType::WindowPixelSizeChanged>().size;
            m_scene->set_surface_size(surface_size);
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

      GpuCommandBuffer command_buffer = render_manager()->begin_command_buffer();

      recorder.clear();
      m_scene->render(recorder);
      recorder.sort();

      RenderTarget target = render_manager()->current_render_target();
      GpuRenderPass render_pass = command_buffer.begin_render_pass(target, m_scene->clear_color());

      render_records(render_pass, recorder);

      command_buffer.end_render_pass(render_pass);
      render_manager()->end_command_buffer(command_buffer);
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

      Vec2I surface_size = window()->surface_size();
      Vec2I window_size = window()->size();

      std::for_each(scenes.begin(), scenes.end(), [surface_size, window_size](BasicScene* scene) {
        scene->set_surface_size(surface_size);
        scene->set_window_size(window_size);
      });

      BasicScene* current_scene = scenes.back();
      current_scene->show();
      current_scene->resume();

      while (!m_scenes_changed && !window()->should_close()) {
        // events

        while (std::optional<Event> event = Events::poll()) {
          switch (event->type()) {
            case EventType::Quit:
              window()->close();
              break;

            case EventType::WindowResized:
              window_size = event->from<EventType::WindowResized>().size;
              std::for_each(scenes.begin(), scenes.end(), [window_size](BasicScene* scene) { scene->set_window_size(window_size); });
              break;

            case EventType::WindowPixelSizeChanged:
              surface_size = event->from<EventType::WindowPixelSizeChanged>().size;
              std::for_each(scenes.begin(), scenes.end(), [surface_size](BasicScene* scene) { scene->set_surface_size(surface_size); });
              break;

            default:
              // do nothing
              break;
          }

          const Event& actual_event = *event;
          std::for_each(scenes.begin(), scenes.end(), [&actual_event](BasicScene* scene) { scene->process_event(actual_event); });
        }

        std::for_each(scenes.begin(), scenes.end(), [](BasicScene* scene) { scene->handle_actions(); });

        // update

        const Time time = clock.restart();
        std::for_each(scenes.begin(), scenes.end(), [time](BasicScene* scene) { scene->update(time); });

        // render

        GpuCommandBuffer command_buffer = render_manager()->begin_command_buffer();

        recorder.clear();
        std::for_each(scenes.begin(), scenes.end(), [&recorder](BasicScene* scene) { scene->render(recorder); });
        recorder.sort();

        RenderTarget target = render_manager()->current_render_target();
        GpuRenderPass render_pass = command_buffer.begin_render_pass(target, current_scene->clear_color());

        render_records(render_pass, recorder);

        command_buffer.end_render_pass(render_pass);
        render_manager()->end_command_buffer(command_buffer);
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

    for (BasicScene* scene : scenes) {
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

    for (BasicScene* scene : m_curr_scenes) {
      scene->set_rank(SceneRank::None);
    }

    m_curr_scenes.clear();
  }

}
