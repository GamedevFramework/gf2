// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/RichMap.h>

#include <gf2/core/ResourceBundle.h>
#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/GpuTexture.h>

namespace gf {

  RichMap::RichMap(const TiledMap* map, std::vector<GpuTexture*> textures)
  : m_tiled_map(map)
  , m_textures(std::move(textures))
  {
  }

  namespace {

    std::vector<GpuTexture*> load_resources(const TiledMap* resource, ResourceManager* resource_manager)
    {
      std::vector<GpuTexture*> resources;
      resources.reserve(resource->textures.size());

      for (const auto& texture : resource->textures) {
        resources.push_back(resource_manager->get<GpuTexture>(texture));
      }

      return resources;
    }

  }

  RichMap::RichMap(const RichMapResource& resource, ResourceManager* resource_manager)
  : m_tiled_map(resource_manager->get<TiledMap>(resource.filename))
  , m_textures(load_resources(m_tiled_map, resource_manager))
  {
  }

  RichMap::RichMap(const std::filesystem::path& filename, RichMapContext context)
  : m_tiled_map(context.resource_manager->get<TiledMap>(filename))
  , m_textures(load_resources(m_tiled_map, context.resource_manager))
  {
  }

  ResourceBundle RichMap::bundle(const std::filesystem::path& filename, RichMapContext context)
  {
    const TiledMap* map = context.resource_manager->get<TiledMap>(filename);

    return ResourceBundle([map, context](ResourceBundle* bundle, ResourceManager* resource_manager, ResourceAction action) {
      for (const std::filesystem::path& texture : map->textures) {
        bundle->handle<GpuTexture>(texture, context.render_manager, resource_manager, action);
      }
    });
  }

  const TiledMap* RichMap::tiled_map() const
  {
    return m_tiled_map;
  }

  GpuTexture* RichMap::texture(uint32_t i)
  {
    assert(i < m_textures.size());
    return m_textures[i];
  }

}
