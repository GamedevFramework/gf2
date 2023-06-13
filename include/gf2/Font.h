// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FONT_H
#define GF_FONT_H

#include <filesystem>

#include "Bitmap.h"
#include "CoreApi.h"
#include "Rect.h"
#include "Ref.h"
#include "Stream.h"

namespace gf {
  class FontManager;

  struct GF_CORE_API FontGlyph {
    RectF bounds = {};
    float advance = 0.0f;
    Bitmap bitmap;
  };

  class GF_CORE_API Font {
  public:
    using Context = Ref<FontManager>;

    Font(const std::filesystem::path& filename, Ref<FontManager> manager);
    Font(InputStream& stream, Ref<FontManager> manager);
    Font(const Font&) = delete;
    Font(Font&& other) noexcept;
    ~Font();

    Font& operator=(const Font&) = delete;
    Font& operator=(Font&& other) noexcept;

    FontGlyph create_glyph(char32_t codepoint, unsigned character_size, float outline_thickness = 0.0f);

    float compute_kerning(char32_t left, char32_t right, unsigned character_size);
    float compute_line_spacing(unsigned character_size);

  private:
    bool set_current_character_size(unsigned character_size);

    template<typename T>
    T face_as()
    {
      return static_cast<T>(m_face);
    }

    Ref<FontManager> m_manager;
    void* m_face = nullptr;
    unsigned m_character_size = 0;
  };

}

#endif // GF_FONT_H
