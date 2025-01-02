// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/framework/ConsoleSystem.h>

namespace gf {

  ConsoleSystem::ConsoleSystem(Vec2I size, const ConsoleFont* console_font, RenderManager* render_manager)
  : m_render_manager(render_manager)
  , m_console_scene_manager(size)
  , m_console_entity(console_font)
  {
    m_console_entity.set_location({ 0.0f, 0.0f });
    m_console_entity.set_origin({ 0.5f, 0.5f });
    add_world_entity(&m_console_entity);

    set_world_size(size * console_font->character_size());
    set_world_center({ 0.0f, 0.0f });
  }

  void ConsoleSystem::do_process_event(const Event& event)
  {
    m_console_scene_manager.process_event(event);
  }

  void ConsoleSystem::do_handle_actions()
  {
    m_console_scene_manager.handle_actions();
  }

  void ConsoleSystem::do_update(Time time)
  {
    m_console_scene_manager.update(time);
    m_console_scene_manager.render();
    m_console_entity.graphics().update(m_console_scene_manager.console(), m_render_manager);
  }

}
