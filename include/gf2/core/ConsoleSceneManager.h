// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_SCENE_MANAGER_H
#define GF_CONSOLE_SCENE_MANAGER_H

#include <vector>

#include "Console.h"
#include "ConsoleScene.h"
#include "CoreApi.h"
#include "Vec2.h"

namespace gf {

  class GF_CORE_API ConsoleSceneManager {
  public:
    ConsoleSceneManager(Vec2I size);

    void push_scene(ConsoleScene* scene);

    void pop_scene();
    void pop_all_scenes();

    void replace_scene(ConsoleScene* scene);

    void process_event(const Event& event);
    void handle_actions();
    void update(Time time);
    void render();

    Console& console();

  private:
    Console m_console;
    std::vector<ConsoleScene*> m_scenes;
  };

}

#endif // GF_CONSOLE_SCENE_MANAGER_H
