// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Cursor.h>

#include <utility>

#include <SDL3/SDL.h>

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
      cursor_type_check<CursorType::Default, SDL_SYSTEM_CURSOR_DEFAULT>();
      cursor_type_check<CursorType::Text, SDL_SYSTEM_CURSOR_TEXT>();
      cursor_type_check<CursorType::Wait, SDL_SYSTEM_CURSOR_WAIT>();
      cursor_type_check<CursorType::Cross, SDL_SYSTEM_CURSOR_CROSSHAIR>();
      cursor_type_check<CursorType::Progress, SDL_SYSTEM_CURSOR_PROGRESS>();
      cursor_type_check<CursorType::SizeTopLeftBottomRight, SDL_SYSTEM_CURSOR_NWSE_RESIZE>();
      cursor_type_check<CursorType::SizeBottomLeftTopRight, SDL_SYSTEM_CURSOR_NESW_RESIZE>();
      cursor_type_check<CursorType::SizeHorizontal, SDL_SYSTEM_CURSOR_EW_RESIZE>();
      cursor_type_check<CursorType::SizeVertical, SDL_SYSTEM_CURSOR_NS_RESIZE>();
      cursor_type_check<CursorType::Move, SDL_SYSTEM_CURSOR_MOVE>();
      cursor_type_check<CursorType::Pointer, SDL_SYSTEM_CURSOR_POINTER>();
      cursor_type_check<CursorType::NotAllowed, SDL_SYSTEM_CURSOR_NOT_ALLOWED>();
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
      SDL_DestroyCursor(m_cursor);
    }
  }

  Cursor& Cursor::operator=(Cursor&& other) noexcept
  {
    std::swap(m_cursor, other.m_cursor);
    return *this;
  }

  void Cursor::set_mouse_cursor_visible(bool visible)
  {
    // TODO: [SDL3] check return
    if (visible) {
      SDL_ShowCursor();
    } else {
      SDL_HideCursor();
    }
  }

  void Cursor::set_mouse_cursor(const Cursor* cursor)
  {
    if (cursor == nullptr || cursor->m_cursor == nullptr) {
      return;
    }

    SDL_SetCursor(cursor->m_cursor);
  }

}
