// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CURSOR_H
#define GF_CURSOR_H

#include <cstdint>

#include "GraphicsApi.h"

struct SDL_Cursor;

namespace gf {

  enum class CursorType : uint8_t {
    Default,
    Text,
    Wait,
    Cross,
    Progress,
    SizeTopLeftBottomRight,
    SizeBottomLeftTopRight,
    SizeHorizontal,
    SizeVertical,
    Move,
    NotAllowed,
    Pointer,
  };

  class GF_GRAPHICS_API Cursor {
  public:
    Cursor() = default;
    Cursor(CursorType type);
    Cursor(const Cursor&) = delete;
    Cursor(Cursor&& other) noexcept;
    ~Cursor();

    Cursor& operator=(const Cursor&) = delete;
    Cursor& operator=(Cursor&& other) noexcept;

    static void set_mouse_cursor_visible(bool visible = true);
    static void set_mouse_cursor(const Cursor* cursor);

  private:
    SDL_Cursor* m_cursor = nullptr;
  };

}

#endif // GF_CURSOR_H
