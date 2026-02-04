// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Window.h>

#include <type_traits>
#include <utility>

#include <SDL3/SDL.h>

#include <gf2/core/Log.h>

namespace gf {

  static_assert(std::is_same_v<std::underlying_type_t<WindowId>, uint32_t>); // TODO: [SDL3] use SDL_WindowID

  namespace {
    Uint32 to_flags(Flags<WindowHints> hints)
    {
      Uint32 flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;

#ifdef SDL_PLATFORM_MACOS
      flags |= SDL_WINDOW_METAL;
#else
      flags |= SDL_WINDOW_VULKAN;
#endif

      if (hints.test(WindowHints::Resizable)) {
        flags |= SDL_WINDOW_RESIZABLE;
      }

      if (!hints.test(WindowHints::Visible)) {
        flags |= SDL_WINDOW_HIDDEN;
      }

      if (!hints.test(WindowHints::Decorated)) {
        flags |= SDL_WINDOW_BORDERLESS;
      }

      return flags;
    }

  }

  Window::Window(const std::string& title, Vec2I size, Flags<WindowHints> hints)
  {
    auto flags = to_flags(hints);
    m_window = SDL_CreateWindow(title.c_str(), size.w, size.h, flags);

    if (m_window == nullptr) {
      Log::fatal("Failed to create a window: {}", SDL_GetError());
    }
  }

  Window::Window(Window&& other) noexcept
  : m_window(std::exchange(other.m_window, nullptr))
  , m_should_close(other.m_should_close)
  , m_fullscreen(other.m_fullscreen)
  {
  }

  Window::~Window()
  {
    if (m_window != nullptr) {
      SDL_DestroyWindow(m_window);
    }
  }

  Window& Window::operator=(Window&& other) noexcept
  {
    std::swap(m_window, other.m_window);
    std::swap(m_should_close, other.m_should_close);
    std::swap(m_fullscreen, other.m_fullscreen);
    return *this;
  }

  WindowId Window::id()
  {
    assert(m_window);
    const uint32_t id = SDL_GetWindowID(m_window);

    if (id == 0) {
      Log::warning("Failed to get the window id: {}", SDL_GetError());
    }

    return WindowId{id};
  }

  bool Window::should_close() const
  {
    return m_should_close;
  }

  void Window::close()
  {
    m_should_close = true;
  }

  void Window::set_title(const std::string& title)
  {
    assert(m_window);
    SDL_SetWindowTitle(m_window, title.c_str());
    // TODO: [SDL3] check return
  }

  std::string Window::title()
  {
    assert(m_window);
    return SDL_GetWindowTitle(m_window);
  }

  void Window::set_position(Vec2I position)
  {
    assert(m_window);
    SDL_SetWindowPosition(m_window, position.x, position.y);
    // TODO: [SDL3] check return
  }

  Vec2I Window::position()
  {
    assert(m_window);
    Vec2I position = {};
    SDL_GetWindowPosition(m_window, &position.x, &position.y);
    // TODO: [SDL3] check return
    return position;
  }

  void Window::set_size(Vec2I size)
  {
    assert(m_window);
    SDL_SetWindowSize(m_window, size.w, size.h);
    // TODO: [SDL3] check return
  }

  Vec2I Window::size()
  {
    assert(m_window);
    Vec2I size = {};
    SDL_GetWindowSize(m_window, &size.w, &size.h);
    // TODO: [SDL3] check return
    return size;
  }

  Vec2I Window::surface_size()
  {
    assert(m_window);
    Vec2I size = {};
    SDL_GetWindowSizeInPixels(m_window, &size.w, &size.h);
    // TODO: [SDL3] check return
    return size;
  }

  void Window::set_minimum_size(Vec2I size)
  {
    assert(m_window);
    SDL_SetWindowMinimumSize(m_window, size.x, size.y);
    // TODO: [SDL3] check return
  }

  Vec2I Window::minimum_size()
  {
    assert(m_window);
    Vec2I size = {};
    SDL_GetWindowMinimumSize(m_window, &size.x, &size.y);
    // TODO: [SDL3] check return
    return size;
  }

  void Window::set_maximum_size(Vec2I size)
  {
    assert(m_window);
    SDL_SetWindowMaximumSize(m_window, size.x, size.y);
    // TODO: [SDL3] check return
  }

  Vec2I Window::maximum_size()
  {
    assert(m_window);
    Vec2I size = {};
    SDL_GetWindowMaximumSize(m_window, &size.x, &size.y);
    // TODO: [SDL3] check return
    return size;
  }

  void Window::minimize()
  {
    assert(m_window);
    SDL_MinimizeWindow(m_window);
  }

  bool Window::minimized()
  {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_MINIMIZED) != 0;
  }

  void Window::maximize()
  {
    assert(m_window);
    SDL_MaximizeWindow(m_window);
  }

  bool Window::maximized()
  {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_MAXIMIZED) != 0;
  }

  void Window::set_fullscreen(bool fullscreen)
  {
    assert(m_window);
    SDL_SetWindowFullscreen(m_window, fullscreen);
    // TODO: [SDL3] check return
    m_fullscreen = fullscreen;
  }

  void Window::toggle_fullscreen()
  {
    set_fullscreen(!m_fullscreen);
  }

  bool Window::fullscreen() const
  {
    return m_fullscreen;
  }

  void Window::set_resizable(bool resizable)
  {
    assert(m_window);
    SDL_SetWindowResizable(m_window, resizable);
  }

  bool Window::resizable()
  {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_RESIZABLE) != 0;
  }

  void Window::set_visible(bool visible)
  {
    assert(m_window);

    if (visible) {
      SDL_ShowWindow(m_window);
    } else {
      SDL_HideWindow(m_window);
    }
  }

  bool Window::visible()
  {
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_HIDDEN) == 0;
  }

  void Window::set_decorated(bool decorated)
  {
    assert(m_window);
    SDL_SetWindowBordered(m_window, decorated);
  }

  bool Window::decorated()
  {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_BORDERLESS) == 0;
  }

  void Window::start_text_input()
  {
    bool result = SDL_StartTextInput(m_window);

    if (!result) {
      Log::fatal("Could not start text input: {}", SDL_GetError());
    }
  }

  void Window::stop_text_input()
  {
    bool result = SDL_StopTextInput(m_window);

    if (!result) {
      Log::fatal("Could not start text input: {}", SDL_GetError());
    }
  }

  bool Window::text_input_active()
  {
    return SDL_TextInputActive(m_window);
  }

}
