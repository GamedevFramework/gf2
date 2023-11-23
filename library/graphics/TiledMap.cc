// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <cstdint>
#include <gf2/graphics/TiledMap.h>
// clang-format on

#include <gf2/core/Property.h>
#include <gf2/core/ResourceBundle.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/Span.h>
#include <gf2/core/TiledMapData.h>
#include <gf2/core/Transform.h>

#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Vertex.h>

namespace gf {
  namespace {

    constexpr int ChunkSize = 128;

    struct Geometry {
      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
    };

    struct SplitGeometry {
      std::map<uint32_t, Geometry> geometry_map;

      void add_quad(uint32_t texture_index, StaticSpan<const Vertex, 4> vertices)
      {
        Geometry& geometry = geometry_map[texture_index];
        auto index = static_cast<uint16_t>(geometry.vertices.size());

        geometry.vertices.insert(geometry.vertices.end(), vertices.begin(), vertices.end());

        // first triangle
        geometry.indices.push_back(index);
        geometry.indices.push_back(index + 1);
        geometry.indices.push_back(index + 2);

        // second triangle
        geometry.indices.push_back(index + 2);
        geometry.indices.push_back(index + 1);
        geometry.indices.push_back(index + 3);
      }

      std::vector<Vertex> merge_vertices() const
      {
        std::vector<Vertex> vertices;

        for (const auto& [texture_index, geometry] : geometry_map) {
          vertices.insert(vertices.end(), geometry.vertices.begin(), geometry.vertices.end());
        }

        return vertices;
      }

      std::vector<uint16_t> merge_indices() const
      {
        std::vector<uint16_t> indices;

        for (const auto& [texture_index, geometry] : geometry_map) {
          indices.insert(indices.end(), geometry.indices.begin(), geometry.indices.end());
        }

        return indices;
      }
    };

  }

  TiledMap::TiledMap(std::vector<const Texture*> textures, const TiledMapData& data, RenderManager* render_manager)
  : m_textures(std::move(textures))
  , m_bounds(RectF::from_size(data.map_size * data.tile_size))
  {
    compute_grid(data);
    compute_tile_layers(data, render_manager);
    compute_object_layers(data, render_manager);
  }

  namespace {

    std::vector<const Texture*> load_resources(const TiledMapResource& resource, ResourceManager* resource_manager)
    {
      std::vector<const Texture*> resources;
      resources.reserve(resource.textures.size());

      for (const auto& texture : resource.textures) {
        resources.push_back(resource_manager->get<Texture>(texture));
      }

      return resources;
    }

  }

  TiledMap::TiledMap(const TiledMapResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : TiledMap(load_resources(resource, resource_manager), resource.data, render_manager)
  {
  }

  TiledMap::TiledMap(const std::filesystem::path& filename, TiledMapContext context)
  : TiledMap(TiledMapResource(filename), context.render_manager, context.resource_manager)
  {
  }

  ResourceBundle TiledMap::bundle(const std::filesystem::path& filename, TiledMapContext context)
  {
    auto textures = TiledMapResource::textures_only(filename);

    return ResourceBundle([textures,context](ResourceBundle* bundle, ResourceManager* manager, ResourceAction action) {
      for (const auto& texture : textures) {
        bundle->handle<Texture>(texture, context.render_manager, manager, action);
      }
    });
  }

  void TiledMap::compute_grid(const TiledMapData& data)
  {
    switch (data.orientation) {
      case GridOrientation::Orthogonal:
        m_grid = AnyGrid::make_orthogonal(data.map_size, data.tile_size);
        break;
      case GridOrientation::Isometric:
        m_grid = AnyGrid::make_isometric(data.map_size, data.tile_size);
        break;
      case GridOrientation::Staggered:
        m_grid = AnyGrid::make_staggered(data.map_size, data.tile_size, data.cell_axis, data.cell_index);
        break;
      case GridOrientation::Hexagonal:
        m_grid = AnyGrid::make_hexagonal(data.map_size, data.tile_size, static_cast<float>(data.hex_side_length), data.cell_axis, data.cell_index);
        break;
      default:
        break;
    }
  }

  void TiledMap::compute_tile_layers(const TiledMapData& data, RenderManager* render_manager)
  {
    const Vec2I chunk_count = { div_ceil(data.map_size.w, ChunkSize), div_ceil(data.map_size.h, ChunkSize) };

    for (const auto& tile_layer_data : data.tile_layers) {
      TileLayer tile_layer;
      tile_layer.chunks = Array2D<LayerBuffers>(chunk_count);

      for (auto base : tile_layer.chunks.position_range()) {
        RectI chunk_rectangle = RectI::from_position_size(base * ChunkSize, vec(ChunkSize, ChunkSize));
        auto maybe_intersection = RectI::from_size(data.map_size).intersection(chunk_rectangle);
        assert(maybe_intersection);
        chunk_rectangle = *maybe_intersection;
        assert(!chunk_rectangle.empty());

        SplitGeometry split_geometry;

        for (auto position : gf::position_range(chunk_rectangle.size())) {
          position += chunk_rectangle.position();

          const TileData tile_data = tile_layer_data.tiles(position);
          const TilesetData* tileset_data = data.tileset_from_gid(tile_data.gid);
          const uint32_t gid = tile_data.gid - tileset_data->first_gid;
          assert(tileset_data != nullptr);

          const Texture* texture = m_textures[tileset_data->texture_index];
          const RectF texture_region = tileset_data->compute_texture_region(gid, texture->size());

          const RectF bounds = m_grid.compute_cell_bounds(position);

          Vertex vertices[4] = {
            { bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast) },
            { bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast) },
            { bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest) },
            { bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest) }
          };

          split_geometry.add_quad(tileset_data->texture_index, vertices);
        }

        LayerBuffers chunk;
        std::size_t first = 0;

        for (auto& [texture_index, geometry] : split_geometry.geometry_map) {
          BufferRange range = {};
          range.texture_index = texture_index;
          range.first = first;
          range.count = geometry.indices.size();
          first += range.count;
          chunk.ranges.push_back(range);
        }

        auto vertices = split_geometry.merge_vertices();
        chunk.vertices = Buffer(BufferType::Device, BufferUsage::Vertex, vertices.data(), vertices.size(), render_manager);
        auto indices = split_geometry.merge_indices();
        chunk.indices = Buffer(BufferType::Device, BufferUsage::Index, indices.data(), indices.size(), render_manager);

        tile_layer.chunks(base) = std::move(chunk);
      }

      m_tile_layers.push_back(std::move(tile_layer));
    }
  }

  void TiledMap::compute_object_layers(const TiledMapData& data, RenderManager* render_manager)
  {
    for (const auto& object_layer_data : data.object_layers) {
      SplitGeometry split_geometry;

      for (const auto& object_data : object_layer_data.objects) {
        if (object_data.type != ObjectType::Tile) {
          continue;
        }

        const TileData tile_data = std::get<TileData>(object_data.feature);
        const TilesetData* tileset_data = data.tileset_from_gid(tile_data.gid);
        const uint32_t gid = tile_data.gid - tileset_data->first_gid;
        assert(tileset_data != nullptr);

        const Texture* texture = m_textures[tileset_data->texture_index];
        const RectF texture_region = tileset_data->compute_texture_region(gid, texture->size());

        const RectF bounds = RectF::from_size(tileset_data->tile_size);

        Vertex vertices[4] = {
          { bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast) },
          { bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast) },
          { bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest) },
          { bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest) }
        };

        const Vec2F origin = data.orientation == GridOrientation::Isometric ? vec(0.5f, 1.0f) : vec(0.0f, 1.0f);
        const Transform transform(object_data.location, origin, degrees_to_radians(object_data.rotation));
        const Mat3F transform_matrix = transform.compute_matrix(bounds);

        for (Vertex& vertex : vertices) {
          vertex.location = transform_point(transform_matrix, vertex.location);
        }

        split_geometry.add_quad(tileset_data->texture_index, vertices);
      }

      LayerBuffers buffers;
      std::size_t first = 0;

      for (auto& [texture_index, geometry] : split_geometry.geometry_map) {
        BufferRange range = {};
        range.texture_index = texture_index;
        range.first = first;
        range.count = geometry.indices.size();
        first += range.count;
        buffers.ranges.push_back(range);
      }

      auto vertices = split_geometry.merge_vertices();
      buffers.vertices = Buffer(BufferType::Device, BufferUsage::Vertex, vertices.data(), vertices.size(), render_manager);
      auto indices = split_geometry.merge_indices();
      buffers.indices = Buffer(BufferType::Device, BufferUsage::Index, indices.data(), indices.size(), render_manager);

      m_object_layers.push_back({ std::move(buffers) });
    }
  }

}
