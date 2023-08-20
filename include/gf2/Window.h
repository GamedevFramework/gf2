// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_WINDOW_H
#define GF_WINDOW_H

#include <string>

#include "Clock.h"
#include "Flags.h"
#include "GraphicsApi.h"
#include "Time.h"
#include "Vec2.h"

using SDL_Window = struct SDL_Window;

namespace gf {

  enum class WindowHints : uint8_t {
    Resizable = 0x01,
    Visible = 0x02,
    Decorated = 0x04,
  };

  template<>
  struct EnableBitmaskOperators<WindowHints> : std::true_type {
  };

  class GF_GRAPHICS_API Window {
  public:
    Window(const std::string& title, Vec2I size, Flags<WindowHints> hints = All);
    Window(const Window&) = delete;
    Window(Window&& other) noexcept;
    ~Window();

    Window& operator=(const Window&) = delete;
    Window& operator=(Window&& other) noexcept;

    uint32_t id();

    bool closed() const;
    void close();

    void set_title(const std::string& title);
    std::string title();

    void set_position(Vec2I position);
    Vec2I position();

    void set_size(Vec2I size);
    Vec2I size();

    void set_minimum_size(Vec2I size);
    Vec2I minimum_size();

    void set_maximum_size(Vec2I size);
    Vec2I maximum_size();

    void minimize();
    bool minimized();

    void maximize();
    bool maximized();

    void set_fullscreen(bool fullscreen = true);
    void toggle_fullscreen();
    bool fullscreen() const;

    void set_resizable(bool resizable = true);
    bool resizable();

    void set_visible(bool visible = true);
    bool visible();

    void set_decorated(bool decorated = true);
    bool decorated();

  private:
    friend class BasicRenderer;

    SDL_Window* m_window = nullptr;
    bool m_should_close = false;
    bool m_fullscreen = false;
  };

}

#endif // GF_WINDOW_H
