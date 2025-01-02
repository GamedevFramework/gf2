// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_SCENE_H
#define GF_CONSOLE_SCENE_H

#include "ConsoleElementContainer.h"
#include "CoreApi.h"
#include "SceneTypes.h"

namespace gf {
  class Console;
  class Event;

  class GF_CORE_API ConsoleScene {
  public:
    ConsoleScene() = default;
    ConsoleScene(const ConsoleScene&) = delete;
    ConsoleScene(ConsoleScene&&) noexcept = default;
    virtual ~ConsoleScene();

    ConsoleScene& operator=(const ConsoleScene&) = delete;
    ConsoleScene& operator=(ConsoleScene&&) noexcept = default;

    void set_rank(SceneRank rank);
    SceneRank rank() const;

    void pause();
    void resume();
    bool paused() const;

    void hide();
    void show();
    bool hidden() const;

    void add_element(ConsoleElement* element);

    virtual void process_event(const Event& event);
    virtual void handle_actions();
    virtual void update(Time time);
    virtual void render(Console& buffer);

  protected:
    void update_elements(Time time);
    void render_elements(Console& buffer);

  private:
    SceneRank m_rank = SceneRank::None;
    SceneStatus m_status = SceneStatus::Resumed;
    SceneVisibility m_visibility = SceneVisibility::Shown;

    ConsoleElementContainer m_elements;
  };

}

#endif // GF_CONSOLE_SCENE_H
