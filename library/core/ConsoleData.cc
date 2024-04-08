// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/ConsoleData.h>

#include <cassert>

#include <gf2/core/Blit.h>
#include <gf2/core/ConsoleChar.h>
#include <gf2/core/Log.h>
#include <gf2/core/StringUtils.h>

namespace gf {

  namespace {

    struct ConsoleLine {
      std::vector<std::string_view> words;
      int indent = 0;
    };

    struct ConsoleParagraph {
      std::vector<ConsoleLine> lines;
    };

    std::vector<ConsoleParagraph> make_paragraph(const std::string& message, ConsoleAlignment alignment, int paragraph_width)
    {
      auto raw_paragraphs = split_in_paragraphs(message);
      std::vector<ConsoleParagraph> paragraphs;

      for (auto& raw_paragraph : raw_paragraphs) {
        auto raw_words = split_in_words(raw_paragraph);

        ConsoleParagraph paragraph;
        ConsoleLine current_line;
        int current_width = 0;

        for (auto raw_word : raw_words) {
          int word_width = static_cast<int>(raw_word.size());

          if (!current_line.words.empty() && current_width + 1 + word_width > paragraph_width) {
            switch (alignment) {
              case ConsoleAlignment::Left:
                current_line.indent = 0;
                break;

              case ConsoleAlignment::Right:
                current_line.indent = paragraph_width - current_width;
                break;

              case ConsoleAlignment::Center:
                current_line.indent = (paragraph_width - current_width) / 2;
                break;
            }

            paragraph.lines.push_back(std::move(current_line));
            current_line = {};
          }

          if (current_line.words.empty()) {
            current_width = word_width;
          } else {
            current_width += 1 + word_width;
          }

          current_line.words.push_back(raw_word);
        }

        // add the last line

        if (!current_line.words.empty()) {
          switch (alignment) {
            case ConsoleAlignment::Left:
              current_line.indent = 0;
              break;

            case ConsoleAlignment::Right:
              current_line.indent = paragraph_width - current_width;
              break;

            case ConsoleAlignment::Center:
              current_line.indent = (paragraph_width - current_width) / 2;
              break;
          }

          paragraph.lines.push_back(std::move(current_line));
        }

        paragraphs.push_back(std::move(paragraph));
        paragraph = {};
      }

      return paragraphs;
    }

  }

  void ConsoleData::clear(const ConsoleStyle& style)
  {
    for (auto& cell : screen) {
      cell.background = style.background;
      cell.foreground = style.foreground;
      cell.character = u' ';
    }
  }

  void ConsoleData::clear(RectI area, const ConsoleStyle& style)
  {
    auto maybe_area = area.intersection(RectI::from_size(screen.size()));

    if (!maybe_area) {
      return;
    }

    area = *maybe_area;

    for (auto position : position_range(area.extent)) {
      position += area.offset;
      auto& cell = screen(position);
      cell.background = style.background;
      cell.foreground = style.foreground;
      cell.character = u' ';
    }
  }

  void ConsoleData::set_background(Vec2I position, Color color, ConsoleEffect effect)
  {
    if (!screen.valid(position)) {
      return;
    }

    screen(position).background = effect.compute_color(screen(position).background, color);
  }

  Color ConsoleData::background(Vec2I position) const
  {
    assert(screen.valid(position));
    return screen(position).background;
  }

  void ConsoleData::set_foreground(Vec2I position, Color color)
  {
    if (!screen.valid(position)) {
      return;
    }

    screen(position).foreground = color;
  }

  Color ConsoleData::foreground(Vec2I position) const
  {
    assert(screen.valid(position));
    return screen(position).foreground;
  }

  void ConsoleData::set_character(Vec2I position, char16_t character)
  {
    if (!screen.valid(position)) {
      return;
    }

    screen(position).character = character;
  }

  char16_t ConsoleData::character(Vec2I position) const
  {
    assert(screen.valid(position));
    return screen(position).character;
  }

  void ConsoleData::put_character(Vec2I position, char16_t character, const ConsoleStyle& style)
  {
    if (!screen.valid(position)) {
      return;
    }

    auto& cell = screen(position);
    cell.foreground = style.foreground;
    cell.background = style.effect.compute_color(cell.background, style.background);
    cell.character = character;
  }

  void ConsoleData::put_character(Vec2I position, char16_t character, Color foreground, Color background)
  {
    if (!screen.valid(position)) {
      return;
    }

    screen(position) = { foreground, background, character };
  }

  void ConsoleData::draw_rectangle(RectI area, const ConsoleStyle& style)
  {
    for (int x = area.offset.x; x < area.offset.x + area.extent.w; ++x) {
      set_background({ x, area.offset.y }, style.background, style.effect);
      set_background({ x, area.offset.y + area.extent.h - 1 }, style.background, style.effect);
    }

    for (int y = area.offset.y; y < area.offset.y + area.extent.h; ++y) {
      set_background({ area.offset.x, y }, style.background, style.effect);
      set_background({ area.offset.x + area.extent.w - 1, y }, style.background, style.effect);
    }
  }

  void ConsoleData::draw_horizontal_line(Vec2I left, int width, const ConsoleStyle& style)
  {
    for (int i = 0; i < width; ++i) {
      put_character(left, ConsoleChar::BoxDrawingsLightHorizontal, style);
      ++left.x;
    }
  }

  void ConsoleData::draw_vertical_line(Vec2I top, int height, const ConsoleStyle& style)
  {
    for (int j = 0; j < height; ++j) {
      put_character(top, ConsoleChar::BoxDrawingsLightVertical, style);
      ++top.y;
    }
  }

  void ConsoleData::blit_to(ConsoleData& console, RectI source, Vec2I destination, float foreground_alpha, float background_alpha) const
  {
    Blit blit = compute_blit(source, screen.size(), destination, console.screen.size());
    Vec2I offset = {};

    for (offset.y = 0; offset.y < blit.source_region.extent.h; ++offset.y) {
      for (offset.x = 0; offset.x < blit.source_region.extent.w; ++offset.x) {
        assert(console.screen.valid(blit.target_offset + offset));
        assert(screen.valid(blit.source_region.offset + offset));

        auto& target_cell = console.screen(blit.target_offset + offset);
        const auto& origin_cell = screen(blit.source_region.offset + offset);

        target_cell.background = gf::lerp(target_cell.background, origin_cell.background, background_alpha);
        target_cell.foreground = gf::lerp(target_cell.foreground, origin_cell.foreground, foreground_alpha);
        target_cell.character = origin_cell.character;
      }
    }
  }

  int ConsoleData::put_string(Vec2I position, std::string_view message, const ConsoleStyle& style)
  {
    int width = 0;

    for (auto c : gf::codepoints(message)) {
      if (c >= 0x10000 || c < 0x20) {
        // outside BMP or control chars
        c = '\0';
      }

      put_character(position, static_cast<char16_t>(c), style);
      ++position.x;
      ++width;
    }

    return width;
  }

  int ConsoleData::actual_print(RectI area, const std::string& message, const ConsoleStyle& style, uint8_t flags)
  {
    auto size = screen.size();
    auto min = area.min();
    auto max = area.max();

    if (min.x < 0 || min.y < 0 || max.x >= size.w || max.y >= size.h) {
      Log::warning("Position of console text is outside the console.");
    }

    if ((flags & PrintSplit) != 0) {
      // multiline
      return actual_print_multiline(area, message, style, flags);
    }
    // single line
    assert(area.extent == gf::vec(0, 0));
    auto position = area.position();
    int width = static_cast<int>(message.size());

    switch (style.alignment) {
      case ConsoleAlignment::Left:
        break;

      case ConsoleAlignment::Center:
        position.x -= width / 2;
        break;

      case ConsoleAlignment::Right:
        position.x -= width;
        break;
    }

    put_string(position, message, style);
    return 1;
  }

  int ConsoleData::actual_print_multiline(RectI area, const std::string& message, const ConsoleStyle& style, uint8_t flags)
  {
    auto size = screen.size();
    int line_count = 0;
    int paragraph_width = area.extent.w;

    if (paragraph_width == 0) {
      switch (style.alignment) {
        case ConsoleAlignment::Left:
          paragraph_width = size.w - area.offset.x;
          break;

        case ConsoleAlignment::Center:
          paragraph_width = size.w;
          break;

        case ConsoleAlignment::Right:
          paragraph_width = area.offset.x + 1;
          break;
      }
    }

    auto paragraphs = make_paragraph(message, style.alignment, paragraph_width);
    auto position = area.position();

    for (const auto& paragraph : paragraphs) {
      if ((flags & PrintCount) != 0) {
        line_count += static_cast<int>(paragraph.lines.size());
      } else {
        for (const auto& line : paragraph.lines) {
          if (area.offset.y + line_count >= area.offset.y + area.extent.y) {
            break;
          }

          Vec2I line_position = position;
          line_position.x += line.indent;
          auto word_count = line.words.size();

          for (auto word : line.words) {
            line_position.x += put_string(line_position, word, style);
            --word_count;

            if (word_count > 0) {
              put_character(line_position, ' ', style);
              ++line_position.x;
            }
          }

          ++line_count;
          ++position.y;
        }
      }
    }

    return line_count;
  }

  void ConsoleData::actual_draw_frame(RectI area, const ConsoleStyle& style, const std::string& title)
  {
    draw_rectangle(area, style);
    area.extent -= 1;
    auto size = area.extent - 1;
    Vec2I min = area.offset;
    Vec2I max = area.offset + area.extent;

    put_character(area.position_at(Orientation::NorthWest), ConsoleChar::BoxDrawingsLightDownAndRight, style);
    put_character(area.position_at(Orientation::NorthEast), ConsoleChar::BoxDrawingsLightDownAndLeft, style);
    put_character(area.position_at(Orientation::SouthWest), ConsoleChar::BoxDrawingsLightUpAndRight, style);
    put_character(area.position_at(Orientation::SouthEast), ConsoleChar::BoxDrawingsLightUpAndLeft, style);
    draw_horizontal_line({ min.x + 1, min.y }, size.w, style);
    draw_horizontal_line({ min.x + 1, max.y }, size.w, style);
    draw_vertical_line({ min.x, min.y + 1 }, size.h, style);
    draw_vertical_line({ max.x, min.y + 1 }, size.h, style);

    if (title.empty()) {
      return;
    }

    ConsoleStyle title_style = style;
    std::swap(title_style.foreground, title_style.background);
    title_style.effect = ConsoleEffect::set();
    title_style.alignment = ConsoleAlignment::Left;
    print({ min.x + 1, min.y }, title_style, " {} ", title);
  }

}
