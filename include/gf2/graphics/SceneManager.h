// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SCENE_MANAGER_H
#define GF_SCENE_MANAGER_H

#include <string>
#include <vector>

#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "GpuCommandBuffer.h"
#include "GpuGraphicsPipeline.h"
#include "GpuShader.h"
#include "GpuTexture.h"
#include "GraphicsApi.h"
#include "GraphicsInitializer.h"
#include "RenderManager.h"
#include "RenderObject.h"
#include "Window.h"

namespace gf {
  class BasicScene;
  class RenderRecorder;

  class GF_GRAPHICS_API BasicSceneManager {
  public:
    BasicSceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints = All);

    Window* window()
    {
      return &m_window;
    }

    RenderManager* render_manager()
    {
      return &m_render_manager;
    }

  protected:
    GpuGraphicsPipeline* render_pipeline(RenderPipelineType type);
    void render_records(GpuRenderPass render_pass, RenderRecorder& recorder);

  private:
    void compile_shaders();
    void build_default_pipelines();

    GraphicsInitializer m_initializer;
    Window m_window;
    RenderManager m_render_manager;
    GpuTexture m_white;

    GpuShader m_default_vertex_shader;
    GpuShader m_default_fragment_shader;
    GpuShader m_text_fragment_shader;
    GpuShader m_fullscreen_vertex_shader;
    GpuShader m_fullscreen_fragment_shader;

    GpuGraphicsPipeline m_default_pipeline;
    GpuGraphicsPipeline m_text_pipeline;
    GpuGraphicsPipeline m_fullscreen_pipeline;
    GpuGraphicsPipeline m_imgui_pipeline;

    GpuGraphicsPipeline* m_last_pipeline = nullptr;
    RenderObject m_last_object = {};
  };

  class GF_GRAPHICS_API SingleSceneManager : public BasicSceneManager {
  public:
    SingleSceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints = All);

    int run();

    int run(BasicScene* scene)
    {
      set_scene(scene);
      return run();
    }

    void set_scene(BasicScene* scene);

  private:
    BasicScene* m_scene = nullptr;
  };

  class GF_GRAPHICS_API SceneManager : public BasicSceneManager {
  public:
    SceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints = All);

    int run();
    int run(BasicScene* scene)
    {
      push_scene(scene);
      return run();
    }

    void push_scene(BasicScene* scene);
    void push_scenes(Span<BasicScene*> scenes);

    void pop_scene();
    void pop_all_scenes();

    void replace_scene(BasicScene* scene)
    {
      pop_scene();
      push_scene(scene);
    }

    void replace_scene(Span<BasicScene*> scenes)
    {
      pop_scene();
      push_scenes(scenes);
    }

    void replace_all_scenes(BasicScene* scene)
    {
      pop_all_scenes();
      push_scene(scene);
    }

    void replace_all_scenes(Span<BasicScene*> scenes)
    {
      pop_all_scenes();
      push_scenes(scenes);
    }

  private:
    bool m_scenes_changed = false;
    std::vector<BasicScene*> m_curr_scenes;
    std::vector<BasicScene*> m_prev_scenes;
  };

}

#endif // GF_SCENE_MANAGER_H
