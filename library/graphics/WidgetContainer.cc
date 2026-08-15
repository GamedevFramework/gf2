// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/WidgetContainer.h>

#include <cassert>

#include <gf2/core/Range.h>
#include <gf2/graphics/Widget.h>

namespace gf {

  bool WidgetContainer::empty() const
  {
    return m_widgets.empty();
  }

  std::size_t WidgetContainer::size() const
  {
    return m_widgets.size();
  }

  void WidgetContainer::add_widget(Widget* widget)
  {
    if (widget == nullptr) {
      return;
    }

    m_widgets.push_back(widget);
  }

  void WidgetContainer::remove_widget(Widget* widget)
  {
    if (widget == nullptr) {
      return;
    }

    std::erase(m_widgets, widget);
  }

  void WidgetContainer::clear()
  {
    m_widgets.clear();
    m_selected = std::nullopt;
  }

  void WidgetContainer::point_to(Vec2F pointer)
  {
    if (m_widgets.empty()) {
      return;
    }

    unselect_currently_selected();
    m_selected = std::nullopt;

    for (auto [ index, widget ] : gf::enumerate(m_widgets)) {
      if (widget->is_default() && widget->contains(pointer)) {
        m_selected = index;
        widget->set_selected();
        break; // to avoid multiple selection
      }
    }
  }

  void WidgetContainer::select_next_widget()
  {
    if (m_widgets.empty()) {
      return;
    }

    unselect_currently_selected();

    // select the new one

    if (m_selected) {
      compute_next_index();
    } else {
      m_selected = 0;
    }

    const std::size_t max_count = m_widgets.size();
    std::size_t count = 0;

    while (count < max_count && !current_widget()->is_default()) {
      compute_next_index();
      ++count;
    }

    if (count == max_count) {
      m_selected = std::nullopt;
      return;
    }

    current_widget()->set_selected();
  }

  void WidgetContainer::select_prev_widget()
  {
    if (m_widgets.empty()) {
      return;
    }

    unselect_currently_selected();

    // select the new one

    if (m_selected) {
      compute_prev_index();
    } else {
      m_selected = 0;
    }

    const std::size_t max_count = m_widgets.size();
    std::size_t count = 0;

    while (count < max_count && !current_widget()->is_default()) {
      compute_prev_index();
      ++count;
    }

    if (count == max_count) {
      m_selected = std::nullopt;
      return;
    }

    current_widget()->set_selected();
  }

  void WidgetContainer::trigger_selected()
  {
    if (m_widgets.empty()) {
      return;
    }

    if (m_selected) {
      current_widget()->trigger_callback();
      // unselect_currently_selected();
      // m_selected = std::nullopt;
    }
  }

  void WidgetContainer::unselect_currently_selected()
  {
    if (m_selected) {
      assert(current_widget()->is_selected());
      current_widget()->set_default();
    }
  }

  void WidgetContainer::compute_next_index()
  {
    assert(m_selected);
    ++m_selected.value();

    if (m_selected.value() >= m_widgets.size()) {
      m_selected = 0;
    }
  }

  void WidgetContainer::compute_prev_index()
  {
    assert(m_selected);
    if (m_selected.value() == 0) {
      m_selected = m_widgets.size();
    }

    --m_selected.value();
  }

  Widget* WidgetContainer::current_widget()
  {
    assert(m_selected);
    assert(m_selected.value() < m_widgets.size());
    return m_widgets[m_selected.value()];
  }

}
