// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/core/Font.h>
// clang-format on

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

  Font::Font(const std::filesystem::path& filename, FontManager* manager)
  : m_manager(manager)
  {
    assert(manager != nullptr);

    FT_Face face = nullptr;

    if (auto err = FT_New_Face(m_manager->library_as<FT_Library>(), filename.string().c_str(), 0, &face)) {
      Log::fatal("Could not create the font face '{}': {}\n", filename.string(), FontManager::error_message(err));
    }

    m_face = face;
  }

  Font::Font(InputStream& stream, FontManager* manager)
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
  }

  Font::Font(Font&& other) noexcept
  : m_manager(other.m_manager)
  , m_face(std::exchange(other.m_face, nullptr))
  , m_character_size(std::exchange(other.m_character_size, 0))
  {
  }

  Font::~Font()
  {
    if (m_face != nullptr) {
      [[maybe_unused]] auto err = FT_Done_Face(face_as<FT_Face>());
      assert(err == FT_Err_Ok);
    }
  }

  Font& Font::operator=(Font&& other) noexcept
  {
    using std::swap;
    swap(m_manager, other.m_manager);
    swap(m_face, other.m_face);
    swap(m_character_size, other.m_character_size);
    return *this;
  }

  std::string Font::family_name() const
  {
    if (m_face == nullptr) {
      return "";
    }

    return face_as<FT_Face>()->family_name;
  }

  std::string Font::style_name() const
  {
    if (m_face == nullptr) {
      return "";
    }

    return face_as<FT_Face>()->style_name;
  }

  FontGlyph Font::create_glyph(char32_t codepoint, uint32_t character_size, float outline_thickness)
  {
    FontGlyph result = {};

    if (m_face == nullptr) {
      return result;
    }

    if (!set_current_character_size(character_size)) {
      return result;
    }

    auto* face = face_as<FT_Face>();
    FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;

    if (outline_thickness > 0) {
      flags |= FT_LOAD_NO_BITMAP;
    }

    if (auto err = FT_Load_Char(face, codepoint, flags)) {
      Log::error("Could not load the glyph: {}", FontManager::error_message(err));
      return result;
    }

    FT_GlyphSlot slot = face->glyph;
    FT_Glyph glyph = nullptr;

    if (auto err = FT_Get_Glyph(slot, &glyph)) {
      Log::error("Could not extract the glyph: {}", FontManager::error_message(err));
      return result;
    }

    if (outline_thickness > 0) {
      assert(glyph->format == FT_GLYPH_FORMAT_OUTLINE);

      auto* stroker = m_manager->stroker_as<FT_Stroker>();

      FT_Stroker_Set(stroker, static_cast<FT_Fixed>(outline_thickness * Scale), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
      FT_Glyph_Stroke(&glyph, stroker, 0);
    }

    if (auto err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, 1)) {
      Log::error("Could create a bitmap from the glyph: {}", FontManager::error_message(err));
      FT_Done_Glyph(glyph);
      return result;
    }

    assert(glyph->format == FT_GLYPH_FORMAT_BITMAP);
    auto* bglyph = reinterpret_cast<FT_BitmapGlyph>(glyph); // NOLINT

    // advance

    result.advance = ft_convert_metrics(slot->metrics.horiAdvance);

    // size

    const Vec2I glyph_size(static_cast<int>(bglyph->bitmap.width), static_cast<int>(bglyph->bitmap.rows));

    if (glyph_size.h == 0 || glyph_size.w == 0) {
      FT_Done_Glyph(glyph);
      return result;
    }

    // bounds

    if (outline_thickness > 0.0f) {
      // clang-format: off
      result.bounds = RectF::from_position_size(
          { static_cast<float>(bglyph->left), -static_cast<float>(bglyph->top) },
          { static_cast<float>(bglyph->bitmap.width), static_cast<float>(bglyph->bitmap.rows) }
          // clang-format: on
      );
    } else {
      // clang-format: off
      result.bounds = RectF::from_position_size(
          { ft_convert_metrics(slot->metrics.horiBearingX), -ft_convert_metrics(slot->metrics.horiBearingY) },
          { ft_convert_metrics(slot->metrics.width), ft_convert_metrics(slot->metrics.height) });
      // clang-format: on
    }

    // bitmap

    result.bitmap = Bitmap(glyph_size, bglyph->bitmap.buffer, bglyph->bitmap.pitch);

    FT_Done_Glyph(glyph);
    return result;
  }

  float Font::compute_kerning(char32_t left, char32_t right, uint32_t character_size)
  {
    if (left == 0 || right == 0) {
      return 0.0f;
    }

    if (m_face == nullptr) {
      return 0.0f;
    }

    if (!set_current_character_size(character_size)) {
      return 0.0f;
    }

    auto* face = face_as<FT_Face>();

    if (!FT_HAS_KERNING(face)) {
      return 0.0f;
    }

    auto index_left = FT_Get_Char_Index(face, left);
    auto index_right = FT_Get_Char_Index(face, right);

    FT_Vector kerning = {};

    if (auto err = FT_Get_Kerning(face, index_left, index_right, FT_KERNING_UNFITTED, &kerning)) {
      Log::warning("Could not compute kerning: {}", FontManager::error_message(err));
      return 0.0f;
    }

    return ft_convert_metrics(kerning.x);
  }

  float Font::compute_line_spacing(uint32_t character_size)
  {
    if (m_face == nullptr) {
      return 0.0f;
    }

    if (!set_current_character_size(character_size)) {
      return 0.0f;
    }

    return ft_convert_metrics(face_as<FT_Face>()->size->metrics.height);
  }

  bool Font::set_current_character_size(uint32_t character_size)
  {
    if (m_character_size == character_size) {
      return true;
    }

    if (auto err = FT_Set_Pixel_Sizes(face_as<FT_Face>(), 0, character_size)) {
      Log::error("Could not change the font size: {}", FontManager::error_message(err));
      return false;
    }

    m_character_size = character_size;
    return true;
  }

}
