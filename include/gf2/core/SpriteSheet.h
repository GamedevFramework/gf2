// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SPRITE_SHEET_H
#define GF_SPRITE_SHEET_H

#include <filesystem>
#include <map>
#include <string>

#include "Rect.h"
#include "SpriteData.h"
#include "NinePatchData.h"

namespace gf {

  class SpriteSheet {
  public:
    SpriteSheet(const std::filesystem::path& filename);

    const std::filesystem::path& texture_path() const;

    RectI texture_rectangle(const std::string& name) const;
    RectF texture_region(const std::string& name) const;

    SpriteResource sprite(const std::string& name) const;
    NinePatchResource nine_patch(const std::string& name) const;

  private:
    void load_xml(const std::filesystem::path& filename);

    std::filesystem::path m_texture_path;
    Vec2I m_texture_size;
    std::map<std::string, RectI> m_sprites;
  };

}

#endif // GF_SPRITE_SHEET_H

