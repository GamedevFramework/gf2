// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CONSOLE_DATA_H
#define GF_CONSOLE_DATA_H

#include <cstdint>

#include <filesystem>

#include <fmt/core.h>

#include "Array2D.h"
#include "Color.h"
#include "ConsoleEffect.h"
#include "ConsoleStyle.h"
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
      ConsoleRichStyle rich_style(style);
      raw_print(RectI::from_position_size(position, { 0, 0 }), message, rich_style, PrintSimple);
    }

    template<typename... T>
    void print_area(RectI area, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
    {
      auto message = fmt::format(fmt, std::forward<T>(args)...);
      ConsoleRichStyle rich_style(style);
      raw_print(area, message, rich_style, PrintSplit | PrintSimple);
    }

    template<typename... T>
    void print(Vec2I position, const ConsoleRichStyle& style, fmt::format_string<T...> fmt, T&&... args)
    {
      auto message = fmt::format(fmt, std::forward<T>(args)...);
      raw_print(RectI::from_position_size(position, { 0, 0 }), message, style);
    }

    template<typename... T>
    void print_area(RectI area, const ConsoleRichStyle& style, fmt::format_string<T...> fmt, T&&... args)
    {
      auto message = fmt::format(fmt, std::forward<T>(args)...);
      raw_print(area, message, style, PrintSplit);
    }

    template<typename... T>
    int print_area_height(RectI area, fmt::format_string<T...> fmt, T&&... args)
    {
      auto message = fmt::format(fmt, std::forward<T>(args)...);
      return raw_print(area, message, PrintSplit | PrintCount);
    }

    void draw_rectangle(RectI area, const ConsoleStyle& style = ConsoleStyle());
    void draw_horizontal_line(Vec2I left, int width, const ConsoleStyle& style = ConsoleStyle());
    void draw_vertical_line(Vec2I top, int height, const ConsoleStyle& style = ConsoleStyle());

    void draw_frame(RectI area, const ConsoleStyle& style)
    {
      raw_draw_frame(area, style, "");
    }

    template<typename... T>
    void draw_frame(RectI area, const ConsoleStyle& style, fmt::format_string<T...> title, T&&... args)
    {
      auto actual_title = fmt::format(title, std::forward<T>(args)...);
      raw_draw_frame(area, style, actual_title);
    }

    void blit_to(ConsoleData& console, RectI source, Vec2I destination, float foreground_alpha = 1.0f, float background_alpha = 1.0f) const;

  private:
    static constexpr uint8_t PrintSplit = 0x01;
    static constexpr uint8_t PrintCount = 0x02;
    static constexpr uint8_t PrintSimple = 0x04;

    int put_string(Vec2I position, std::string_view message, const ConsoleStyle& style);
    int raw_print(RectI area, const std::string& message, const ConsoleRichStyle& style, uint8_t flags = 0x00);
    int raw_print_multiline(RectI area, const std::string& message, const ConsoleRichStyle& style, uint8_t flags = 0x00);
    void raw_draw_frame(RectI area, const ConsoleStyle& style, std::string_view title);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ConsoleData, Archive>& data)
  {
    return ar | data.screen;
  }

  struct GF_CORE_API ConsoleResource {
    std::filesystem::path console_font;
    ConsoleData data;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MaybeConst<ConsoleResource, Archive>& resource)
  {
    return ar | resource.console_font | resource.data;
  }

}

#endif // GF_CONSOLE_DATA_H
