// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ConsoleSceneManager.h>

#include <cassert>

namespace gf {

  ConsoleSceneManager::ConsoleSceneManager(Vec2I size)
  : m_console(size)
  {
  }

  void ConsoleSceneManager::push_scene(ConsoleScene* scene)
  {
    if (scene == nullptr) {
      return;
    }

    if (!m_scenes.empty()) {
      m_scenes.back()->set_rank(SceneRank::Active);
    }

    m_scenes.push_back(scene);
    scene->set_rank(SceneRank::Top);
  }

  void ConsoleSceneManager::pop_scene()
  {
    assert(!m_scenes.empty());
    m_scenes.back()->set_rank(SceneRank::None);
    m_scenes.pop_back();

    if (!m_scenes.empty()) {
      m_scenes.back()->set_rank(SceneRank::Top);
    }
  }

  void ConsoleSceneManager::pop_all_scenes()
  {
    for (ConsoleScene* scene : m_scenes) {
      scene->set_rank(SceneRank::None);
    }

    m_scenes.clear();
  }

  void ConsoleSceneManager::replace_scene(ConsoleScene* scene)
  {
    pop_scene();
    push_scene(scene);
  }

  void ConsoleSceneManager::process_event(const Event& event)
  {
    for (ConsoleScene* scene : m_scenes) {
      scene->process_event(event);
    }
  }

  void ConsoleSceneManager::handle_actions()
  {
    for (ConsoleScene* scene : m_scenes) {
      scene->handle_actions();
    }
  }

  void ConsoleSceneManager::update(Time time)
  {
    for (ConsoleScene* scene : m_scenes) {
      if (!scene->paused()) {
        scene->update(time);
      }
    }
  }

  void ConsoleSceneManager::render()
  {
    if (m_scenes.empty()) {
      return;
    }

    m_console.clear(m_scenes.back()->clear_style());

    for (ConsoleScene* scene : m_scenes) {
      if (!scene->hidden()) {
        scene->render(m_console);
      }
    }
  }

  Console& ConsoleSceneManager::console()
  {
    return m_console;
  }

}
