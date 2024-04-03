// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_FONT_FACE_H
#define GF_FONT_FACE_H

#include <cstdint>

#include <filesystem>
#include <type_traits>

#include "Bitmap.h"
#include "CoreApi.h"
#include "Rect.h"
#include "Stream.h"

namespace gf {
  class FontManager;

  struct GF_CORE_API FontGlyph {
    RectF bounds = {};
    Bitmap bitmap;
  };

  class GF_CORE_API FontFace {
  public:
    using Context = FontManager*;

    FontFace(const std::filesystem::path& filename, FontManager* manager);
    FontFace(InputStream& stream, FontManager* manager);
    FontFace(const FontFace&) = delete;
    FontFace(FontFace&& other) noexcept;
    ~FontFace();

    FontFace& operator=(const FontFace&) = delete;
    FontFace& operator=(FontFace&& other) noexcept;

    std::string family_name() const;
    std::string style_name() const;

    FontGlyph create_glyph(uint32_t index);

    float compute_line_height();
    float compute_space_width();

    void* raw_handle()
    {
      return m_face;
    }

    static float size_factor(float character_size);

  private:
    bool set_current_character_size(uint32_t character_size);

    template<typename T>
    auto face_as()
    {
      return static_cast<T>(m_face);
    }

    template<typename T>
    auto face_as() const
    {
      return static_cast<const std::remove_pointer_t<T>*>(m_face);
    }

    FontManager* m_manager;
    void* m_face = nullptr;
  };

}

#endif // GF_FONT_FACE_H
