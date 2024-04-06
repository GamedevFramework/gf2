// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONSOLE_DATA_H
#define GF_CONSOLE_DATA_H

#include <cstdint>

#include <fmt/core.h>

#include "Array2D.h"
#include "Color.h"
#include "ConsoleEffect.h"
#include "CoreApi.h"
#include "Rect.h"
#include "TypeTraits.h"

namespace gf {

  struct GF_CORE_API ConsoleCell {
    Color foreground = White;
    Color background = Black;
    char16_t character = ' ';
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ConsoleCell, Archive>& cell)
  {
    return ar | cell.foreground | cell.background | cell.character;
  }

  enum class ConsoleAlignment {
    Left,
    Center,
    Right,
  };

  struct GF_CORE_API ConsoleStyle {
    Color foreground = White;
    Color background = Black;
    ConsoleEffect effect = ConsoleEffect::none();
    ConsoleAlignment alignment = ConsoleAlignment::Left;
  };

  struct GF_CORE_API ConsoleData {
    Array2D<ConsoleCell> screen;

    ConsoleData(Vec2I size = { 0, 0 })
    : screen(size)
    {
    }

    void clear(const ConsoleStyle& style = ConsoleStyle());
    void clear(RectI area, const ConsoleStyle& style = ConsoleStyle());

    void set_background(Vec2I position, Color color, ConsoleEffect effect = ConsoleEffect::set());
    Color background(Vec2I position) const;

    void set_foreground(Vec2I position, Color color);
    Color foreground(Vec2I position) const;

    void set_character(Vec2I position, char16_t character);
    char16_t character(Vec2I position) const;

    void put_character(Vec2I position, char16_t character, const ConsoleStyle& style = ConsoleStyle());
    void put_character(Vec2I position, char16_t character, Color foreground, Color background);

    template<typename... T>
    void print(Vec2I position, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
    {
      auto message = fmt::format(fmt, std::forward<T>(args)...);
      actual_print(RectI::from_position_size(position, { 0, 0 }), message, style);
    }

    template<typename... T>
    void print_area(RectI area, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
    {
      auto message = fmt::format(fmt, std::forward<T>(args)...);
      actual_print(area, message, style, PrintSplit);
    }

    template<typename... T>
    int print_area_height(RectI area, fmt::format_string<T...> fmt, T&&... args)
    {
      auto message = fmt::format(fmt, std::forward<T>(args)...);
      return actual_print(area, message, PrintSplit | PrintCount);
    }

    void draw_rectangle(RectI area, const ConsoleStyle& style = ConsoleStyle());
    void draw_horizontal_line(Vec2I left, int width, const ConsoleStyle& style = ConsoleStyle());
    void draw_vertical_line(Vec2I top, int height, const ConsoleStyle& style = ConsoleStyle());

    template<typename... T>
    void draw_frame(RectI area, const ConsoleStyle& style, fmt::format_string<T...> title, T&&... args)
    {
      auto actual_title = fmt::format(title, std::forward<T>(args)...);
      actual_draw_frame(area, style, actual_title);
    }

    void blit_to(ConsoleData& console, RectI source, Vec2I destination, float foreground_alpha = 1.0f, float background_alpha = 1.0f) const;

  private:
    static constexpr uint8_t PrintSplit = 0x01;
    static constexpr uint8_t PrintCount = 0x02;

    int put_string(Vec2I position, std::string_view message, const ConsoleStyle& style);
    int actual_print(RectI area, const std::string& message, const ConsoleStyle& style, uint8_t flags = 0x00);
    int actual_print_multiline(RectI area, const std::string& message, const ConsoleStyle& style, uint8_t flags = 0x00);
    void actual_draw_frame(RectI area, const ConsoleStyle& style, const std::string& title);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ConsoleData, Archive>& cell)
  {
    return ar | cell.screen;
  }

}

#endif // GF_CONSOLE_DATA_H
