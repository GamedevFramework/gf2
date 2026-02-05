// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_SCENE_H
#define GF_CONSOLE_SCENE_H

#include "Console.h"
#include "ConsoleElementContainer.h"
#include "ConsoleStyle.h"
#include "CoreApi.h"
#include "ModelContainer.h"
#include "SceneTypes.h"

namespace gf {
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

    void set_clear_style(const ConsoleStyle& style);
    const ConsoleStyle& clear_style() const;

    void add_model(Model* model);
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

    ConsoleStyle m_clear_style;
    ConsoleElementContainer m_elements;
    ModelContainer m_models;
  };

}

#endif // GF_CONSOLE_SCENE_H
