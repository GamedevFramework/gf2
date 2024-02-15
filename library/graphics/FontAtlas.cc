// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/FontAtlas.h>

#include <cassert>

#include <algorithm>

#include <gf2/core/FontManager.h>
#include <gf2/core/Log.h>
#include <gf2/core/StringUtils.h>

namespace gf {

  namespace {

    constexpr int FontAtlasPadding = 1;

  }

  using namespace operators;

  FontAtlas::FontAtlas(FontFace* face, Vec2I size, RenderManager* render_manager)
  : m_face(face)
  , m_bin_pack(size)
  , m_bitmap(size, 0x00)
  , m_texture(size, TextureUsage::TransferDestination | TextureUsage::Sampled, Format::Gray8U, render_manager)
  {
  }

  RectF FontAtlas::texture_region(char32_t codepoint)
  {
    auto key = codepoint;

    if (auto iterator = m_atlas.find(key); iterator != m_atlas.end()) {
      return iterator->second;
    }

    auto font_glyph = glyph(codepoint);
    auto maybe_rectangle = m_bin_pack.insert(font_glyph.bitmap.size() + 2 * FontAtlasPadding);

    if (!maybe_rectangle) {
      Log::fatal("Unable to insert rectangle in the bin pack.");
    }

    const RectI bounds = *maybe_rectangle;
    assert(bounds.size() == font_glyph.bitmap.size() + 2 * FontAtlasPadding);

    m_bitmap.blit(font_glyph.bitmap, bounds.position() + FontAtlasPadding);

    const Vec2F texture_abs_position = bounds.position() + FontAtlasPadding;
    const Vec2F texture_abs_size = bounds.size() - 2 * FontAtlasPadding;
    const Vec2F total_size = m_bitmap.size();

    const RectF texture_region = RectF::from_position_size(texture_abs_position / total_size, texture_abs_size / total_size);

    m_atlas.emplace(key, texture_region);
    return texture_region;
  }

  void FontAtlas::update_texture_regions_for(std::string_view string)
  {
    std::vector<char32_t> new_codepoints;

    for (auto codepoint : gf::codepoints(string)) {
      if (codepoint == U' ') {
        continue;
      }

      auto key = codepoint;

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
      const auto& font_glyph = glyph(codepoint);
      sizes.push_back(font_glyph.bitmap.size() + 2 * FontAtlasPadding);
      glyphs.push_back(&font_glyph);
    }

    assert(sizes.size() == count);
    assert(glyphs.size() == count);

    auto maybe_rectangles = m_bin_pack.insert(sizes);

    if (!maybe_rectangles) {
      Log::fatal("Unable to insert rectangle in the bin pack.");
    }

    auto rectangles = std::move(*maybe_rectangles);
    assert(rectangles.size() == count);

    for (std::size_t i = 0; i < count; ++i) {
      auto key = new_codepoints[i];

      const RectI bounds = rectangles[i];
      assert(bounds.size() == sizes[i]);
      assert(bounds.size() == glyphs[i]->bitmap.size() + 2 * FontAtlasPadding);

      m_bitmap.blit(glyphs[i]->bitmap, bounds.position() + FontAtlasPadding);

      const Vec2F texture_abs_position = bounds.position() + FontAtlasPadding;
      const Vec2F texture_abs_size = bounds.size() - 2 * FontAtlasPadding;
      const Vec2F total_size = m_bitmap.size();

      const RectF texture_region = RectF::from_position_size(texture_abs_position / total_size, texture_abs_size / total_size);

      m_atlas.emplace(key, texture_region);
    }
  }

  const FontGlyph& FontAtlas::glyph(char32_t codepoint)
  {
    auto key = codepoint;

    if (auto iterator = m_glyph_cache.find(key); iterator != m_glyph_cache.end()) {
      return iterator->second;
    }

    auto glyph = m_face->create_glyph(codepoint);
    auto [iterator, inserted] = m_glyph_cache.emplace(key, std::move(glyph));
    assert(inserted);
    return iterator->second;
  }

  void FontAtlas::update_texture(RenderManager* render_manager)
  {
    m_texture.update(m_bitmap, render_manager);
  }

}
