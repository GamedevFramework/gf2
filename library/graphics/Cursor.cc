// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Cursor.h>

#include <utility>

#include <SDL2/SDL.h>

#include <gf2/core/Log.h>

namespace gf {

  namespace {

    template<CursorType Type, SDL_SystemCursor Value>
    constexpr void cursor_type_check()
    {
      static_assert(static_cast<SDL_SystemCursor>(Type) == Value);
    }

    [[maybe_unused]] constexpr void cursor_check()
    {
      cursor_type_check<CursorType::Arrow, SDL_SYSTEM_CURSOR_ARROW>();
      cursor_type_check<CursorType::Text, SDL_SYSTEM_CURSOR_IBEAM>();
      cursor_type_check<CursorType::Wait, SDL_SYSTEM_CURSOR_WAIT>();
      cursor_type_check<CursorType::Cross, SDL_SYSTEM_CURSOR_CROSSHAIR>();
      cursor_type_check<CursorType::ArrowWait, SDL_SYSTEM_CURSOR_WAITARROW>();
      cursor_type_check<CursorType::SizeTopLeftBottomRight, SDL_SYSTEM_CURSOR_SIZENWSE>();
      cursor_type_check<CursorType::SizeBottomLeftTopRight, SDL_SYSTEM_CURSOR_SIZENESW>();
      cursor_type_check<CursorType::SizeHorizontal, SDL_SYSTEM_CURSOR_SIZEWE>();
      cursor_type_check<CursorType::SizeVertical, SDL_SYSTEM_CURSOR_SIZENS>();
      cursor_type_check<CursorType::SizeAll, SDL_SYSTEM_CURSOR_SIZEALL>();
      cursor_type_check<CursorType::Hand, SDL_SYSTEM_CURSOR_HAND>();
      cursor_type_check<CursorType::NotAllowed, SDL_SYSTEM_CURSOR_NO>();
    }

  }

  Cursor::Cursor(CursorType type)
  : m_cursor(SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(type)))
  {
    if (m_cursor == nullptr) {
      Log::fatal("Could not load system cursor: {}.", SDL_GetError());
    }
  }

  Cursor::Cursor(Cursor&& other) noexcept
  : m_cursor(std::exchange(other.m_cursor, nullptr))
  {
  }

  Cursor::~Cursor()
  {
    if (m_cursor != nullptr) {
      SDL_FreeCursor(m_cursor);
    }
  }

  Cursor& Cursor::operator=(Cursor&& other) noexcept
  {
    std::swap(m_cursor, other.m_cursor);
    return *this;
  }

  void Cursor::set_mouse_cursor_visible(bool visible)
  {
    SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
  }

  void Cursor::set_mouse_cursor(const Cursor* cursor)
  {
    if (cursor == nullptr || cursor->m_cursor == nullptr) {
      return;
    }

    SDL_SetCursor(cursor->m_cursor);
  }

}
