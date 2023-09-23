// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/FontAtlas.h>
// clang-format on

#include <cassert>

#include <algorithm>
#include <stdexcept>

#include <gf2/core/StringUtils.h>

namespace gf {

  using namespace operators;

  FontAtlas::FontAtlas(Font* font, Vec2I size, Renderer* renderer)
  : m_font(font)
  , m_bin_pack(size)
  , m_bitmap(size, 0x00)
  , m_texture(size, TextureUsage::TransferDestination | TextureUsage::Sampled, Format::Gray8U, renderer)
  {
  }

  RectF FontAtlas::texture_region(char32_t codepoint, unsigned character_size, float outline_thickness)
  {
    const Fixed32 fixed_thickness(outline_thickness);
    auto key = std::make_tuple(codepoint, character_size, fixed_thickness);

    if (auto iterator = m_atlas.find(key); iterator != m_atlas.end()) {
      return iterator->second;
    }

    auto font_glyph = glyph(codepoint, character_size, outline_thickness);
    auto maybe_rectangle = m_bin_pack.insert(font_glyph.bitmap.size());

    if (!maybe_rectangle) {
      throw std::runtime_error("Unable to insert rectangle in the bin pack.");
    }

    const RectI bounds = *maybe_rectangle;
    assert(bounds.size() == font_glyph.bitmap.size());

    m_bitmap.blit(font_glyph.bitmap, bounds.position());

    const Vec2F total_size = m_bitmap.size();
    const RectF texture_region = RectF::from_position_size(Vec2F(bounds.position()) / total_size, Vec2F(bounds.size()) / total_size);
    m_atlas.emplace(key, texture_region);
    return texture_region;
  }

  void FontAtlas::update_texture_regions_for(std::string_view string, uint32_t character_size, float outline_thickness)
  {
    const Fixed32 fixed_thickness(outline_thickness);
    std::vector<char32_t> new_codepoints;

    for (auto codepoint : gf::codepoints(string)) {
      if (codepoint == U' ') {
        continue;
      }

      auto key = std::make_tuple(codepoint, character_size, fixed_thickness);

      if (auto it = m_atlas.find(key); it != m_atlas.end()) {
        continue;
      }

      new_codepoints.push_back(codepoint);
    }

    if (new_codepoints.empty()) {
      return;
    }

    std::sort(new_codepoints.begin(), new_codepoints.end());
    new_codepoints.erase(std::unique(new_codepoints.begin(), new_codepoints.end()), new_codepoints.end());

    const std::size_t count = new_codepoints.size();

    std::vector<Vec2I> sizes;
    std::vector<const FontGlyph*> glyphs;

    for (auto codepoint : new_codepoints) {
      const auto& font_glyph = glyph(codepoint, character_size, outline_thickness);
      sizes.push_back(font_glyph.bitmap.size());
      glyphs.push_back(&font_glyph);
    }

    assert(sizes.size() == count);
    assert(glyphs.size() == count);

    auto maybe_rectangles = m_bin_pack.insert(sizes);

    if (!maybe_rectangles) {
      throw std::runtime_error("Unable to insert rectangle in the bin pack.");
    }

    auto rectangles = std::move(*maybe_rectangles);
    assert(rectangles.size() == count);

    for (std::size_t i = 0; i < count; ++i) {
      auto key = std::make_tuple(new_codepoints[i], character_size, fixed_thickness);

      const RectI bounds = rectangles[i];
      assert(bounds.size() == sizes[i]);

      m_bitmap.blit(glyphs[i]->bitmap, bounds.position());

      const Vec2F total_size = m_bitmap.size();
      const RectF texture_region = RectF::from_position_size(Vec2F(bounds.position()) / total_size, Vec2F(bounds.size()) / total_size);
      m_atlas.emplace(key, texture_region);
    }
  }

  const FontGlyph& FontAtlas::glyph(char32_t codepoint, uint32_t character_size, float outline_thickness)
  {
    const Fixed32 fixed_thickness(outline_thickness);
    auto key = std::make_tuple(codepoint, character_size, fixed_thickness);

    if (auto iterator = m_glyph_cache.find(key); iterator != m_glyph_cache.end()) {
      return iterator->second;
    }

    auto glyph = m_font->create_glyph(codepoint, character_size, outline_thickness);
    auto [iterator, inserted] = m_glyph_cache.emplace(key, std::move(glyph));
    assert(inserted);
    return iterator->second;
  }

  void FontAtlas::update_texture(Renderer* renderer)
  {
    m_texture.update(m_bitmap, renderer);
  }

}
