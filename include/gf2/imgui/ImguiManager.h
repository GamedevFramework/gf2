// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_IMGUI_MANAGER_H
#define GF_IMGUI_MANAGER_H

#include <gf2/core/Time.h>

#include <gf2/graphics/Cursor.h>
#include <gf2/graphics/Event.h>
#include <gf2/graphics/RenderPipeline.h>
#include <gf2/graphics/Renderer.h>
#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Window.h>

#include "ImguiApi.h"

namespace gf {

  class GF_IMGUI_API ImguiManager {
  public:
    ImguiManager(Window* window, Renderer* renderer);

    bool process_event(const Event& event);
    void update(Time time);

  private:
    void update_keyboard_key(Keycode keycode, Flags<Modifier> modifiers, bool pressed);
    void update_mouse_button(MouseButton button, bool pressed);
    void udpate_gamepad_button(GamepadButton button, bool pressed, float value);
    void update_gamepad_axis(GamepadAxis axis, int16_t raw);

    void update_mouse_cursor();

    Texture m_font_texture;
    CursorType m_cursor_type = CursorType::Arrow;
    Cursor m_cursor;
  };

}

#endif // GF_IMGUI_MANAGER_H
