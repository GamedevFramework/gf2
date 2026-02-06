// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_OPERATIONS_H
#define GF_CONSOLE_OPERATIONS_H

#include <fmt/core.h>

#include "Console.h"
#include "ConsoleEffect.h"
#include "ConsoleStyle.h"
#include "CoreApi.h"
#include "Rect.h"

namespace gf {

  namespace details {

    enum class ConsoleStyleChoice : uint8_t {
      Simple,
      Rich,
    };

    enum class ConsoleAction : uint8_t {
      Print,
      Count,
    };

    struct ConsolePrintParameters {
      ConsoleRichStyle style;
      ConsoleStyleChoice style_choice = ConsoleStyleChoice::Simple;
      ConsoleAction action = ConsoleAction::Print;
      ConsoleAlignment alignment = ConsoleAlignment::Left;
      ConsoleMode mode = ConsoleMode::Picture;
    };

    int32_t raw_console_print_line(Console& console, Vec2I position, const ConsolePrintParameters& params, std::string_view message);
    int32_t raw_console_print_multiline(Console& console, RectI area, const ConsolePrintParameters& params, std::string_view message);
    void raw_draw_frame(Console& console, RectI area, const ConsoleStyle& style, ConsoleMode mode, std::string_view title);

  }

  GF_CORE_API void console_clear(Console& console, const ConsoleStyle& style);
  GF_CORE_API void console_clear(Console& console, RectI area, const ConsoleStyle& style);

  GF_CORE_API void console_write_background(Console& console, Vec2I position, Color color, ConsoleEffect effect = ConsoleEffect::set());

  GF_CORE_API void console_write_picture(Console& console, Vec2I position, char16_t character);
  GF_CORE_API void console_write_picture(Console& console, Vec2I position, char16_t character, const ConsoleStyle& style);
  GF_CORE_API int32_t console_write_picture(Console& console, Vec2I position, std::string_view message, const ConsoleStyle& style);
  GF_CORE_API void console_write_text(Console& console, Vec2I position, uint8_t part_index, char16_t character);
  GF_CORE_API void console_write_text(Console& console, Vec2I position, uint8_t part_index, char16_t character, const ConsoleStyle& style);
  GF_CORE_API int32_t console_write_text(Console& console, Vec2I position, std::string_view message, const ConsoleStyle& style);

  GF_CORE_API void console_draw_rectangle(Console& console, RectI area, const ConsoleStyle& style);
  GF_CORE_API void console_draw_horizontal_line(Console& console, Vec2I left, int width, const ConsoleStyle& style);
  GF_CORE_API void console_draw_vertical_line(Console& console, Vec2I top, int height, const ConsoleStyle& style);
  inline void console_draw_frame(Console& console, RectI area, const ConsoleStyle& style)
  {
    details::raw_draw_frame(console, area, style, ConsoleMode::Picture, "");
  }
  template<typename... T>
  void console_draw_frame(Console& console, RectI area, const ConsoleStyle& style, ConsoleMode mode, fmt::format_string<T...> title, T&&... args)
  {
    const std::string message = fmt::format(title, std::forward<T>(args)...);
    details::raw_draw_frame(console, area, style, mode, message);
  }

  GF_CORE_API void console_blit_to(const Console& origin_console, Console& target_console, RectI origin, Vec2I target, float foreground_alpha = 1.0f, float background_alpha = 1.0f);
  inline void console_blit_to(const Console& origin_console, Console& target_console, Vec2I target, float foreground_alpha = 1.0f, float background_alpha = 1.0f)
  {
    console_blit_to(origin_console, target_console, RectI::from_size(origin_console.size()), target, foreground_alpha, background_alpha);
  }

  /*
   * simple style, picture
   */

  template<typename... T>
  void console_print_picture(Console& console, Vec2I position, ConsoleAlignment alignment, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Simple,
      .action = details::ConsoleAction::Print,
      .alignment = alignment,
      .mode = ConsoleMode::Picture
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    details::raw_console_print_line(console, position, params, message);
  }

  template<typename... T>
  void console_print_picture(Console& console, RectI area, ConsoleAlignment alignment, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Simple,
      .action = details::ConsoleAction::Print,
      .alignment = alignment,
      .mode = ConsoleMode::Picture
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    details::raw_console_print_multiline(console, area, params, message);
  }

  template<typename... T>
  int32_t console_print_picture_height(Console& console, RectI area, ConsoleAlignment alignment, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Simple,
      .action = details::ConsoleAction::Count,
      .alignment = alignment,
      .mode = ConsoleMode::Picture
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    return details::raw_console_print_multiline(console, area, params, message);
  }

  /*
   * simple style, text
   */

  template<typename... T>
  void console_print_text(Console& console, Vec2I position, ConsoleAlignment alignment, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Simple,
      .action = details::ConsoleAction::Print,
      .alignment = alignment,
      .mode = ConsoleMode::Text
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    details::raw_console_print_line(console, position, params, message);
  }

  template<typename... T>
  void console_print_text(Console& console, RectI area, ConsoleAlignment alignment, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Simple,
      .action = details::ConsoleAction::Print,
      .alignment = alignment,
      .mode = ConsoleMode::Text
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    details::raw_console_print_multiline(console, area, params, message);
  }

  template<typename... T>
  int32_t console_print_text_height(Console& console, RectI area, ConsoleAlignment alignment, const ConsoleStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Simple,
      .action = details::ConsoleAction::Count,
      .alignment = alignment,
      .mode = ConsoleMode::Picture
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    return details::raw_console_print_multiline(console, area, params, message);
  }

  /*
   * rich style, picture
   */

  template<typename... T>
  void console_print_picture(Console& console, Vec2I position, ConsoleAlignment alignment, const ConsoleRichStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Rich,
      .action = details::ConsoleAction::Print,
      .alignment = alignment,
      .mode = ConsoleMode::Picture
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    details::raw_console_print_line(console, position, params, message);
  }

  template<typename... T>
  void console_print_picture(Console& console, RectI area, ConsoleAlignment alignment, const ConsoleRichStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Rich,
      .action = details::ConsoleAction::Print,
      .alignment = alignment,
      .mode = ConsoleMode::Picture
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    details::raw_console_print_multiline(console, area, params, message);
  }

  template<typename... T>
  int32_t console_print_picture_height(Console& console, RectI area, ConsoleAlignment alignment, const ConsoleRichStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Rich,
      .action = details::ConsoleAction::Count,
      .alignment = alignment,
      .mode = ConsoleMode::Picture
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    return details::raw_console_print_multiline(console, area, params, message);
  }

  /*
   * rich style, text
   */

  template<typename... T>
  void console_print_text(Console& console, Vec2I position, ConsoleAlignment alignment, const ConsoleRichStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Rich,
      .action = details::ConsoleAction::Print,
      .alignment = alignment,
      .mode = ConsoleMode::Text
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    details::raw_console_print_line(console, position, params, message);
  }

  template<typename... T>
  void console_print_text(Console& console, RectI area, ConsoleAlignment alignment, const ConsoleRichStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Rich,
      .action = details::ConsoleAction::Print,
      .alignment = alignment,
      .mode = ConsoleMode::Text
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    details::raw_console_print_multiline(console, area, params, message);
  }

  template<typename... T>
  int32_t console_print_text_height(Console& console, RectI area, ConsoleAlignment alignment, const ConsoleRichStyle& style, fmt::format_string<T...> fmt, T&&... args)
  {
    const details::ConsolePrintParameters params = {
      .style = style,
      .style_choice = details::ConsoleStyleChoice::Rich,
      .action = details::ConsoleAction::Count,
      .alignment = alignment,
      .mode = ConsoleMode::Picture
    };
    const std::string message = fmt::format(fmt, std::forward<T>(args)...);
    return details::raw_console_print_multiline(console, area, params, message);
  }

}

#endif // GF_CONSOLE_OPERATIONS_H
