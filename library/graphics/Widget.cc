// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Widget.h>

#include <utility>

namespace gf {

  Widget::Widget(int32_t priority)
  : TransformableEntity(priority)
  {
  }

  void Widget::set_state(WidgetState state)
  {
    const WidgetState old_state = std::exchange(m_state, state);

    if (old_state != state) {
      on_state_change(old_state, state);
    }
  }

  WidgetState Widget::state() const
  {
    return m_state;
  }

  void Widget::set_disabled()
  {
    set_state(WidgetState::Disabled);
  }

  bool Widget::is_disabled() const
  {
    return m_state == WidgetState::Disabled;
  }

  void Widget::set_default()
  {
    set_state(WidgetState::Default);
  }

  bool Widget::is_default() const
  {
    return m_state == WidgetState::Default;
  }

  void Widget::set_selected()
  {
    set_state(WidgetState::Selected);
  }

  bool Widget::is_selected() const
  {
    return m_state == WidgetState::Selected;
  }

  void Widget::set_callback(std::function<void()> callback)
  {
    m_callback = std::move(callback);
  }

  void Widget::trigger_callback()
  {
    on_trigger();

    if (m_callback) {
      m_callback();
    }
  }

  void Widget::on_state_change([[maybe_unused]] WidgetState old_state, [[maybe_unused]] WidgetState new_state)
  {
    // nothing to do, by default
  }

  void Widget::on_trigger()
  {
    // nothing to do, by default
  }

}

