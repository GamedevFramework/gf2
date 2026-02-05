// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ConsoleOperations.h>

#include <cassert>

#include <gf2/core/Blit.h>
#include <gf2/core/ConsoleChar.h>
#include <gf2/core/ConsoleStyle.h>
#include <gf2/core/Log.h>
#include <gf2/core/StringUtils.h>
#include <gf2/core/TextLexer.h>

namespace gf {

  namespace {
    struct ConsoleWordPart {
      std::string_view data;
      ConsoleColorStyle style;
    };

    struct ConsoleWord {
      std::vector<ConsoleWordPart> parts;

      int width() const
      {
        std::size_t length = 0;

        for (const ConsoleWordPart& part : parts) {
          length += part.data.size();
        }

        return static_cast<int>(length);
      }
    };

    enum class ConsoleTextItem : uint8_t {
      Word,
      Space,
    };

    struct ConsoleLine {
      std::vector<ConsoleTextItem> items;
      std::vector<ConsoleWord> words;
      std::vector<ConsoleColorStyle> spaces;
      int indent = 0;
    };

    struct ConsoleParagraph {
      std::vector<ConsoleLine> lines;
    };

    class ConsoleTextParser {
    public:
      ConsoleTextParser(std::string_view message, int paragraph_width, ConsoleAlignment alignment, const ConsoleRichStyle* rich_style)
      : m_message(message)
      , m_paragraph_width(paragraph_width)
      , m_alignment(alignment)
      , m_rich_style(rich_style)
      {
        m_stack.push_back(m_rich_style->default_style().color);
      }

      std::vector<ConsoleParagraph> parser_with(TextLexer& lexer)
      {
        std::vector<ConsoleLine> raw_paragraphs = compute_raw_paragraphs(lexer);
        return compute_paragraphs(raw_paragraphs);
      }

    private:
      std::vector<ConsoleLine> compute_raw_paragraphs(TextLexer& lexer)
      {
        assert(m_stack.size() == 1);
        lexer.initialize(m_message);

        std::vector<ConsoleLine> paragraphs;
        ConsoleLine current_line;
        ConsoleWord current_word;

        for (;;) {
          const TextToken token = lexer.next();

          if (token.type == TextTokenType::StartTag) {
            const TextTag data = std::get<TextTag>(token.data);

            if (data.tag != "style") {
              Log::fatal("Unknown tag: {}.", data.tag);
            }

            m_stack.push_back(m_rich_style->style(data.value));
            continue;
          }

          if (token.type == TextTokenType::EndTag) {
            if (m_stack.size() == 1 || m_stack.empty()) {
              Log::fatal("Too many end tags.");
            }

            m_stack.pop_back();
            continue;
          }

          if (token.type == TextTokenType::Word) {
            const std::string_view word = std::get<std::string_view>(token.data);

            assert(!m_stack.empty());
            current_word.parts.push_back({ word, m_stack.back() });
            continue;
          }

          // space, newline or end

          if (!current_word.parts.empty()) {
            current_line.items.push_back(ConsoleTextItem::Word);
            current_line.words.push_back(std::move(current_word));
            current_word = {};
          }

          if (token.type == TextTokenType::Space) {
            current_line.items.push_back(ConsoleTextItem::Space);
            current_line.spaces.push_back(m_stack.back());
            continue;
          }

          if (!current_line.items.empty()) {
            paragraphs.push_back(std::move(current_line));
            current_line = {};
          }

          if (token.type == TextTokenType::Newline) {
            continue;
          }

          assert(token.type == TextTokenType::End);
          break;
        }

        if (m_stack.size() != 1) {
          Log::fatal("Mismatched tags.");
        }

        return paragraphs;
      }

      std::vector<ConsoleParagraph> compute_paragraphs(std::vector<ConsoleLine>& raw_paragraphs) const
      {
        std::vector<ConsoleParagraph> paragraphs;

        for (ConsoleLine& raw_paragraph : raw_paragraphs) {
          ConsoleParagraph paragraph;

          ConsoleLine line;
          int line_width = 0;
          std::size_t word_index = 0;
          std::size_t space_index = 0;

          for (ConsoleTextItem item : raw_paragraph.items) {
            switch (item) {
              case ConsoleTextItem::Space:
                {
                  assert(space_index < raw_paragraph.spaces.size());
                  const ConsoleColorStyle& space = raw_paragraph.spaces[space_index];
                  line.spaces.push_back(space);
                  line.items.push_back(ConsoleTextItem::Space);
                  ++line_width;
                  ++space_index;
                }
                break;

              case ConsoleTextItem::Word:
                {
                  assert(word_index < raw_paragraph.words.size());
                  ConsoleWord& word = raw_paragraph.words[word_index];
                  const int word_width = word.width();

                  if (!line.words.empty() && line_width + word_width > m_paragraph_width) {
                    // remove trailing spaces

                    while (line.items.back() == ConsoleTextItem::Space) {
                      --line_width;
                      line.items.pop_back();
                      line.spaces.pop_back();
                    }

                    // compute indent

                    switch (m_alignment) {
                      case ConsoleAlignment::Left:
                        line.indent = 0;
                        break;

                      case ConsoleAlignment::Right:
                        line.indent = m_paragraph_width - line_width;
                        break;

                      case ConsoleAlignment::Center:
                        line.indent = (m_paragraph_width - line_width) / 2;
                        break;
                    }

                    paragraph.lines.push_back(std::move(line));
                    line = {};
                    line_width = 0;
                  }

                  line_width += word_width;
                  line.words.push_back(std::move(word));
                  line.items.push_back(ConsoleTextItem::Word);

                  ++word_index;
                }
                break;
            }
          }

          // add the last line

          if (!line.words.empty()) {
            switch (m_alignment) {
              case ConsoleAlignment::Left:
                line.indent = 0;
                break;
              case ConsoleAlignment::Right:
                line.indent = m_paragraph_width - line_width;
                break;
              case ConsoleAlignment::Center:
                line.indent = (m_paragraph_width - line_width) / 2;
                break;
            }

            paragraph.lines.push_back(std::move(line));
          }

          paragraphs.push_back(std::move(paragraph));
        }

        return paragraphs;
      }

      std::string_view m_message;
      int m_paragraph_width = 0;
      ConsoleAlignment m_alignment = ConsoleAlignment::Left;
      const ConsoleRichStyle* m_rich_style = nullptr;
      std::vector<ConsoleColorStyle> m_stack;
    };

  }

  void console_clear(Console& console, const ConsoleStyle& style)
  {
    for (ConsoleCell& cell : console) {
      // do not change the mode of the cell
      cell.parts[0].background = cell.parts[1].background = style.color.background;
      cell.parts[0].foreground = cell.parts[1].foreground = style.color.foreground;
      cell.parts[0].character = cell.parts[1].character = u' ';
    }
  }

  void console_clear(Console& console, RectI area, const ConsoleStyle& style)
  {
    std::optional<RectI> maybe_area = area.intersection(RectI::from_size(console.size()));

    if (!maybe_area) {
      return;
    }

    area = maybe_area.value();

    for (Vec2I position : position_range(area.extent)) {
      position += area.offset;
      ConsoleCell& cell = console(position);
      // do not change the mode of the cell
      cell.parts[0].background = cell.parts[1].background = style.color.background;
      cell.parts[0].foreground = cell.parts[1].foreground = style.color.foreground;
      cell.parts[0].character = cell.parts[1].character = u' ';
    }
  }

  void console_write_background(Console& console, Vec2I position, Color color, ConsoleEffect effect)
  {
    if (!console.valid(position)) {
      return;
    }

    ConsoleCell& cell = console(position);
    // do not change the mode of the cell
    cell.parts[0].background = effect.compute_color(cell.parts[0].background, color);
    cell.parts[1].background = effect.compute_color(cell.parts[1].background, color);
  }

  void console_write_picture(Console& console, Vec2I position, char16_t character, const ConsoleStyle& style)
  {
    if (!console.valid(position)) {
      return;
    }

    ConsoleCell& cell = console(position);
    cell.mode = ConsoleMode::Picture;
    cell.parts[0].foreground = style.color.foreground;
    cell.parts[0].background = style.effect.compute_color(cell.parts[0].background, style.color.background);
    cell.parts[0].character = character;
  }

  int32_t console_write_picture(Console& console, Vec2I position, std::string_view message, const ConsoleStyle& style)
  {
    int32_t width = 0;

    for (char32_t c : gf::codepoints(message)) {
      if (c >= 0x10000 || c < 0x20) {
        // outside BMP or control chars
        c = '\0';
      }

      console_write_picture(console, position, static_cast<char16_t>(c), style);
      ++position.x;
      ++width;
    }

    return width;
  }

  void console_write_text(Console& console, Vec2I position, uint8_t part_index, char16_t character, const ConsoleStyle& style)
  {
    if (!console.valid(position)) {
      return;
    }

    ConsoleCell& cell = console(position);
    cell.mode = ConsoleMode::Text;
    cell.parts[part_index].foreground = style.color.foreground;
    cell.parts[part_index].background = style.effect.compute_color(cell.parts[part_index].background, style.color.background);
    cell.parts[part_index].character = character;
  }

  int32_t console_write_text(Console& console, Vec2I position, std::string_view message, const ConsoleStyle& style)
  {
    int32_t width = 0;
    uint8_t part_index = 0;

    for (char32_t c : gf::codepoints(message)) {
      if (c >= 0x10000 || c < 0x20) {
        // outside BMP or control chars
        c = '\0';
      }

      if (!console.valid(position)) {
        break;
      }

      console_write_text(console, position, part_index, static_cast<char16_t>(c), style);
      position.x += part_index;
      width += part_index;
      part_index = 1 - part_index; // toggle between 0 and 1
    }

    if (part_index == 1) {
      // the second part of the cell has not been written
      console_write_text(console, position, 1, u' ', style);
      ++width;
    }

    return width;
  }

  void console_draw_rectangle(Console& console, RectI area, const ConsoleStyle& style)
  {
    for (int x = area.offset.x; x < area.offset.x + area.extent.w; ++x) {
      console_write_background(console, { x, area.offset.y }, style.color.background, style.effect);
      console_write_background(console, { x, area.offset.y + area.extent.h - 1 }, style.color.background, style.effect);
    }

    for (int y = area.offset.y; y < area.offset.y + area.extent.h; ++y) {
      console_write_background(console, { area.offset.x, y }, style.color.background, style.effect);
      console_write_background(console, { area.offset.x + area.extent.w - 1, y }, style.color.background, style.effect);
    }
  }

  void console_draw_horizontal_line(Console& console, Vec2I left, int width, const ConsoleStyle& style)
  {
    for (int i = 0; i < width; ++i) {
      console_write_picture(console, left, ConsoleChar::BoxDrawingsLightHorizontal, style);
      ++left.x;
    }
  }

  void console_draw_vertical_line(Console& console, Vec2I top, int height, const ConsoleStyle& style)
  {
    for (int j = 0; j < height; ++j) {
      console_write_picture(console, top, ConsoleChar::BoxDrawingsLightVertical, style);
      ++top.y;
    }
  }

  namespace details {

    void raw_draw_frame(Console& console, RectI area, const ConsoleStyle& style, ConsoleMode mode, std::string_view title)
    {
      console_draw_rectangle(console, area, style);
      area.extent -= 1;
      const Vec2I size = area.extent - 1;
      const Vec2I min = area.offset;
      const Vec2I max = area.offset + area.extent;

      console_write_picture(console, area.position_at(Orientation::NorthWest), ConsoleChar::BoxDrawingsLightDownAndRight, style);
      console_write_picture(console, area.position_at(Orientation::NorthEast), ConsoleChar::BoxDrawingsLightDownAndLeft, style);
      console_write_picture(console, area.position_at(Orientation::SouthWest), ConsoleChar::BoxDrawingsLightUpAndRight, style);
      console_write_picture(console, area.position_at(Orientation::SouthEast), ConsoleChar::BoxDrawingsLightUpAndLeft, style);
      console_draw_horizontal_line(console, { min.x + 1, min.y }, size.w, style);
      console_draw_horizontal_line(console, { min.x + 1, max.y }, size.w, style);
      console_draw_vertical_line(console, { min.x, min.y + 1 }, size.h, style);
      console_draw_vertical_line(console, { max.x, min.y + 1 }, size.h, style);

      if (title.empty()) {
        return;
      }

      ConsoleStyle title_style = style;
      std::swap(title_style.color.foreground, title_style.color.background);
      title_style.effect = ConsoleEffect::set();
      const Vec2I title_position = { min.x + 1, min.y };

      switch (mode) {
        case ConsoleMode::Picture:
          console_print_picture(console, title_position, ConsoleAlignment::Left, title_style, " {} ", title);
          break;
        case ConsoleMode::Text:
          console_print_text(console, title_position, ConsoleAlignment::Left, title_style, " {} ", title);
          break;
      }
    }

  }


  void console_blit_to(const Console& origin_console, Console& target_console, RectI origin, Vec2I target, float foreground_alpha, float background_alpha)
  {
    const Blit blit = compute_blit(origin, origin_console.size(), target, target_console.size());
    Vec2I offset = {};

    for (offset.y = 0; offset.y < blit.origin_region.extent.h; ++offset.y) {
      for (offset.x = 0; offset.x < blit.origin_region.extent.w; ++offset.x) {
        assert(target_console.valid(blit.target_offset + offset));
        assert(origin_console.valid(blit.origin_region.offset + offset));

        ConsoleCell& target_cell = target_console(blit.target_offset + offset);
        const ConsoleCell& origin_cell = origin_console(blit.origin_region.offset + offset);

        target_cell.mode = origin_cell.mode;
        target_cell.parts[0].background = gf::lerp(target_cell.parts[0].background, origin_cell.parts[0].background, background_alpha);
        target_cell.parts[0].foreground = gf::lerp(target_cell.parts[0].foreground, origin_cell.parts[0].foreground, foreground_alpha);
        target_cell.parts[0].character = origin_cell.parts[0].character;
        target_cell.parts[1].background = gf::lerp(target_cell.parts[1].background, origin_cell.parts[1].background, background_alpha);
        target_cell.parts[1].foreground = gf::lerp(target_cell.parts[1].foreground, origin_cell.parts[1].foreground, foreground_alpha);
        target_cell.parts[1].character = origin_cell.parts[1].character;
      }
    }
  }

  namespace details {

    int32_t raw_console_print_line(Console& console, Vec2I position, const ConsolePrintParameters& params, std::string_view message)
    {
      const Vec2I size = console.size();
      RectI single_line_area = {};

      switch (params.alignment) {
        case ConsoleAlignment::Left:
          single_line_area.offset = position;
          single_line_area.extent = { size.w - position.x, 1 };
          break;

        case ConsoleAlignment::Center:
          {
            const int half_width = std::min(position.x, size.w - position.x - 1);
            single_line_area.offset = { position.x - half_width, position.y };
            single_line_area.extent = { (2 * half_width) + 1, 1 };
          }
          break;

        case ConsoleAlignment::Right:
          single_line_area.offset = { 0, position.y };
          single_line_area.extent = { position.x + 1, 1 };
          break;
      }

      return raw_console_print_multiline(console, single_line_area, params, message);
    }

    namespace {

      void raw_console_print_line_picture(Console& console, Vec2I position, const ConsoleLine& line, ConsoleStyle& current_style)
      {
        Vec2I line_position = position;
        line_position.x += line.indent;
        std::size_t word_index = 0;
        std::size_t space_index = 0;

        for (ConsoleTextItem item : line.items) {
          switch (item) {
            case ConsoleTextItem::Space:
              {
                assert(space_index < line.spaces.size());
                const ConsoleColorStyle& space = line.spaces[space_index];
                current_style.color = space;

                console_write_picture(console, line_position, ' ', current_style);
                ++line_position.x;

                ++space_index;
              }
              break;

            case ConsoleTextItem::Word:
              {
                assert(word_index < line.words.size());
                const ConsoleWord& word = line.words[word_index];

                for (const ConsoleWordPart& parts : word.parts) {
                  current_style.color = parts.style;
                  line_position.x += console_write_picture(console, line_position, parts.data, current_style);
                }

                ++word_index;
              }
              break;
          }
        }
      }

      void raw_console_print_line_text(Console& console, Vec2I position, const ConsoleLine& line, ConsoleStyle& current_style)
      {
        position.x += line.indent / 2;
        std::size_t word_index = 0;
        std::size_t space_index = 0;
        uint8_t part_index = line.indent % 2;

        for (ConsoleTextItem item : line.items) {
          switch (item) {
            case ConsoleTextItem::Space:
              {
                assert(space_index < line.spaces.size());
                const ConsoleColorStyle& space = line.spaces[space_index];
                current_style.color = space;

                console_write_text(console, position, part_index, ' ', current_style);
                position.x += part_index;
                part_index = 1 - part_index;

                ++space_index;
              }
              break;

            case ConsoleTextItem::Word:
              {
                assert(word_index < line.words.size());
                const ConsoleWord& word = line.words[word_index];

                for (const ConsoleWordPart& parts : word.parts) {
                  current_style.color = parts.style;

                  for (char32_t character : gf::codepoints(parts.data)) {
                    if (character >= 0x10000 || character < 0x20) {
                      // outside BMP or control chars
                      character = '\0';
                    }

                    console_write_text(console, position, part_index, static_cast<char16_t>(character), current_style);
                    position.x += part_index;
                    part_index = 1 - part_index;
                  }
                }

                ++word_index;
              }
              break;
          }
        }
      }

      std::vector<ConsoleParagraph> raw_compute_paragraphs(Vec2I size, RectI area, const ConsolePrintParameters& params, std::string_view message)
      {
        const Vec2I min = area.min();
        const Vec2I max = area.max();

        if (min.x < 0 || min.y < 0 || max.x > size.w || max.y > size.h) {
          Log::warning("Position of console text is outside the console.");
        }

        int32_t paragraph_width = area.extent.w;

        if (paragraph_width == 0) {
          switch (params.alignment) {
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

        if (params.mode == ConsoleMode::Text) {
          paragraph_width *= 2;
        }

        ConsoleTextParser parser(message, paragraph_width, params.alignment, &params.style);

        std::vector<ConsoleParagraph> paragraphs;

        if (params.style_choice == ConsoleStyleChoice::Simple) {
          SimpleTextLexer lexer;
          paragraphs = parser.parser_with(lexer);
        } else {
          RichTextLexer lexer;
          paragraphs = parser.parser_with(lexer);
        }

        return paragraphs;
      }

    }

    int32_t raw_console_print_multiline(Console& console, RectI area, const ConsolePrintParameters& params, std::string_view message)
    {
      std::vector<ConsoleParagraph> paragraphs = raw_compute_paragraphs(console.size(), area, params, message);

      int32_t line_count = 0;

      Vec2I position = area.position();
      ConsoleStyle current_style = params.style.default_style();

      for (const ConsoleParagraph& paragraph : paragraphs) {
        if (params.action == ConsoleAction::Count) {
          line_count += static_cast<int>(paragraph.lines.size());
        } else {
          for (const ConsoleLine& line : paragraph.lines) {
            if (area.offset.y + line_count >= area.offset.y + area.extent.y) {
              break;
            }

            if (params.mode == ConsoleMode::Picture) {
              raw_console_print_line_picture(console, position, line, current_style);
            } else {
              raw_console_print_line_text(console, position, line, current_style);
            }

            ++line_count;
            ++position.y;
          }
        }
      }

      return line_count;
    }

  }

}
