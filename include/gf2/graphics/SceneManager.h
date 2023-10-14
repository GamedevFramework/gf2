// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SCENE_MANAGER_H
#define GF_SCENE_MANAGER_H

#include <string>
#include <vector>

#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "CommandBuffer.h"
#include "GraphicsApi.h"
#include "GraphicsInitializer.h"
#include "Renderer.h"
#include "RenderObject.h"
#include "Texture.h"
#include "Window.h"

namespace gf {
  class Scene;
  class RenderRecorder;

  class GF_GRAPHICS_API BasicSceneManager {
  public:
    BasicSceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints = All);

    Window* window()
    {
      return &m_window;
    }

    Renderer* renderer()
    {
      return &m_renderer;
    }

  protected:
    const RenderPipeline* render_pipeline(RenderPipelineType type) const;
    void render_objects(RenderCommandBuffer command_buffer, const RenderRecorder& recorder);

  private:
    GraphicsInitializer m_initializer;
    Window m_window;
    Renderer m_renderer;
    Texture m_white;
  };

  class GF_GRAPHICS_API SingleSceneManager : public BasicSceneManager {
  public:
    SingleSceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints = All);

    void run();

    void run(Scene* scene)
    {
      set_scene(scene);
      run();
    }

    void set_scene(Scene* scene);

  private:
    Scene* m_scene = nullptr;
  };

  class GF_GRAPHICS_API SceneManager : public BasicSceneManager {
  public:
    SceneManager(const std::string& title, Vec2I size, Flags<WindowHints> hints = All);

    void run();
    void run(Scene* scene)
    {
      push_scene(scene);
      run();
    }

    void push_scene(Scene* scene);
    void push_scenes(Span<Scene*> scenes);

    void pop_scene();
    void pop_all_scenes();

    void replace_scene(Scene* scene)
    {
      pop_scene();
      push_scene(scene);
    }

    void replace_scene(Span<Scene*> scenes)
    {
      pop_scene();
      push_scenes(scenes);
    }

    void replace_all_scenes(Scene* scene)
    {
      pop_all_scenes();
      push_scene(scene);
    }

    void replace_all_scenes(Span<Scene*> scenes)
    {
      pop_all_scenes();
      push_scenes(scenes);
    }

  private:
    bool m_scenes_changed = false;
    std::vector<Scene*> m_curr_scenes;
    std::vector<Scene*> m_prev_scenes;
  };

}

#endif // GF_SCENE_MANAGER_H
