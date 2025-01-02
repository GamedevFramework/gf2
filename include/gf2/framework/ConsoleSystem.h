// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_SYSTEM_H
#define GF_CONSOLE_SYSTEM_H

#include <gf2/core/ConsoleSceneManager.h>

#include <gf2/graphics/Scene.h>
#include <gf2/graphics/ConsoleEntity.h>

#include "FrameworkApi.h"

namespace gf {

  class GF_FRAMEWORK_API ConsoleSystem : public Scene {
  public:
    ConsoleSystem(Vec2I size, const ConsoleFont* font, RenderManager* render_manager);


    ConsoleSceneManager* console_scene_manager()
    {
      return &m_console_scene_manager;
    }

  private:
    void do_process_event(const Event& event) override;
    void do_handle_actions() override;
    void do_update(Time time) override;

    RenderManager* m_render_manager = nullptr;
    ConsoleSceneManager m_console_scene_manager;
    ConsoleEntity m_console_entity;
  };

}

#endif // GF_CONSOLE_SYSTEM_H
