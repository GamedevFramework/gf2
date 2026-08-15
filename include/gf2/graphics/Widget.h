// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_WIDGET_H
#define GF_WIDGET_H

#include <cstdint>

#include <functional>

#include "GraphicsApi.h"
#include "TransformableEntity.h"

namespace gf {

  enum class WidgetState : uint8_t {
    Disabled,
    Default,
    Selected,
  };

  class GF_GRAPHICS_API Widget : public TransformableEntity {
  public:
    Widget(int32_t priority = 0);

    virtual bool contains(Vec2F pointer) = 0;

    void set_state(WidgetState state);
    WidgetState state() const;

    void set_disabled();
    bool is_disabled() const;

    void set_default();
    bool is_default() const;

    void set_selected();
    bool is_selected() const;

    void set_callback(std::function<void()> callback);
    void trigger_callback();

  private:
    virtual void on_state_change(WidgetState old_state, WidgetState new_state);
    virtual void on_trigger();

    WidgetState m_state = WidgetState::Default;
    std::function<void()> m_callback;
  };

}

#endif // GF_WIDGET_H
