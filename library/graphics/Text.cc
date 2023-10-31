// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Text.h>
// clang-format on

#include <cassert>

#include <string_view>
#include <vector>

#include <gf2/core/ResourceManager.h>
#include <gf2/core/StringUtils.h>

#include <gf2/graphics/FontPack.h>
#include <gf2/graphics/Vertex.h>

namespace gf {

  namespace {

    float compute_word_width(std::string_view word, FontAtlas* atlas, const TextData& data)
    {
      float word_width = 0.0f;
      char32_t prev_codepoint = '\0';

      for (const char32_t curr_codepoint : gf::codepoints(word)) {
        word_width += atlas->kerning(prev_codepoint, curr_codepoint, data.character_size);
        prev_codepoint = curr_codepoint;
        word_width += atlas->glyph(curr_codepoint, data.character_size).advance;
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

    Paragraph compute_multi_line_paragraph(const std::vector<std::string_view>& raw_words, FontAtlas* atlas, const TextData& data, float space_width)
    {
      Paragraph paragraph;

      ParagraphLine line;
      float line_width = 0.0f;

      for (auto word : raw_words) {
        const float word_width = compute_word_width(word, atlas, data);

        if (!line.words.empty() && line_width + space_width + word_width > data.paragraph_width) {
          auto word_count = line.words.size();

          switch (data.alignment) {
            case Alignment::Left:
              line.indent = 0.0f;
              line.spacing = space_width;
              break;

            case Alignment::Right:
              line.indent = data.paragraph_width - line_width;
              line.spacing = space_width;
              break;

            case Alignment::Center:
              line.indent = (data.paragraph_width - line_width) / 2;
              line.spacing = space_width;
              break;

            case Alignment::Justify:
              line.indent = 0.0f;

              if (word_count > 1) {
                line.spacing = space_width + (data.paragraph_width - line_width) / static_cast<float>(word_count - 1);
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
          line_width += space_width + word_width;
        }

        line.words.push_back(word);
      }

      // add the last line
      if (!line.words.empty()) {
        switch (data.alignment) {
          case Alignment::Left:
          case Alignment::Justify:
            line.indent = 0.0f;
            line.spacing = space_width;
            break;

          case Alignment::Right:
            line.indent = data.paragraph_width - line_width;
            line.spacing = space_width;
            break;

          case Alignment::Center:
            line.indent = (data.paragraph_width - line_width) / 2;
            line.spacing = space_width;
            break;

          case Alignment::None:
            assert(false);
            break;
        }

        paragraph.lines.push_back(std::move(line));
      }

      return paragraph;
    }

    std::vector<Paragraph> compute_paragraphs(FontAtlas* atlas, const TextData& data, float space_width)
    {
      const std::vector<std::string_view> raw_paragraphs = split_in_paragraphs(data.content);
      std::vector<Paragraph> paragraphs;

      if (data.alignment == Alignment::None) {
        for (auto raw_paragraph : raw_paragraphs) {
          std::vector<std::string_view> raw_words = split_in_words(raw_paragraph);

          ParagraphLine line;
          line.words = std::move(raw_words);
          line.indent = 0.0f;
          line.spacing = space_width;

          Paragraph paragraph;
          paragraph.lines.push_back(std::move(line));

          paragraphs.push_back(std::move(paragraph));
        }
      } else {
        for (auto raw_paragraph : raw_paragraphs) {
          const std::vector<std::string_view> raw_words = split_in_words(raw_paragraph);
          paragraphs.push_back(compute_multi_line_paragraph(raw_words, atlas, data, space_width));
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

    float compute_space_width(FontAtlas* atlas, const TextData& data)
    {
      const float space_width = atlas->glyph(' ', data.character_size).advance;
      const float additional_space = (space_width / 3) * (data.letter_spacing_factor - 1.0f); // TODO: investigate!
      return space_width + additional_space;
    }

    void compute_vertices(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, RectF bounds, RectF texture_region, Vec2F position, Color color)
    {
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

    RawTextGeometry compute_geometry(const std::vector<Paragraph>& paragraphs, FontAtlas* atlas, const TextData& data)
    {
      RawTextGeometry geometry;
      const Color linear_color = gf::srgb_to_linear(data.color);

      const float line_height = atlas->line_spacing(data.character_size) * data.line_spacing_factor;

      Vec2F position = { 0.0f, 0.0f };
      Vec2F min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
      Vec2F max = { 0.0f, 0.0f };

      for (const auto& paragraph : paragraphs) {
        for (const auto& line : paragraph.lines) {
          position.x += line.indent;

          for (auto word : line.words) {
            char32_t prev_codepoint = '\0';

            for (const char32_t curr_codepoint : gf::codepoints(word)) {
              position.x += atlas->kerning(prev_codepoint, curr_codepoint, data.character_size);
              prev_codepoint = curr_codepoint;

              if (data.outline_thickness > 0) {
                const RectF bounds = atlas->glyph(curr_codepoint, data.character_size, data.outline_thickness).bounds;
                const RectF texture_region = atlas->texture_region(curr_codepoint, data.character_size, data.outline_thickness);
                compute_vertices(geometry.outline_vertices, geometry.outline_indices, bounds, texture_region, position, data.outline_color);

                min = gf::min(min, position + bounds.position_at(Orientation::NorthWest));
                max = gf::max(max, position + bounds.position_at(Orientation::SouthEast));
              }

              auto glyph = atlas->glyph(curr_codepoint, data.character_size);
              const RectF texture_region = atlas->texture_region(curr_codepoint, data.character_size);
              compute_vertices(geometry.vertices, geometry.indices, glyph.bounds, texture_region, position, linear_color);

              if (data.outline_thickness == 0.0f) {
                min = gf::min(min, position + glyph.bounds.position_at(Orientation::NorthWest));
                max = gf::max(max, position + glyph.bounds.position_at(Orientation::SouthEast));
              }

              position.x += glyph.advance; // TODO: + additional_space;
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
  {
    assert(atlas);
    atlas->update_texture_regions_for(data.content, data.character_size);

    if (data.outline_thickness > 0) {
      atlas->update_texture_regions_for(data.content, data.character_size, data.outline_thickness);
    }

    atlas->update_texture(render_manager);

    const float space_width = compute_space_width(atlas, data);
    auto paragraphs = compute_paragraphs(atlas, data, space_width);
    auto geometry = compute_geometry(paragraphs, atlas, data);

    m_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_indices = Buffer(BufferType::Device, BufferUsage::Index, geometry.indices.data(), geometry.indices.size(), render_manager);

    if (data.outline_thickness > 0.0f) {
      m_outline_vertices = Buffer(BufferType::Device, BufferUsage::Vertex, geometry.outline_vertices.data(), geometry.outline_vertices.size(), render_manager);
      m_outline_indices = Buffer(BufferType::Device, BufferUsage::Index, geometry.outline_indices.data(), geometry.outline_indices.size(), render_manager);
    }

    m_bounds = geometry.bounds;
  }

  Text::Text(const TextResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : Text(resource_manager->get<FontPack>(resource.font)->atlas(), resource.data, render_manager)
  {
  }

  TextGeometry Text::geometry() const
  {
    TextGeometry geometry;

    RenderGeometry text;
    text.pipeline = RenderPipelineType::Text;
    text.texture = m_atlas->texture();
    text.vertices = &m_vertices;
    text.indices = &m_indices;
    text.count = m_indices.count();

    geometry.text = text;

    if (m_outline_vertices.has_value()) {
      assert(m_outline_indices.has_value());

      RenderGeometry outline;
      outline.pipeline = RenderPipelineType::Text;
      outline.texture = m_atlas->texture();
      outline.vertices = &m_outline_vertices.value();
      outline.indices = &m_outline_indices.value();
      outline.count = m_outline_indices.value().count();

      geometry.outline = outline;
    }

    return geometry;
  }

}
