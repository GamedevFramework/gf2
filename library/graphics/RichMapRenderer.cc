// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/RichMapRenderer.h>

#include <cstdint>

#include <gf2/core/Property.h>
#include <gf2/core/ResourceBundle.h>
#include <gf2/core/ResourceManager.h>
#include <gf2/core/Span.h>
#include <gf2/core/StringUtils.h>
#include <gf2/core/TiledMap.h>
#include <gf2/core/Transform.h>

#include <gf2/graphics/RawGeometry.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/Texture.h>
#include <gf2/graphics/Vertex.h>

namespace gf {
  namespace {

    constexpr int ChunkSize = 128;

    struct SplitGeometry {
      std::map<uint32_t, RawGeometry> geometry_map;

      void add_quad(uint32_t texture_index, StaticSpan<const Vertex, 4> vertices)
      {
        auto& geometry = geometry_map[texture_index];
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

    AnyGrid compute_grid(const TiledMap* tiled_map)
    {
      switch (tiled_map->orientation) {
        case GridOrientation::Orthogonal:
          return AnyGrid::make_orthogonal(tiled_map->map_size, tiled_map->tile_size);
        case GridOrientation::Isometric:
          return AnyGrid::make_isometric(tiled_map->map_size, tiled_map->tile_size);
        case GridOrientation::Staggered:
          return AnyGrid::make_staggered(tiled_map->map_size, tiled_map->tile_size, tiled_map->cell_axis, tiled_map->cell_index);
        case GridOrientation::Hexagonal:
          return AnyGrid::make_hexagonal(tiled_map->map_size, tiled_map->tile_size, tiled_map->hex_side_length, tiled_map->cell_axis, tiled_map->cell_index);
        default:
          break;
      }

      return {};
    }

    void flip_vertices(StaticSpan<Vertex, 4> vertices, Flags<CellFlip> flip)
    {
      if (flip.test(CellFlip::Diagonally)) {
        std::swap(vertices[0].tex_coords, vertices[3].tex_coords);
      }

      if (flip.test(CellFlip::Horizontally)) {
        std::swap(vertices[0].tex_coords, vertices[2].tex_coords);
        std::swap(vertices[1].tex_coords, vertices[3].tex_coords);
      }

      if (flip.test(CellFlip::Vertically)) {
        std::swap(vertices[0].tex_coords, vertices[1].tex_coords);
        std::swap(vertices[2].tex_coords, vertices[3].tex_coords);
      }
    }

  }

  RichMapRenderer::RichMapRenderer(const RichMap* map, RenderManager* render_manager)
  : m_map(map)
  , m_grid(compute_grid(m_map->tiled_map()))
  {
    compute_tile_layers(render_manager);
    compute_object_layers(render_manager);
  }

  RichMapRenderer::RichMapRenderer(const RichMapResource& resource, RenderManager* render_manager, ResourceManager* resource_manager)
  : RichMapRenderer(resource_manager->get<RichMap>(resource.filename), render_manager)
  {
  }

  std::vector<RenderGeometry> RichMapRenderer::select_geometry(Vec2I position, std::string_view path, Flags<RichMapQuery> query)
  {
    const auto& structure = tiled_map()->compute_structure(path);
    return select_geometry(position, structure, query);
  }

  std::vector<RenderGeometry> RichMapRenderer::select_geometry(std::string_view path, Flags<RichMapQuery> query)
  {
    const auto& structure = tiled_map()->compute_structure(path);
    return select_geometry(structure, query);
  }

  std::vector<RenderGeometry> RichMapRenderer::select_geometry(Vec2I position, Span<const MapLayerStructure> structure, Flags<RichMapQuery> query)
  {
    std::vector<RenderGeometry> geometries;
    const RectI neighbors = RectI::from_center_size(position / ChunkSize, { 3, 3 });
    compute_geometries(neighbors, query, structure, geometries);
    return geometries;
  }

  std::vector<RenderGeometry> RichMapRenderer::select_geometry(Span<const MapLayerStructure> structure, Flags<RichMapQuery> query)
  {
    RectI neighbors = RectI::from_size({ 1, 1 });

    if (!m_tile_layers.empty()) {
      neighbors = RectI::from_size(m_tile_layers.front().chunks.size());
    }

    std::vector<RenderGeometry> geometries;
    compute_geometries(neighbors, query, structure, geometries);
    return geometries;
  }

  Vec2I RichMapRenderer::compute_position(Vec2F location) const
  {
    return m_grid.compute_position(location);
  }

  // useful documentation: http://eishiya.com/articles/tiled/#rendering-maps
  void RichMapRenderer::compute_tile_layers(RenderManager* render_manager)
  {
    const TiledMap* map = tiled_map();

    const Vec2I chunk_count = { div_ceil(map->map_size.w, ChunkSize), div_ceil(map->map_size.h, ChunkSize) };

    for (const auto& raw_tile_layer : map->tile_layers) {
      TileLayer tile_layer;
      tile_layer.chunks = Array2D<LayerBuffers>(chunk_count);

      for (auto base : tile_layer.chunks.position_range()) {
        RectI chunk_rectangle = RectI::from_position_size(base * ChunkSize, vec(ChunkSize, ChunkSize));
        auto maybe_intersection = RectI::from_size(map->map_size).intersection(chunk_rectangle);
        assert(maybe_intersection);
        chunk_rectangle = *maybe_intersection;
        assert(!chunk_rectangle.empty());

        SplitGeometry split_geometry;

        for (auto position : gf::position_range(chunk_rectangle.size())) {
          position += chunk_rectangle.position();

          const MapTile tile = raw_tile_layer.tiles(position);

          if (tile.gid == 0) {
            continue;
          }

          const MapTileset* tileset = map->tileset_from_gid(tile.gid);
          assert(tileset != nullptr);
          const uint32_t gid = tile.gid - tileset->first_gid;

          const Texture* texture = rich_map()->texture(tileset->texture_index);
          const RectF texture_region = tileset->compute_texture_region(gid, texture->size());

          RectI bounds = m_grid.compute_cell_bounds(position);
          bounds.offset.y += map->tile_size.h - tileset->tile_size.h;
          bounds.offset += raw_tile_layer.layer.offset;
          bounds.offset += tileset->offset;
          bounds.extent = tileset->tile_size;

          Vertex vertices[4] = {
            { bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast) },
            { bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast) },
            { bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest) },
            { bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest) }
          };

          flip_vertices(vertices, tile.flip);

          split_geometry.add_quad(tileset->texture_index, vertices);
        }

        LayerBuffers chunk;
        std::size_t first = 0;

        for (auto& [texture_index, geometry] : split_geometry.geometry_map) {
          BufferRange range = {};
          range.texture_index = texture_index;
          range.first = first;
          range.size = geometry.indices.size();
          first += range.size;
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

  void RichMapRenderer::compute_object_layers(RenderManager* render_manager)
  {
    const TiledMap* map = tiled_map();

    for (const auto& raw_object_layer : map->object_layers) {
      SplitGeometry split_geometry;

      for (const auto& object : raw_object_layer.objects) {
        if (object.type != MapObjectType::Tile) {
          continue;
        }

        const MapTile tile = std::get<MapTile>(object.feature);
        const MapTileset* tileset = map->tileset_from_gid(tile.gid);
        assert(tileset != nullptr);
        const uint32_t gid = tile.gid - tileset->first_gid;

        const Texture* texture = rich_map()->texture(tileset->texture_index);
        const RectF texture_region = tileset->compute_texture_region(gid, texture->size());

        const RectF bounds = RectF::from_size(tileset->tile_size);

        Vertex vertices[4] = {
          { bounds.position_at(Orientation::NorthEast), texture_region.position_at(Orientation::NorthEast) },
          { bounds.position_at(Orientation::SouthEast), texture_region.position_at(Orientation::SouthEast) },
          { bounds.position_at(Orientation::NorthWest), texture_region.position_at(Orientation::NorthWest) },
          { bounds.position_at(Orientation::SouthWest), texture_region.position_at(Orientation::SouthWest) }
        };

        const Vec2F origin = map->orientation == GridOrientation::Isometric ? vec(0.5f, 1.0f) : vec(0.0f, 1.0f);
        const Transform transform(object.location + raw_object_layer.layer.offset, origin, degrees_to_radians(object.rotation));
        const Mat3F transform_matrix = transform.compute_matrix(bounds);

        for (Vertex& vertex : vertices) {
          vertex.location = transform_point(transform_matrix, vertex.location);
        }

        split_geometry.add_quad(tileset->texture_index, vertices);
      }

      LayerBuffers buffers;
      std::size_t first = 0;

      for (auto& [texture_index, geometry] : split_geometry.geometry_map) {
        BufferRange range = {};
        range.texture_index = texture_index;
        range.first = first;
        range.size = geometry.indices.size();
        first += range.size;
        buffers.ranges.push_back(range);
      }

      auto vertices = split_geometry.merge_vertices();
      buffers.vertices = Buffer(BufferType::Device, BufferUsage::Vertex, vertices.data(), vertices.size(), render_manager);
      auto indices = split_geometry.merge_indices();
      buffers.indices = Buffer(BufferType::Device, BufferUsage::Index, indices.data(), indices.size(), render_manager);

      m_object_layers.push_back({ std::move(buffers) });
    }
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  void RichMapRenderer::compute_geometries(RectI view, Flags<RichMapQuery> query, Span<const MapLayerStructure> structure, std::vector<RenderGeometry>& geometries) const
  {
    const TiledMap* map = tiled_map();

    for (const auto& layer : structure) {
      switch (layer.type) {
        case MapLayerType::Group:
          if (query.test(RichMapQuery::Recursive)) {
            const std::vector<MapLayerStructure>& sub_structure = map->group_layers[layer.layer_index].sub_layers;
            compute_geometries(view, query, sub_structure, geometries);
          }
          break;

        case MapLayerType::Tile:
          if (query.test(RichMapQuery::Tile)) {
            compute_tile_geometry(view, m_tile_layers[layer.layer_index], geometries);
          }
          break;

        case MapLayerType::Object:
          if (query.test(RichMapQuery::Object)) {
            compute_object_geometry(m_object_layers[layer.layer_index], geometries);
          }
          break;
      }
    }
  }

  void RichMapRenderer::compute_tile_geometry(RectI view, const TileLayer& tile_layer, std::vector<RenderGeometry>& geometries) const
  {
    if (auto maybe_view = view.intersection(RectI::from_size(tile_layer.chunks.size()))) {
      for (auto xy : gf::position_range(maybe_view->size())) {
        xy += maybe_view->position();
        assert(tile_layer.chunks.valid(xy));
        const auto& chunk = tile_layer.chunks(xy);

        if (chunk.vertices.empty() || chunk.indices.empty()) {
          continue;
        }

        RenderGeometry geometry = {};
        geometry.pipeline = RenderPipelineType::Default;
        geometry.vertices = &chunk.vertices;
        geometry.indices = &chunk.indices;

        for (auto range : chunk.ranges) {
          geometry.texture = rich_map()->texture(range.texture_index);
          geometry.first = range.first;
          geometry.size = range.size;

          geometries.push_back(geometry);
        }
      }
    }
  }

  void RichMapRenderer::compute_object_geometry(const ObjectLayer& object_layer, std::vector<RenderGeometry>& geometries) const
  {
    RenderGeometry geometry = {};
    geometry.pipeline = RenderPipelineType::Default;
    geometry.vertices = &object_layer.buffers.vertices;
    geometry.indices = &object_layer.buffers.indices;

    for (auto range : object_layer.buffers.ranges) {
      geometry.texture = rich_map()->texture(range.texture_index);
      geometry.first = range.first;
      geometry.size = range.size;

      geometries.push_back(geometry);
    }
  }

}
