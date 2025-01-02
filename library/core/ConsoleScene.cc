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

  void ConsoleScene::add_element(ConsoleElement* element)
  {
    m_elements.add_element(element);
  }

  void ConsoleScene::process_event([[maybe_unused]] const Event& event)
  {
    // do nothing by default
  }

  void ConsoleScene::handle_actions()
  {
    // do nothing by default
  }

  void ConsoleScene::update()
  {
    update_elements();
  }

  void ConsoleScene::render([[maybe_unused]] Console& buffer)
  {
    render_elements(buffer);
  }

  void ConsoleScene::update_elements()
  {
    m_elements.update();
  }

  void ConsoleScene::render_elements(Console& buffer)
  {
    m_elements.render(buffer);
  }

}
