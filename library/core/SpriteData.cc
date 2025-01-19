// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/SpriteData.h>

#include <gf2/core/Range.h>

namespace gf {

  uint32_t SpriteGroupResource::add_texture(const std::filesystem::path& texture_path)
  {
    for (auto [index, known_texture_path] : enumerate(textures)) {
      if (texture_path == known_texture_path) {
        return uint32_t(index);
      }
    }

    auto index = uint32_t(textures.size());
    textures.push_back(texture_path);
    return index;
  }

  void SpriteGroupResource::add_sprite(const SpriteResource& resource, const Transform& transform)
  {
    IndexedSpriteData sprite_data = {};
    sprite_data.texture_index = add_texture(resource.texture);
    sprite_data.texture_region = resource.data.texture_region;
    sprite_data.color = resource.data.color;
    sprite_data.transform = transform;

    data.sprites.push_back(sprite_data);
  }

}
