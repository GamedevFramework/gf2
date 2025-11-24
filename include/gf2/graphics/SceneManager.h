// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SCENE_MANAGER_H
#define GF_SCENE_MANAGER_H

#include <string>
#include <vector>

#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "CommandBuffer.h"
#include "GraphicsApi.h"
#include "GraphicsInitializer.h"
#include "RenderManager.h"
#include "RenderObject.h"
#include "Texture.h"
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
    const RenderPipeline* render_pipeline(RenderPipelineType type) const;
    void render_records(RenderPass render_pass, const RenderRecorder& recorder);
    void render_object(RenderPass render_pass, RenderObject object);

  private:
    void build_default_pipelines();

    GraphicsInitializer m_initializer;
    Window m_window;
    RenderManager m_render_manager;
    Texture m_white;

    DescriptorLayout m_camera_descriptor_layout;
    DescriptorLayout m_sampler_descriptor_layout;
    DescriptorLayout m_text_descriptor_layout;

    RenderPipelineLayout m_default_pipeline_layout;
    RenderPipelineLayout m_text_pipeline_layout;
    RenderPipelineLayout m_fullscreen_pipeline_layout;

    RenderPipeline m_default_pipeline;
    RenderPipeline m_text_pipeline;
    RenderPipeline m_fullscreen_pipeline;
    RenderPipeline m_imgui_pipeline;

    const RenderPipeline* m_last_pipeline = nullptr;
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
