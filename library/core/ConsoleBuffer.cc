// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/ConsoleBuffer.h>

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

        for (const auto& part : parts) {
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
      ConsoleTextParser(const std::string& message, int paragraph_width, ConsoleAlignment alignment, ConsoleRichStyle* rich_style)
      : m_message(message)
      , m_paragraph_width(paragraph_width)
      , m_alignment(alignment)
      , m_rich_style(rich_style)
      {
        m_stack.push_back(m_rich_style->default_style().color);
      }

      std::vector<ConsoleParagraph> parser_with(TextLexer& lexer)
      {
        auto raw_paragraphs = compute_raw_paragraphs(lexer);
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
          auto token = lexer.next();

          if (token.type == TextTokenType::StartTag) {
            auto data = std::get<TextTag>(token.data);

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
            auto word = std::get<std::string_view>(token.data);

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

        for (auto& raw_paragraph : raw_paragraphs) {
          ConsoleParagraph paragraph;

          ConsoleLine line;
          int line_width = 0;
          std::size_t word_index = 0;
          std::size_t space_index = 0;

          for (auto item : raw_paragraph.items) {
            switch (item) {
              case ConsoleTextItem::Space:
                {
                  auto& space = raw_paragraph.spaces[space_index];
                  line.spaces.push_back(space);
                  line.items.push_back(ConsoleTextItem::Space);
                  ++line_width;
                  ++space_index;
                }
                break;

              case ConsoleTextItem::Word:
                {
                  auto& word = raw_paragraph.words[word_index];
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

      const std::string& m_message; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
      int m_paragraph_width = 0;
      ConsoleAlignment m_alignment = ConsoleAlignment::Left;
      ConsoleRichStyle* m_rich_style;
      std::vector<ConsoleColorStyle> m_stack;
    };

  }

  void ConsoleBuffer::clear(const ConsoleStyle& style)
  {
    for (auto& cell : cells) {
      cell.background = style.color.background;
      cell.foreground = style.color.foreground;
      cell.character = u' ';
    }
  }

  void ConsoleBuffer::clear(RectI area, const ConsoleStyle& style)
  {
    auto maybe_area = area.intersection(RectI::from_size(cells.size()));

    if (!maybe_area) {
      return;
    }

    area = *maybe_area;

    for (auto position : position_range(area.extent)) {
      position += area.offset;
      auto& cell = cells(position);
      cell.background = style.color.background;
      cell.foreground = style.color.foreground;
      cell.character = u' ';
    }
  }

  void ConsoleBuffer::set_background(Vec2I position, Color color, ConsoleEffect effect)
  {
    if (!cells.valid(position)) {
      return;
    }

    cells(position).background = effect.compute_color(cells(position).background, color);
  }

  Color ConsoleBuffer::background(Vec2I position) const
  {
    assert(cells.valid(position));
    return cells(position).background;
  }

  void ConsoleBuffer::set_foreground(Vec2I position, Color color)
  {
    if (!cells.valid(position)) {
      return;
    }

    cells(position).foreground = color;
  }

  Color ConsoleBuffer::foreground(Vec2I position) const
  {
    assert(cells.valid(position));
    return cells(position).foreground;
  }

  void ConsoleBuffer::set_character(Vec2I position, char16_t character)
  {
    if (!cells.valid(position)) {
      return;
    }

    cells(position).character = character;
  }

  char16_t ConsoleBuffer::character(Vec2I position) const
  {
    assert(cells.valid(position));
    return cells(position).character;
  }

  void ConsoleBuffer::put_character(Vec2I position, char16_t character, const ConsoleStyle& style)
  {
    if (!cells.valid(position)) {
      return;
    }

    auto& cell = cells(position);
    cell.foreground = style.color.foreground;
    cell.background = style.effect.compute_color(cell.background, style.color.background);
    cell.character = character;
  }

  void ConsoleBuffer::put_character(Vec2I position, char16_t character, Color foreground, Color background)
  {
    if (!cells.valid(position)) {
      return;
    }

    cells(position) = { foreground, background, character };
  }

  void ConsoleBuffer::draw_rectangle(RectI area, const ConsoleStyle& style)
  {
    for (int x = area.offset.x; x < area.offset.x + area.extent.w; ++x) {
      set_background({ x, area.offset.y }, style.color.background, style.effect);
      set_background({ x, area.offset.y + area.extent.h - 1 }, style.color.background, style.effect);
    }

    for (int y = area.offset.y; y < area.offset.y + area.extent.h; ++y) {
      set_background({ area.offset.x, y }, style.color.background, style.effect);
      set_background({ area.offset.x + area.extent.w - 1, y }, style.color.background, style.effect);
    }
  }

  void ConsoleBuffer::draw_horizontal_line(Vec2I left, int width, const ConsoleStyle& style)
  {
    for (int i = 0; i < width; ++i) {
      put_character(left, ConsoleChar::BoxDrawingsLightHorizontal, style);
      ++left.x;
    }
  }

  void ConsoleBuffer::draw_vertical_line(Vec2I top, int height, const ConsoleStyle& style)
  {
    for (int j = 0; j < height; ++j) {
      put_character(top, ConsoleChar::BoxDrawingsLightVertical, style);
      ++top.y;
    }
  }

  void ConsoleBuffer::blit_to(ConsoleBuffer& console, RectI source, Vec2I destination, float foreground_alpha, float background_alpha) const
  {
    const Blit blit = compute_blit(source, cells.size(), destination, console.cells.size());
    Vec2I offset = {};

    for (offset.y = 0; offset.y < blit.source_region.extent.h; ++offset.y) {
      for (offset.x = 0; offset.x < blit.source_region.extent.w; ++offset.x) {
        assert(console.cells.valid(blit.target_offset + offset));
        assert(cells.valid(blit.source_region.offset + offset));

        auto& target_cell = console.cells(blit.target_offset + offset);
        const auto& origin_cell = cells(blit.source_region.offset + offset);

        target_cell.background = gf::lerp(target_cell.background, origin_cell.background, background_alpha);
        target_cell.foreground = gf::lerp(target_cell.foreground, origin_cell.foreground, foreground_alpha);
        target_cell.character = origin_cell.character;
      }
    }
  }

  int ConsoleBuffer::put_string(Vec2I position, std::string_view message, const ConsoleStyle& style)
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

  int ConsoleBuffer::raw_print(RectI area, const std::string& message, ConsoleAlignment alignment, const ConsoleRichStyle& style, uint8_t flags)
  {
    auto size = cells.size();
    auto min = area.min();
    auto max = area.max();

    if (min.x < 0 || min.y < 0 || max.x > size.w || max.y > size.h) {
      Log::warning("Position of console text is outside the console.");
    }

    if ((flags & PrintSplit) != 0) {
      // multiline
      return raw_print_multiline(area, message, alignment, style, flags);
    }
    // single line
    assert(area.extent == gf::vec(0, 0));

    auto position = area.position();

    RectI single_line_area = {};

    switch (alignment) {
      case ConsoleAlignment::Left:
        single_line_area.offset = position;
        single_line_area.extent = { size.w - position.x, 1 };
        break;

      case ConsoleAlignment::Center:
        {
          const int half_width = std::min(position.x, size.w - position.x - 1);
          single_line_area.offset = { position.x - half_width, position.y };
          single_line_area.extent = { 2 * half_width + 1, 1 };
        }
        break;

      case ConsoleAlignment::Right:
        single_line_area.offset = { 0, position.y };
        single_line_area.extent = { position.x + 1, 1 };
        break;
    }

    return raw_print_multiline(single_line_area, message, alignment, style, flags);
  }

  int ConsoleBuffer::raw_print_multiline(RectI area, const std::string& message, ConsoleAlignment alignment, const ConsoleRichStyle& style, uint8_t flags)
  {
    auto size = cells.size();
    int line_count = 0;
    int paragraph_width = area.extent.w;

    if (paragraph_width == 0) {
      switch (alignment) {
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

    ConsoleRichStyle rich_style(style);
    ConsoleTextParser parser(message, paragraph_width, alignment, &rich_style);

    std::vector<ConsoleParagraph> paragraphs;

    if ((flags & PrintSimple) != 0) {
      SimpleTextLexer lexer;
      paragraphs = parser.parser_with(lexer);
    } else {
      RichTextLexer lexer;
      paragraphs = parser.parser_with(lexer);
    }

    auto position = area.position();
    ConsoleStyle current_style = style.default_style();

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
          std::size_t word_index = 0;
          std::size_t space_index = 0;

          for (auto item : line.items) {
            switch (item) {
              case ConsoleTextItem::Space:
                {
                  const auto& space = line.spaces[space_index];
                  current_style.color = space;
                  put_character(line_position, ' ', current_style);
                  ++line_position.x;
                  ++space_index;
                }
                break;

              case ConsoleTextItem::Word:
                {
                  const auto& word = line.words[word_index];

                  for (const auto& parts : word.parts) {
                    current_style.color = parts.style;
                    line_position.x += put_string(line_position, parts.data, current_style);
                  }

                  ++word_index;
                }
                break;
            }
          }

          ++line_count;
          ++position.y;
        }
      }
    }

    return line_count;
  }

  void ConsoleBuffer::raw_draw_frame(RectI area, const ConsoleStyle& style, std::string_view title)
  {
    draw_rectangle(area, style);
    area.extent -= 1;
    auto size = area.extent - 1;
    const Vec2I min = area.offset;
    const Vec2I max = area.offset + area.extent;

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
    std::swap(title_style.color.foreground, title_style.color.background);
    title_style.effect = ConsoleEffect::set();
    print({ min.x + 1, min.y }, ConsoleAlignment::Left, title_style, " {} ", title);
  }

}
