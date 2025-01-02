// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/SpriteSheet.h>

#include <fstream>

#include <pugixml.hpp>

#include <gf2/core/Image.h>
#include <gf2/core/Log.h>

namespace gf {

  SpriteSheet::SpriteSheet(const std::filesystem::path& filename)
  : m_texture_size(0, 0)
  {
    load_xml(filename);
  }

  const std::filesystem::path& SpriteSheet::texture_path() const
  {
    return m_texture_path;
  }

  RectI SpriteSheet::texture_rectangle(const std::string& name) const
  {
    if (auto iterator = m_sprites.find(name); iterator != m_sprites.end()) {
      return iterator->second;
    }

    return RectI::from_size({ 1, 1 });
  }

  RectF SpriteSheet::texture_region(const std::string& name) const
  {
    const RectI rectangle = texture_rectangle(name);
    const Vec2F texture_size = m_texture_size;
    return RectF::from_position_size(rectangle.position() / texture_size, rectangle.size() / texture_size);
  }

  SpriteResource SpriteSheet::sprite(const std::string& name) const
  {
    SpriteResource resource;
    resource.data.texture_region = texture_region(name);
    resource.texture = m_texture_path;
    return resource;
  }

  NinePatchResource SpriteSheet::nine_patch(const std::string& name) const
  {
    NinePatchResource resource;
    resource.data.texture_region = texture_region(name);
    resource.texture = m_texture_path;
    return resource;
  }

  void SpriteSheet::load_xml(const std::filesystem::path& filename)
  {
    std::ifstream file(filename);

    if (!file) {
      Log::fatal("Unknown sprite sheet file: {}", filename.string());
    }

    pugi::xml_document doc;
    const pugi::xml_parse_result result = doc.load(file);

    if (!result) {
      Log::fatal("Could not load sprite sheet: {}. Reason: {}", filename.string(), result.description());
    }

    const pugi::xml_node root = doc.child("TextureAtlas");

    if (!root) {
      Log::fatal("Sprite sheet is not in the right format: {}", filename.string());
    }

    const std::filesystem::path texture_path = root.attribute("imagePath").as_string("");

    if (texture_path.empty()) {
      Log::fatal("Image path is not set in sprite sheet: {}", filename.string());
    }

    m_texture_path = filename.parent_path() / texture_path;
    m_texture_size = image_size(m_texture_path);

    for (const pugi::xml_node sub : root.children("SubTexture")) {
      assert(sub.attribute("name"));
      std::string name = sub.attribute("name").value();

      Vec2I position;
      assert(sub.attribute("x"));
      position.x = sub.attribute("x").as_int();
      assert(sub.attribute("y"));
      position.y = sub.attribute("y").as_int();

      Vec2I size;
      assert(sub.attribute("width"));
      size.w = sub.attribute("width").as_int();
      assert(sub.attribute("height"));
      size.h = sub.attribute("height").as_int();

      m_sprites.emplace(std::move(name), RectI::from_position_size(position, size));
    }
  }

}
