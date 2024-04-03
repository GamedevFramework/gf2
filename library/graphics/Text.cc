// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/Text.h>

#include <cassert>
#include <cstdint>

#include <charconv>
#include <string_view>
#include <variant>
#include <vector>

#include <hb-ft.h>
#include <hb.h>

#include <gf2/core/Flags.h>
#include <gf2/core/FontManager.h>
#include <gf2/core/Log.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/StringUtils.h>

#include <gf2/graphics/RawGeometry.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  namespace {

    enum class FontStyle : uint8_t {
      Bold = 0x01,
      Italic = 0x02,
    };

  }

  template<>
  struct EnableBitmaskOperators<FontStyle> : std::true_type {
  };

  namespace {
    using namespace operators;

    constexpr uint8_t style_value(Flags<FontStyle> style)
    {
      return style.value();
    }

    float position_scale(hb_position_t value)
    {
      return static_cast<float>(value) / 64.0f;
    }

    template<typename T, void (*Destroy)(T*), T* (*Reference)(T*)>
    class HbHandle {
    public:
      HbHandle() = default;

      HbHandle(T* handle)
      : m_handle(handle)
      {
      }

      HbHandle(const HbHandle& other)
      : m_handle(other.m_handle)
      {
        Reference(m_handle);
      }

      HbHandle(HbHandle&& other) noexcept
      : m_handle(std::exchange(other.m_handle, nullptr))
      {
      }

      ~HbHandle()
      {
        if (m_handle != nullptr) {
          Destroy(m_handle);
        }
      }

      HbHandle& operator=(const HbHandle& other)
      {
        if (this != &other) {
          Destroy(m_handle);
          m_handle = other.m_handle;
          Reference(m_handle);
        }

        return *this;
      }

      HbHandle& operator=(HbHandle&& other) noexcept
      {
        std::swap(m_handle, other.m_handle);
        return *this;
      }

      operator T*()
      {
        return m_handle;
      }

      operator const T *() const
      {
        return m_handle;
      }

    private:
      T* m_handle = nullptr;
    };

    using HbFont = HbHandle<hb_font_t, hb_font_destroy, hb_font_reference>;
    using HbBuffer = HbHandle<hb_buffer_t, hb_buffer_destroy, hb_buffer_reference>;

    struct FontData {
      FontData() = default;

      FontData(FontFace* face)
      : face(face)
      , font(hb_ft_font_create(static_cast<FT_Face>(face->raw_handle()), nullptr))
      {
      }

      FontFace* face = nullptr;
      HbFont font;
    };

    struct FontProperties {
      FontData data;
      float character_size_factor = 0.0f;
      Color color = Black;
      Flags<FontStyle> style = None;
    };

    class FontPropertiesStack {
    public:
      FontPropertiesStack(FontFace* default_font, const TextData& data)
      : m_default(default_font)
      {
        m_stack.push_back({ m_default, FontFace::size_factor(data.character_size), srgb_to_linear(data.color) });
      }

      FontPropertiesStack(FontFace* default_font, FontFace* bold_font, FontFace* italic_font, FontFace* bold_italic_font, const TextData& data)
      : m_default(default_font)
      , m_bold(bold_font)
      , m_italic(italic_font)
      , m_bold_italic(bold_italic_font)
      {
        m_stack.push_back({ m_default, FontFace::size_factor(data.character_size), data.color });
      }

      void set_italic()
      {
        assert(!m_stack.empty());
        FontProperties properties = m_stack.back();
        properties.style |= FontStyle::Italic;
        properties.data = font_from_style(properties.style);
        m_stack.push_back(properties);
      }

      void set_bold()
      {
        assert(!m_stack.empty());
        FontProperties properties = m_stack.back();
        properties.style |= FontStyle::Bold;
        properties.data = font_from_style(properties.style);
        m_stack.push_back(properties);
      }

      void set_size_factor(float factor)
      {
        assert(!m_stack.empty());
        FontProperties properties = m_stack.back();
        properties.character_size_factor *= factor;
        m_stack.push_back(properties);
      }

      void set_color(Color color)
      {
        assert(!m_stack.empty());
        FontProperties properties = m_stack.back();
        properties.color = srgb_to_linear(color);
        m_stack.push_back(properties);
      }

      void pop()
      {
        m_stack.pop_back();
      }

      const FontProperties& top() const
      {
        return m_stack.back();
      }

      bool ready() const
      {
        return m_stack.size() == 1;
      }

      bool empty() const
      {
        return m_stack.empty();
      }

    private:
      FontData font_from_style(Flags<FontStyle> style)
      {
        FontData data;

        switch (style.value()) {
          case style_value(None):
            data = m_default;
            break;
          case style_value(FontStyle::Bold):
            data = m_bold;
            break;
          case style_value(FontStyle::Italic):
            data = m_italic;
            break;
          case style_value(FontStyle::Bold | FontStyle::Italic):
            data = m_bold_italic;
            break;
        }

        if (data.face == nullptr) {
          data = m_default;
        }

        assert(data.face != nullptr);
        return data;
      }

      FontData m_default;
      FontData m_bold;
      FontData m_italic;
      FontData m_bold_italic;

      std::vector<FontProperties> m_stack;
    };

    enum class TextTokenType : uint8_t {
      Word,
      Space,
      Newline,
      StartTag,
      EndTag,
      End,
    };

    struct TextTag {
      std::string_view tag;
      std::string_view value;
    };

    struct TextToken {
      static TextToken make_word(std::string_view word)
      {
        return { TextTokenType::Word, word };
      }

      static TextToken make_space()
      {
        return { TextTokenType::Space, std::monostate() };
      }

      static TextToken make_newline()
      {
        return { TextTokenType::Newline, std::monostate() };
      }

      static TextToken make_start_tag(std::string_view tag, std::string_view value)
      {
        TextTag data = { tag, value };
        return { TextTokenType::StartTag, data };
      }

      static TextToken make_end_tag()
      {
        return { TextTokenType::EndTag, std::monostate() };
      }

      static TextToken make_end()
      {
        return { TextTokenType::End, std::monostate() };
      }

      TextTokenType type;
      std::variant<std::monostate, std::string_view, TextTag> data;
    };

    class TextLexer {
    public:
      TextLexer() = default;
      TextLexer(const TextLexer&) = default;
      TextLexer(TextLexer&&) noexcept = default;
      virtual ~TextLexer() = default;

      TextLexer& operator=(const TextLexer&) = default;
      TextLexer& operator=(TextLexer&&) noexcept = default;

      void initialize(std::string_view string)
      {
        m_source = string;
        m_current = string.data();
      }

      virtual TextToken next() = 0;

    protected:

      bool at_end() const
      {
        return m_current == m_source.data() + m_source.size();
      }

      char peek() const
      {
        return *m_current;
      }

      char advance()
      {
        return *m_current++;
      }

      bool match(char expected)
      {
        if (at_end()) {
          return false;
        }

        if (*m_current != expected) {
          return false;
        }

        ++m_current;
        return true;
      }

      std::string_view source() const
      {
        return m_source;
      }

      const char* current() const
      {
        return m_current;
      }

      std::size_t current_index() const
      {
        return m_current - m_source.data();
      }

      static bool is_alpha(char c) { return 'a' <= c && c <= 'z'; }
      static bool is_space(char c) { return c == ' ' || c == '\t'; }

    private:
      std::string_view m_source;
      const char* m_current = nullptr;
    };

    class SimpleTextLexer : public TextLexer {
    public:
      TextToken next() override
      {
        if (at_end()) {
          return TextToken::make_end();
        }

        if (match('\n')) {
          return TextToken::make_newline();
        }

        if (is_space(peek())) {
          while (!at_end() && is_space(peek())) {
            advance();
          }

          return TextToken::make_space();
        }

        const char* word_start = current();

        while (!at_end() && !is_space(peek()) && peek() != '\n' && peek() != '<') {
          advance();
        }

        std::string_view word(word_start, static_cast<std::size_t>(current() - word_start));
        return TextToken::make_word(word);
      }

    private:
    };

    class RichTextLexer : public TextLexer {
    public:
      TextToken next() override
      {
        if (at_end()) {
          return TextToken::make_end();
        }

        if (match('<')) {
          // is it end tag?
          if (match('/')) {
            if (!match('>')) {
              Log::fatal("Wrong end tag in string '{}' at {}.", source(), current_index());
            }

            return TextToken::make_end_tag();
          }

          const char* tag_start = current();

          while (!at_end() && is_alpha(peek())) {
            advance();
          }

          std::string_view tag(tag_start, static_cast<std::size_t>(current() - tag_start));
          std::string_view value;

          if (match('=')) {
            const char* value_start = current();

            while (!at_end() && peek() != '>') {
              advance();
            }

            value = std::string_view(value_start, static_cast<std::size_t>(current() - value_start));
          }

          if (!match('>')) {
            Log::fatal("Unclosed tag in string '{}' at index {}.", source(), current_index());
          }

          return TextToken::make_start_tag(tag, value);
        }

        if (match('\n')) {
          return TextToken::make_newline();
        }

        if (is_space(peek())) {
          while (!at_end() && is_space(peek())) {
            advance();
          }

          return TextToken::make_space();
        }

        const char* word_start = current();

        while (!at_end() && !is_space(peek()) && peek() != '\n' && peek() != '<') {
          advance();
        }

        std::string_view word(word_start, static_cast<std::size_t>(current() - word_start));
        return TextToken::make_word(word);
      }
    };

    struct TextGlyph {
      uint32_t index;
      Vec2I advance;
      Vec2I offset;
    };

    struct TextWordPart {
      std::string_view data;
      FontProperties properties;
      std::vector<TextGlyph> glyphs;
    };

    struct TextWord {
      std::string full;
      std::vector<TextWordPart> parts;

      float width() const
      {
        float width = 0.0f;

        for (const auto& part : parts) {
          int32_t part_width = 0;

          for (const auto& glyph : part.glyphs) {
            part_width += glyph.advance.w;
          }

          width += position_scale(part_width) * part.properties.character_size_factor;
        }

        return width;
      }

      float line_height() const
      {
        float line_height = 0.0f;

        for (const auto& part : parts) {
          line_height = std::max(line_height, part.properties.data.face->compute_line_height() * part.properties.character_size_factor);
        }

        return line_height;
      }
    };

    struct TextSpace {
      FontProperties properties;
      float width = 0.0f;
    };

    enum class TextItem : uint8_t {
      Word,
      Space,
    };

    struct TextLine {
      std::vector<TextItem> items;
      std::vector<TextWord> words;
      std::vector<TextSpace> spaces;

      float indent = 0.0f;

      float line_height() const
      {
        float line_height = 0.0f;

        for (const auto& word : words) {
          line_height = std::max(line_height, word.line_height());
        }

        return line_height;
      }
    };

    struct TextParagraph {
      std::vector<TextLine> lines;
    };

    class TextParser {
    public:
      TextParser(FontAtlas* atlas, FontFace* default_font, const TextData& data)
      : m_data(data)
      , m_atlas(atlas)
      , m_stack(default_font, data)
      {
      }

      TextParser(FontAtlas* atlas, FontFace* default_font, FontFace* bold_font, FontFace* italic_font, FontFace* bold_italic_font, const TextData& data)
      : m_data(data)
      , m_atlas(atlas)
      , m_stack(default_font, bold_font, italic_font, bold_italic_font, data)
      {
      }

      RawGeometry parse_with(TextLexer& lexer)
      {
        auto raw_paragraphs = compute_raw_paragraphs(lexer);
        compute_shape(raw_paragraphs);
        auto paragraphs = compute_paragraphs(std::move(raw_paragraphs));
        return compute_geometry(paragraphs);
      }

    private:

      std::vector<TextLine> compute_raw_paragraphs(TextLexer& lexer)
      {
        assert(m_stack.ready());
        lexer.initialize(m_data.content);

        std::vector<TextLine> paragraphs;
        TextLine current_line;
        TextWord current_word;

        for (;;) {
          auto token = lexer.next();

          if (token.type == TextTokenType::StartTag) {
            auto data = std::get<TextTag>(token.data);
            handle_start_tag(data);
            continue;
          }

          if (token.type == TextTokenType::EndTag) {
            handle_end_tag();
            continue;
          }

          if (token.type == TextTokenType::Word) {
            auto word = std::get<std::string_view>(token.data);

            assert(!m_stack.empty());
            current_word.parts.push_back({ word, m_stack.top(), {} });
            current_word.full += word;
            continue;
          }

          // space, newline or end

          if (!current_word.parts.empty()) {
            current_line.items.push_back(TextItem::Word);
            current_line.words.push_back(std::move(current_word));
            current_word = {};
          }

          if (token.type == TextTokenType::Space) {
            TextSpace space;

            assert(!m_stack.empty());
            space.properties = m_stack.top();
            current_line.items.push_back(TextItem::Space);
            current_line.spaces.push_back(std::move(space));
            continue;
          }

          if (!current_line.words.empty() || !current_line.spaces.empty()) {
            paragraphs.push_back(std::move(current_line));
            current_line = {};
          }

          if (token.type == TextTokenType::Newline) {
            continue;
          }

          assert(token.type == TextTokenType::End);
          break;
        }

        if (!m_stack.ready()) {
          Log::fatal("Mismatched tags.");
        }

        return paragraphs;
      }

      void handle_start_tag(const TextTag& data)
      {
        using namespace std::literals;

        if (data.tag == "i"sv) {
          m_stack.set_italic();
          return;
        }

        if (data.tag == "b"sv) {
          m_stack.set_bold();
          return;
        }

        if (data.tag == "size"sv) {
          float factor = 1.0f;
          auto [ptr, ec] = std::from_chars(data.value.data(), data.value.data() + data.value.size(), factor);

          if (ec != std::errc() || ptr != data.value.data() + data.value.size()) {
            Log::fatal("Could not parse float value: {}.", data.value);
          }

          m_stack.set_size_factor(factor);
          return;
        }

        if (data.tag == "color"sv) {
          uint32_t color = 0x000000;
          auto [ptr, ec] = std::from_chars(data.value.data(), data.value.data() + data.value.size(), color, 16);

          if (ec != std::errc() || ptr != data.value.data() + data.value.size() || color >= 0x1000000) {
            Log::fatal("Could not parse hex color value: {}.", data.value);
          }

          m_stack.set_color(color);
          return;
        }

        Log::fatal("Unknown tag: {}.", data.tag);
      }

      void handle_end_tag()
      {
        if (m_stack.ready() || m_stack.empty()) {
          Log::fatal("Too many end tags.");
        }

        m_stack.pop();
      }

      void compute_shape(std::vector<TextLine>& raw_paragraphs)
      {
        HbBuffer buffer(hb_buffer_create());
        std::vector<uint32_t> indices;
        FontFace* last_face = nullptr;

        for (auto& raw_line : raw_paragraphs) {
          for (auto& raw_word : raw_line.words) {

            std::size_t start = 0;

            for (auto& part : raw_word.parts) {
              if (last_face != part.properties.data.face) {
                if (!indices.empty()) {
                  assert(last_face != nullptr);
                  m_atlas->update_texture_regions_for(indices, last_face);
                  indices.clear();
                }

                last_face = part.properties.data.face;
              }

              hb_buffer_reset(buffer);
              hb_buffer_add_utf8(buffer, raw_word.full.c_str(), -1, static_cast<unsigned>(start), static_cast<int>(part.data.size()));
              hb_buffer_guess_segment_properties(buffer);

              hb_shape(part.properties.data.font, buffer, nullptr, 0);

              const unsigned length = hb_buffer_get_length(buffer);
              hb_glyph_info_t* info = hb_buffer_get_glyph_infos(buffer, nullptr);
              hb_glyph_position_t* position = hb_buffer_get_glyph_positions(buffer, nullptr);

              for (unsigned i = 0; i < length; ++i) {
                TextGlyph glyph = {};
                glyph.index = info[i].codepoint;
                glyph.advance = { position[i].x_advance, position[i].y_advance };
                glyph.offset = { position[i].x_offset, position[i].y_offset };
                part.glyphs.push_back(glyph);

                indices.push_back(info[i].codepoint);
              }

              start += part.data.size();
            }

            assert(start == raw_word.full.size());
          }

          for (auto& raw_space : raw_line.spaces) {
            raw_space.width = raw_space.properties.data.face->compute_space_width() * raw_space.properties.character_size_factor;
          }
        }

        if (last_face != nullptr && !indices.empty()) {
          m_atlas->update_texture_regions_for(indices, last_face);
        }
      }

      std::vector<TextParagraph> compute_paragraphs(std::vector<TextLine>&& raw_paragraphs)
      {
        std::vector<TextParagraph> paragraphs;

        if (m_data.alignment == Alignment::None) {
          for (auto& raw_paragraph : raw_paragraphs) {
            TextParagraph paragraph;
            paragraph.lines.push_back(std::move(raw_paragraph));
            paragraphs.push_back(std::move(paragraph));
          }
        } else {
          for (auto& raw_paragraph : raw_paragraphs) {
            paragraphs.push_back(compute_multi_line_paragraph(std::move(raw_paragraph)));
          }
        }

        return paragraphs;
      }

      TextParagraph compute_multi_line_paragraph(TextLine&& raw_line)
      {
        TextParagraph paragraph;

        TextLine line;
        float line_width = 0.0f;
        float space_width = 0.0f;
        std::size_t word_index = 0;
        std::size_t space_index = 0;

        for (auto item : raw_line.items) {
          switch (item) {
            case TextItem::Space:
              if (!line.words.empty()) {
                auto& space = raw_line.spaces[space_index];
                line_width += space.width;
                space_width += space.width;
                line.spaces.push_back(std::move(space));
                line.items.push_back(TextItem::Space);
                ++space_index;
              }
              break;

            case TextItem::Word:
              {
                auto& word = raw_line.words[word_index];
                const float word_width = word.width();

                if (!line.words.empty() && line_width + word_width > m_data.paragraph_width) {
                  // remove trailing spaces

                  while (line.items.back() == TextItem::Space) {
                    auto width = line.spaces.back().width;
                    line_width -= width;
                    space_width -= width;
                    line.items.pop_back();
                    line.spaces.pop_back();
                  }

                  // compute indent

                  switch (m_data.alignment) {
                    case Alignment::Left:
                      line.indent = 0.0f;
                      break;
                    case Alignment::Right:
                      line.indent = m_data.paragraph_width - line_width;
                      break;
                    case Alignment::Center:
                      line.indent = (m_data.paragraph_width - line_width) / 2;
                      break;
                    case Alignment::Justify:
                      if (!line.spaces.empty()) {
                        const float space_unit = (m_data.paragraph_width - line_width) / space_width;

                        for (auto& space : line.spaces) {
                          space.width += (space_unit * space.width);
                        }
                      }
                      break;

                    case Alignment::None:
                      assert(false);
                      break;
                  }

                  paragraph.lines.push_back(std::move(line));
                  line = {};
                  line_width = 0.0f;
                  space_width = 0.0f;
                }

                line_width += word_width;
                line.words.push_back(std::move(word));
                line.items.push_back(TextItem::Word);
                ++word_index;
                break;
              }
          }
        }

        // add the last line

        if (!line.words.empty()) {
          switch (m_data.alignment) {
            case Alignment::Left:
            case Alignment::Justify:
              line.indent = 0.0f;
              break;
            case Alignment::Right:
              line.indent = m_data.paragraph_width - line_width;
              break;
            case Alignment::Center:
              line.indent = (m_data.paragraph_width - line_width) / 2;
              break;
            case Alignment::None:
              assert(false);
              break;
          }

          paragraph.lines.push_back(std::move(line));
        }

        return paragraph;
      }

      RawGeometry compute_geometry(std::vector<TextParagraph>& paragraphs)
      {
        RawGeometry geometry;

        Vec2F position = { 0.0f, 0.0f };

        for (auto& paragraph : paragraphs) {
          for (auto& line : paragraph.lines) {
            position.x = line.indent;

            std::size_t word_index = 0;
            std::size_t space_index = 0;

            for (auto item : line.items) {
              switch (item) {
                case TextItem::Space:
                  assert(space_index < line.spaces.size());
                  position.x += line.spaces[space_index].width;
                  ++space_index;
                  break;
                case TextItem::Word:
                  assert(word_index < line.words.size());

                  for (auto& part : line.words[word_index].parts) {
                    auto* face = part.properties.data.face;
                    auto color = part.properties.color;
                    auto character_size_factor = part.properties.character_size_factor;

                    for (const auto& glyph : part.glyphs) {
                      const RectF glyph_bounds = m_atlas->glyph(glyph.index, face).bounds.grow_by(static_cast<float>(FontManager::spread()));
                      const RectF bounds = RectF::from_position_size((glyph.offset + glyph_bounds.offset) * character_size_factor, glyph_bounds.extent * character_size_factor);
                      const RectF texture_region = m_atlas->texture_region(glyph.index, face);

                      assert(geometry.vertices.size() < UINT16_MAX);
                      auto index = static_cast<uint16_t>(geometry.vertices.size());

                      geometry.vertices.push_back({ position + bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast), color });
                      geometry.vertices.push_back({ position + bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast), color });
                      geometry.vertices.push_back({ position + bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest), color });
                      geometry.vertices.push_back({ position + bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest), color });

                      // first triangle
                      geometry.indices.push_back(index);
                      geometry.indices.push_back(index + 1);
                      geometry.indices.push_back(index + 2);

                      // second triangle
                      geometry.indices.push_back(index + 2);
                      geometry.indices.push_back(index + 1);
                      geometry.indices.push_back(index + 3);

                      position.x += position_scale(glyph.advance.x) * character_size_factor;
                    }
                  }

                  ++word_index;
                  break;
              }
            }

            position.y += line.line_height();
          }
        }

        return geometry;
      }

      const TextData& m_data; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
      FontAtlas* m_atlas = nullptr;
      FontPropertiesStack m_stack;
    };

  }

  /*
   * Text
   */

  Text::Text(FontAtlas* atlas, FontFace* default_font, const TextData& data, RenderManager* render_manager)
  : m_atlas(atlas)
  , m_font(default_font)
  {
    update_buffers(data, render_manager);
  }

  Text::Text(FontAtlas* atlas, const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_atlas(atlas)
  , m_font(resource_manager->get<FontFace>(resource.font))
  {
    update_buffers(resource.data, render_manager);
  }

  RenderGeometry Text::geometry() const
  {
    RenderGeometry geometry;
    geometry.pipeline = RenderPipelineType::Text;
    geometry.texture = m_atlas->texture();
    geometry.vertices = &m_vertices;
    geometry.indices = &m_indices;
    geometry.size = m_indices.size();

    return geometry;
  }

  void Text::update_buffers(const TextData& data, RenderManager* render_manager)
  {
    assert(m_atlas);

    TextParser parser(m_atlas, m_font, data);
    SimpleTextLexer lexer;

    auto geometry = parser.parse_with(lexer);

    m_atlas->update_texture(render_manager);

    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, geometry.indices.data(), geometry.indices.size(), render_manager);

    m_bounds = geometry.compute_bounds();
  }

  /*
   * RichText
   */

  RichText::RichText(FontAtlas* atlas, FontFace* default_font, FontFace* bold_font, FontFace* italic_font, FontFace* bold_italic_font, const TextData& data, RenderManager* render_manager)
  : m_atlas(atlas)
  , m_default_font(default_font)
  , m_bold_font(bold_font)
  , m_italic_font(italic_font)
  , m_bold_italic_font(bold_italic_font)
  {
    update_buffers(data, render_manager);
  }

  RichText::RichText(FontAtlas* atlas, const RichTextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : m_atlas(atlas)
  , m_default_font(resource_manager->get<FontFace>(resource.default_font))
  , m_bold_font(resource.bold_font.empty() ? nullptr : resource_manager->get<FontFace>(resource.bold_font))
  , m_italic_font(resource.italic_font.empty() ? nullptr : resource_manager->get<FontFace>(resource.italic_font))
  , m_bold_italic_font(resource.bold_italic_font.empty() ? nullptr : resource_manager->get<FontFace>(resource.bold_italic_font))
  {
    update_buffers(resource.data, render_manager);
  }

  RenderGeometry RichText::geometry() const
  {
    RenderGeometry geometry;
    geometry.pipeline = RenderPipelineType::Text;
    geometry.texture = m_atlas->texture();
    geometry.vertices = &m_vertices;
    geometry.indices = &m_indices;
    geometry.size = m_indices.size();

    return geometry;
  }

  void RichText::update_buffers(const TextData& data, RenderManager* render_manager)
  {
    assert(m_atlas);

    TextParser parser(m_atlas, m_default_font, m_bold_font, m_italic_font, m_bold_italic_font, data);
    RichTextLexer lexer;

    auto geometry = parser.parse_with(lexer);

    m_atlas->update_texture(render_manager);

    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, geometry.indices.data(), geometry.indices.size(), render_manager);

    m_bounds = geometry.compute_bounds().shrink_by(static_cast<float>(FontManager::spread()));
  }

}
