// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Text.h>
// clang-format on

#include <cassert>

#include <string_view>
#include <vector>

#include <gf2/core/FontManager.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/StringUtils.h>

#include <gf2/graphics/Font.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  namespace {

    float compute_word_width(std::string_view word, FontAtlas* atlas)
    {
      float word_width = 0.0f;
      char32_t prev_codepoint = '\0';

      for (const char32_t curr_codepoint : gf::codepoints(word)) {
        word_width += atlas->kerning(prev_codepoint, curr_codepoint);
        prev_codepoint = curr_codepoint;
        word_width += atlas->glyph(curr_codepoint).advance;
      }

      return word_width;
    }

    struct ParagraphLine {
      std::vector<std::string_view> words;
      float indent = 0.0f;
      float spacing = 0.0f;
    };

    struct Paragraph {
      std::vector<ParagraphLine> lines;
    };

    struct ParagraphProperties {
      float paragraph_width = 0.0f;
      float space_width = 0.0f;
      float additional_space = 0.0f;
    };

    Paragraph compute_multi_line_paragraph(const std::vector<std::string_view>& raw_words, FontAtlas* atlas, const TextData& data, ParagraphProperties properties)
    {
      Paragraph paragraph;

      ParagraphLine line;
      float line_width = 0.0f;

      for (auto word : raw_words) {
        const float word_width = compute_word_width(word, atlas);

        if (!line.words.empty() && line_width + properties.space_width + word_width > properties.paragraph_width) {
          auto word_count = line.words.size();

          switch (data.alignment) {
            case Alignment::Left:
              line.indent = 0.0f;
              line.spacing = properties.space_width;
              break;

            case Alignment::Right:
              line.indent = properties.paragraph_width - line_width;
              line.spacing = properties.space_width;
              break;

            case Alignment::Center:
              line.indent = (properties.paragraph_width - line_width) / 2;
              line.spacing = properties.space_width;
              break;

            case Alignment::Justify:
              line.indent = 0.0f;

              if (word_count > 1) {
                line.spacing = properties.space_width + (properties.paragraph_width - line_width) / static_cast<float>(word_count - 1);
              } else {
                line.spacing = 0.0f;
              }

              break;

            case Alignment::None:
              assert(false);
              break;
          }

          paragraph.lines.push_back(std::move(line));
          line = {};
        }

        if (line.words.empty()) {
          line_width = word_width;
        } else {
          line_width += properties.space_width + word_width;
        }

        line.words.push_back(word);
      }

      // add the last line
      if (!line.words.empty()) {
        switch (data.alignment) {
          case Alignment::Left:
          case Alignment::Justify:
            line.indent = 0.0f;
            line.spacing = properties.space_width;
            break;

          case Alignment::Right:
            line.indent = properties.paragraph_width - line_width;
            line.spacing = properties.space_width;
            break;

          case Alignment::Center:
            line.indent = (properties.paragraph_width - line_width) / 2;
            line.spacing = properties.space_width;
            break;

          case Alignment::None:
            assert(false);
            break;
        }

        paragraph.lines.push_back(std::move(line));
      }

      return paragraph;
    }

    std::vector<Paragraph> compute_paragraphs(FontAtlas* atlas, const TextData& data, ParagraphProperties properties)
    {
      const std::vector<std::string_view> raw_paragraphs = split_in_paragraphs(data.content);
      std::vector<Paragraph> paragraphs;

      if (data.alignment == Alignment::None) {
        for (auto raw_paragraph : raw_paragraphs) {
          std::vector<std::string_view> raw_words = split_in_words(raw_paragraph);

          ParagraphLine line;
          line.words = std::move(raw_words);
          line.indent = 0.0f;
          line.spacing = properties.space_width;

          Paragraph paragraph;
          paragraph.lines.push_back(std::move(line));

          paragraphs.push_back(std::move(paragraph));
        }
      } else {
        for (auto raw_paragraph : raw_paragraphs) {
          const std::vector<std::string_view> raw_words = split_in_words(raw_paragraph);
          paragraphs.push_back(compute_multi_line_paragraph(raw_words, atlas, data, properties));
        }
      }

      return paragraphs;
    }

    struct RawTextGeometry {
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
      std::vector<Vertex> outline_vertices;
      std::vector<uint16_t> outline_indices;
      RectF bounds = {};
    };

    ParagraphProperties compute_paragraph_properties(FontAtlas* atlas, const TextData& data)
    {
      ParagraphProperties properties;
      properties.space_width = atlas->glyph(' ').advance;
      properties.additional_space = (properties.space_width / 3) * (data.letter_spacing_factor - 1.0f); // TODO: investigate!
      properties.space_width += properties.additional_space;
      properties.paragraph_width = data.paragraph_width * data.character_size / 64.0f;
      return properties;
    }

    void compute_vertices(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, RectF bounds, RectF texture_region, Vec2F position, Color color)
    {
      bounds = bounds.grow_by(static_cast<float>(FontManager::spread() - 1));

      assert(vertices.size() < UINT16_MAX);
      auto index = static_cast<uint16_t>(vertices.size());

      vertices.push_back({ position + bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast), color });
      vertices.push_back({ position + bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast), color });
      vertices.push_back({ position + bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest), color });
      vertices.push_back({ position + bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest), color });

      // first triangle
      indices.push_back(index);
      indices.push_back(index + 1);
      indices.push_back(index + 2);

      // second triangle
      indices.push_back(index + 2);
      indices.push_back(index + 1);
      indices.push_back(index + 3);
    }

    RawTextGeometry compute_geometry(const std::vector<Paragraph>& paragraphs, FontAtlas* atlas, const TextData& data, ParagraphProperties properties)
    {
      RawTextGeometry geometry;
      const Color linear_color = gf::srgb_to_linear(data.color);

      const float line_height = atlas->line_spacing() * data.line_spacing_factor;

      Vec2F position = { 0.0f, 0.0f };
      Vec2F min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
      Vec2F max = { 0.0f, 0.0f };

      for (const auto& paragraph : paragraphs) {
        for (const auto& line : paragraph.lines) {
          position.x += line.indent;

          for (auto word : line.words) {
            char32_t prev_codepoint = '\0';

            for (const char32_t curr_codepoint : gf::codepoints(word)) {
              position.x += atlas->kerning(prev_codepoint, curr_codepoint);
              prev_codepoint = curr_codepoint;

              auto glyph = atlas->glyph(curr_codepoint);
              const RectF texture_region = atlas->texture_region(curr_codepoint);
              compute_vertices(geometry.vertices, geometry.indices, glyph.bounds, texture_region, position, linear_color);

              min = gf::min(min, position + glyph.bounds.position_at(Orientation::NorthWest));
              max = gf::max(max, position + glyph.bounds.position_at(Orientation::SouthEast));

              position.x += glyph.advance + properties.additional_space;
            }

            position.x += line.spacing;
          }

          position.y += line_height;
        }
      }

      geometry.bounds = RectF::from_min_max(min, max);

      if (data.alignment != Alignment::None && data.paragraph_width > 0.0f) {
        geometry.bounds.offset.x = 0.0f;
        geometry.bounds.extent.w = data.paragraph_width;
      }

      return geometry;
    }

  }

  Text::Text(FontAtlas* atlas, const TextData& data, RenderManager* render_manager)
  : m_atlas(atlas)
  , m_character_size(data.character_size)
  {
    assert(atlas);
    atlas->update_texture_regions_for(data.content);
    atlas->update_texture(render_manager);

    auto properties = compute_paragraph_properties(atlas, data);
    auto paragraphs = compute_paragraphs(atlas, data, properties);
    auto geometry = compute_geometry(paragraphs, atlas, data, properties);

    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, geometry.indices.data(), geometry.indices.size(), render_manager);

    m_bounds = geometry.bounds;
  }

  Text::Text(const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : Text(resource_manager->get<Font>(resource.font)->atlas(), resource.data, render_manager)
  {
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

  void Text::set_characater_size(float character_size)
  {
    m_character_size = character_size;
  }

  float Text::characater_size() const
  {
    return m_character_size;
  }

}
