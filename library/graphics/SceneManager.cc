// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/SceneManager.h>
// clang-format on

#include <cassert>

#include <gf2/core/Clock.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/Vertex.h>

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
  , m_renderer(&m_window)
  , m_white(Image({ 1, 1 }, White), &m_renderer)
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

  void BasicSceneManager::render_objects(RenderCommandBuffer command_buffer, const RenderRecorder& recorder)
  {
    for (const auto& part : recorder.m_parts) {
      command_buffer.set_viewport(part.viewport);

      RectI scissor = {};
      scissor.offset = part.viewport.offset;
      scissor.extent = part.viewport.extent;
      command_buffer.set_scissor(scissor);

      auto camera_descriptor = m_renderer.allocate_descriptor_for_layout(&m_camera_descriptor_layout);
      camera_descriptor.write(0, &recorder.m_view_matrix_buffers[part.view_matrix_buffer_index]);
      command_buffer.bind_descriptor(&m_default_pipeline_layout, 0, camera_descriptor);

      const RenderPipeline* last_pipeline = nullptr;
      RenderObject last_object = {};

      for (auto object : part.objects) {
        // pipeline

        if (last_pipeline == nullptr || object.geometry.pipeline != last_object.geometry.pipeline) {
          last_pipeline = render_pipeline(object.geometry.pipeline);
          command_buffer.bind_pipeline(last_pipeline);
        }

        // sampler

        if (object.geometry.texture == nullptr) {
          object.geometry.texture = &m_white;
        }

        if (object.geometry.texture != last_object.geometry.texture) {
          auto sampler_descriptor = m_renderer.allocate_descriptor_for_layout(&m_sampler_descriptor_layout);
          sampler_descriptor.write(0, object.geometry.texture);
          command_buffer.bind_descriptor(&m_default_pipeline_layout, 1, sampler_descriptor);
        }

        // model matrix

        command_buffer.push_constant(&m_default_pipeline_layout, gf::ShaderStage::Vertex, &object.transform);

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

  void BasicSceneManager::build_default_pipelines()
  {
    // view matrix descriptor

    const DescriptorBinding camera_binding[] = {
      { 0, DescriptorType::Buffer, ShaderStage::Vertex }
    };

    m_camera_descriptor_layout = DescriptorLayout(camera_binding, renderer());

    // sampler descriptor

    const DescriptorBinding sampler_binding[] = {
      { 0, DescriptorType::Sampler, ShaderStage::Fragment }
    };

    m_sampler_descriptor_layout = DescriptorLayout(sampler_binding, renderer());

    // default pipeline layout

    RenderPipelineLayoutBuilder default_pipeline_layout_builder;

    default_pipeline_layout_builder.set_push_constant_parameters(ShaderStage::Vertex, sizeof(ShaderDataType<Mat3F>))
        .add_descriptor_layout(&m_camera_descriptor_layout)
        .add_descriptor_layout(&m_sampler_descriptor_layout);

    m_default_pipeline_layout = default_pipeline_layout_builder.build(renderer());
    m_default_pipeline_layout.set_debug_name("[gf2] Default Pipeline Layout");

    // fullscreen pipeline layout

    RenderPipelineLayoutBuilder fullscreen_pipeline_layout_builder;
    fullscreen_pipeline_layout_builder.add_descriptor_layout(&m_sampler_descriptor_layout);
    m_fullscreen_pipeline_layout = fullscreen_pipeline_layout_builder.build(renderer());
    m_fullscreen_pipeline_layout.set_debug_name("[gf2] Fullscreen Pipeline Layout");

    // default pipeline

    Shader default_vertex_shader(gf::span(default_vert_shader_code), { ShaderStage::Vertex, renderer() });
    Shader default_fragment_shader(gf::span(default_frag_shader_code), { ShaderStage::Fragment, renderer() });

    RenderPipelineBuilder default_pipeline_builder;

    default_pipeline_builder.set_vertex_input(Vertex::compute_input())
        .add_shader(&default_vertex_shader)
        .add_shader(&default_fragment_shader)
        .set_pipeline_layout(&m_default_pipeline_layout);

    m_default_pipeline = default_pipeline_builder.build(renderer());
    m_default_pipeline.set_debug_name("[gf2] Default Pipeline");

    // text pipeline

    Shader text_fragment_shader(gf::span(text_frag_shader_code), { ShaderStage::Fragment, renderer() });

    RenderPipelineBuilder text_pipeline_builder;

    text_pipeline_builder.set_vertex_input(Vertex::compute_input())
        .add_shader(&default_vertex_shader)
        .add_shader(&text_fragment_shader)
        .set_pipeline_layout(&m_default_pipeline_layout);

    m_text_pipeline = text_pipeline_builder.build(renderer());
    m_text_pipeline.set_debug_name("[gf2] Text Pipeline");

    // fullscreen pipeline

    Shader fullscreen_vertex_shader(gf::span(fullscreen_vert_shader_code), { ShaderStage::Vertex, renderer() });
    Shader fullscreen_fragment_shader(gf::span(fullscreen_frag_shader_code), { ShaderStage::Fragment, renderer() });

    RenderPipelineBuilder fullscreen_pipeline_builder;

    fullscreen_pipeline_builder.add_shader(&fullscreen_vertex_shader)
        .add_shader(&fullscreen_fragment_shader)
        .set_pipeline_layout(&m_fullscreen_pipeline_layout);

    m_fullscreen_pipeline = fullscreen_pipeline_builder.build(renderer());
    m_fullscreen_pipeline.set_debug_name("[gf2] Fullscreen Pipeline");

    // imgui pipeline

    RenderPipelineBuilder imgui_pipeline_builder;

    imgui_pipeline_builder.set_vertex_input(ImguiVertex::compute_input())
        .add_shader(&default_vertex_shader)
        .add_shader(&default_fragment_shader)
        .set_pipeline_layout(&m_default_pipeline_layout);

    m_imgui_pipeline = imgui_pipeline_builder.build(renderer());
    m_imgui_pipeline.set_debug_name("[gf2] Imgui Pipeline");

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
