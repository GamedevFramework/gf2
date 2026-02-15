// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ConsoleScene.h>

namespace gf {

  ConsoleScene::~ConsoleScene() = default;

  void ConsoleScene::set_rank(SceneRank rank)
  {
    m_rank = rank;
  }

  SceneRank ConsoleScene::rank() const
  {
    return m_rank;
  }

  void ConsoleScene::pause()
  {
    m_status = SceneStatus::Paused;
  }

  void ConsoleScene::resume()
  {
    m_status = SceneStatus::Resumed;
  }

  bool ConsoleScene::paused() const
  {
    return m_status == SceneStatus::Paused;
  }

  void ConsoleScene::hide()
  {
    m_visibility = SceneVisibility::Hidden;
  }

  void ConsoleScene::show()
  {
    m_visibility = SceneVisibility::Shown;
  }

  bool ConsoleScene::hidden() const
  {
    return m_visibility == SceneVisibility::Hidden;
  }

  void ConsoleScene::set_clear_style(const ConsoleStyle& style)
  {
    m_clear_style = style;
  }

  const ConsoleStyle& ConsoleScene::clear_style() const
  {
    return m_clear_style;
  }

  void ConsoleScene::add_model(Model* model)
  {
    m_models.add_model(model);
  }

  void ConsoleScene::add_entity(ConsoleEntity* entity)
  {
    m_entities.add_entity(entity);
  }

  void ConsoleScene::process_event([[maybe_unused]] const Event& event)
  {
    // do nothing by default
  }

  void ConsoleScene::handle_actions()
  {
    // do nothing by default
  }

  void ConsoleScene::update(Time time)
  {
    update_entities(time);
  }

  void ConsoleScene::render(Console& buffer)
  {
    render_entities(buffer);
  }

  void ConsoleScene::update_entities(Time time)
  {
    m_models.update(time);
    m_entities.update(time);
  }

  void ConsoleScene::render_entities(Console& buffer)
  {
    m_entities.render(buffer);
  }

}
