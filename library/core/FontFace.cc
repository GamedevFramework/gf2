// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/FontFace.h>

#include <cassert>

#include <utility>

// clang-format off
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H
// clang-format on

#include <gf2/core/FontManager.h>
#include <gf2/core/Log.h>

namespace gf {

  namespace {

    constexpr unsigned FontSize = 64;
    constexpr float Scale = (1 << 6);

    float ft_convert_metrics(FT_Pos value)
    {
      return static_cast<float>(value) / Scale;
    }

    unsigned long ft_callback_read(FT_Stream rec, unsigned long offset, unsigned char* buffer, unsigned long count)
    {
      auto* stream = static_cast<InputStream*>(rec->descriptor.pointer);
      stream->seek(static_cast<ptrdiff_t>(offset));

      if (count == 0) {
        return 0;
      }

      return static_cast<unsigned long>(stream->read(Span<uint8_t>(buffer, count)));
    }

    void ft_callback_close([[maybe_unused]] FT_Stream rec)
    {
      // nothing to do
    }

  }

  FontFace::FontFace(const std::filesystem::path& filename, FontManager* manager)
  : m_manager(manager)
  {
    assert(manager != nullptr);

    FT_Face face = nullptr;

    if (auto err = FT_New_Face(m_manager->library_as<FT_Library>(), filename.string().c_str(), 0, &face)) {
      Log::fatal("Could not create the font face '{}': {}\n", filename.string(), FontManager::error_message(err));
    }

    m_face = face;

    set_current_character_size(FontSize);
  }

  FontFace::FontFace(InputStream& stream, FontManager* manager)
  : m_manager(manager)
  {
    assert(manager != nullptr);

    FT_StreamRec rec = {};
    std::memset(&rec, 0, sizeof(FT_StreamRec));
    rec.base = nullptr;
    rec.size = 0x7FFFFFFF; // unknown size
    rec.pos = 0;
    rec.descriptor.pointer = &stream;
    rec.read = ft_callback_read;
    rec.close = ft_callback_close;

    FT_Open_Args args = {};
    std::memset(&args, 0, sizeof(FT_Open_Args));
    args.flags = FT_OPEN_STREAM;
    args.stream = &rec;
    args.driver = nullptr;

    FT_Face face = nullptr;

    if (auto err = FT_Open_Face(m_manager->library_as<FT_Library>(), &args, 0, &face)) {
      Log::fatal("Could not create the font face from stream: {}", FontManager::error_message(err));
    }

    m_face = face;

    set_current_character_size(FontSize);
  }

  FontFace::FontFace(FontFace&& other) noexcept
  : m_manager(other.m_manager)
  , m_face(std::exchange(other.m_face, nullptr))
  {
  }

  FontFace::~FontFace()
  {
    if (m_face != nullptr) {
      [[maybe_unused]] auto err = FT_Done_Face(face_as<FT_Face>());
      assert(err == FT_Err_Ok);
    }
  }

  FontFace& FontFace::operator=(FontFace&& other) noexcept
  {
    using std::swap;
    swap(m_manager, other.m_manager);
    swap(m_face, other.m_face);
    return *this;
  }

  std::string FontFace::family_name() const
  {
    if (m_face == nullptr) {
      return "";
    }

    return face_as<FT_Face>()->family_name;
  }

  std::string FontFace::style_name() const
  {
    if (m_face == nullptr) {
      return "";
    }

    return face_as<FT_Face>()->style_name;
  }

  uint32_t FontFace::index_from_character(char32_t character)
  {
    if (m_face == nullptr) {
      return 0;
    }

    auto* face = face_as<FT_Face>();
    return FT_Get_Char_Index(face, character);
  }

  FontGlyph FontFace::create_glyph(uint32_t index)
  {
    FontGlyph result = {};

    if (m_face == nullptr) {
      return result;
    }

    auto* face = face_as<FT_Face>();

    if (auto err = FT_Load_Glyph(face, index, FT_LOAD_TARGET_NORMAL)) {
      Log::error("Could not load the glyph: {}", FontManager::error_message(err));
      return result;
    }

    /*
     * BSDF mode
     */

    auto* glyph = face->glyph;

    if (auto err = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL)) {
      Log::error("Could not create a bitmap from the glyph (index {}): {}", index, FontManager::error_message(err));
      return result;
    }

    if (auto err = FT_Render_Glyph(glyph, FT_RENDER_MODE_SDF)) {
      Log::error("Could not create a bitmap from the glyph (index {}): {}", index, FontManager::error_message(err));
      return result;
    }

    assert(glyph->format == FT_GLYPH_FORMAT_BITMAP);
    auto* bitmap = &glyph->bitmap;

    // size

    const Vec2I glyph_size(static_cast<int>(bitmap->width), static_cast<int>(bitmap->rows));

    if (glyph_size.h == 0 || glyph_size.w == 0) {
      return result;
    }

    // bounds

    // clang-format: off
    result.bounds = RectF::from_position_size(
        { ft_convert_metrics(glyph->metrics.horiBearingX), -ft_convert_metrics(glyph->metrics.horiBearingY) },
        { ft_convert_metrics(glyph->metrics.width), ft_convert_metrics(glyph->metrics.height) });
    // clang-format: on

    // bitmap

    result.bitmap = Bitmap(glyph_size, bitmap->buffer, bitmap->pitch);
    return result;
  }

  float FontFace::compute_line_height()
  {
    if (m_face == nullptr) {
      return 0.0f;
    }

    return ft_convert_metrics(face_as<FT_Face>()->size->metrics.height);
  }

  float FontFace::compute_space_width()
  {
    if (m_face == nullptr) {
      return 0.0f;
    }

    auto* face = face_as<FT_Face>();

    if (auto err = FT_Load_Char(face, U' ', FT_LOAD_DEFAULT)) {
      Log::error("Could not load the glyph: {}", FontManager::error_message(err));
      return 0.0f;
    }

    return ft_convert_metrics(face->glyph->metrics.horiAdvance);
  }

  float FontFace::size_factor(float character_size)
  {
    return character_size / static_cast<float>(FontSize);
  }

  bool FontFace::set_current_character_size(uint32_t character_size)
  {
    if (auto err = FT_Set_Pixel_Sizes(face_as<FT_Face>(), 0, character_size)) {
      Log::error("Could not change the font size: {}", FontManager::error_message(err));
      return false;
    }

    return true;
  }

}
