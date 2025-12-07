// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/SpriteGroup.h>

#include <cassert>

#include <gf2/core/Mat3.h>
#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/GpuTexture.h>
#include <gf2/graphics/RawGeometry.h>

namespace gf {

  namespace {

    std::vector<GpuTexture*> load_textures(const SpriteGroupResource& resource, ResourceManager* resource_manager)
    {
      std::vector<GpuTexture*> textures;
      textures.reserve(resource.textures.size());

      for (const std::filesystem::path& texture : resource.textures) {
        textures.push_back(resource_manager->get<GpuTexture>(texture));
      }

      return textures;
    }

  }

  SpriteGroup::SpriteGroup(std::vector<GpuTexture*> textures, const SpriteGroupData& data, RenderManager* render_manager)
  : m_textures(std::move(textures))
  , m_vertices(GpuBufferUsage::Vertex)
  , m_indices(GpuBufferUsage::Index)
  {
    update(data, render_manager);
  }

  SpriteGroup::SpriteGroup(const SpriteGroupResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : SpriteGroup(load_textures(resource, resource_manager), resource.data, render_manager)
  {
  }

  std::vector<RenderGeometry> SpriteGroup::geometry()
  {
    std::vector<RenderGeometry> geometries;

    for (auto& buffer : m_ranges) {
      assert(buffer.texture_index < m_textures.size());

      RenderGeometry geometry;
      geometry.texture = m_textures[buffer.texture_index];
      geometry.vertices = &m_vertices.buffer();
      geometry.indices = &m_indices.buffer();
      geometry.first = buffer.first;
      geometry.size = buffer.size;

      geometries.push_back(geometry);
    }

    return geometries;
  }

  void SpriteGroup::update(const SpriteGroupData& data, RenderManager* render_manager)
  {
    RawSplitGeometry split_geometry;

    for (const auto& sprite : data.sprites) {
      assert(sprite.texture_index < m_textures.size());
      const GpuTexture* texture = m_textures[sprite.texture_index];

      const Vec2F size = texture->size() * sprite.texture_region.size();
      const RectF bounds = RectF::from_size(size);
      const Color color = srgb_to_linear(sprite.color);

      Vertex vertices[4] = {
        { bounds.position_at(Orientation::NorthEast), sprite.texture_region.position_at(Orientation::NorthEast), color },
        { bounds.position_at(Orientation::SouthEast), sprite.texture_region.position_at(Orientation::SouthEast), color },
        { bounds.position_at(Orientation::NorthWest), sprite.texture_region.position_at(Orientation::NorthWest), color },
        { bounds.position_at(Orientation::SouthWest), sprite.texture_region.position_at(Orientation::SouthWest), color },
      };

      const Mat3F matrix = sprite.transform.compute_matrix(bounds);

      for (auto& vertex : vertices) {
        vertex.location = transform_point(matrix, vertex.location);
      }

      split_geometry.add_quad(sprite.texture_index, vertices);
    }

    m_ranges.clear();
    std::size_t first = 0;

    for (auto& [texture_index, geometry] : split_geometry.map) {
      BufferRange range = {};
      range.texture_index = texture_index;
      range.first = first;
      range.size = geometry.indices.size();
      first += range.size;
      m_ranges.push_back(range);
    }

    RawGeometry geometry = split_geometry.merge();
    m_vertices.update(geometry.vertices.data(), geometry.vertices.size(), render_manager);
    m_indices.update(geometry.indices.data(), geometry.indices.size(), render_manager);
  }

}
