// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_WIDGET_CONTAINER_H
#define GF_WIDGET_CONTAINER_H

#include <optional>
#include <vector>

#include <gf2/core/Vec2.h>

#include "GraphicsApi.h"

namespace gf {
  class Widget;

  class GF_GRAPHICS_API WidgetContainer {
  public:

    bool empty() const;
    std::size_t size() const;
    void add_widget(Widget* widget);
    void remove_widget(Widget* widget);
    void clear();

    void point_to(Vec2F pointer);
    void select_next_widget();
    void select_prev_widget();
    void trigger_selected();

  private:
    void unselect_currently_selected();
    void compute_next_index();
    void compute_prev_index();
    Widget* current_widget();

    std::optional<std::size_t> m_selected;
    std::vector<Widget*> m_widgets;
  };

}

#endif // GF_WIDGET_CONTAINER_H
