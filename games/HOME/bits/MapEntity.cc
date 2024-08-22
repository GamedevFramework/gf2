#include "MapEntity.h"

#include <cassert>

#include <algorithm>
#include <iterator>

#include <gf2/core/Geometry.h>
#include <gf2/core/Grids.h>
#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/physics/PhysicsShape.h>
#include <gf2/physics/PhysicsShapeEx.h>

#include "GameHub.h"

namespace home {

  namespace {

    bool vec_compare(gf::Vec2I lhs, gf::Vec2I rhs)
    {
      return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
    }

    std::vector<gf::Polyline> find_contour(const gf::MapTileLayer& tile_layer, const gf::StaggeredGrid& grid)
    {
      gf::Log::info("Tile Layer '{}'", tile_layer.layer.name);

      std::vector<gf::SegmentI> segments;

      for (auto position : tile_layer.tiles.position_range()) {
        if (tile_layer.tiles(position).gid != 0) {
          continue;
        }

        auto contour = grid.compute_contour(position);
        std::sort(contour.begin(), contour.end(), vec_compare);

        auto neighbors = grid.compute_neighbors(position, gf::CellNeighborQuery::Valid);

        for (auto neighbor : neighbors) {
          if (tile_layer.tiles(neighbor).gid == 0) {
            continue;
          }

          auto neighbor_contour = grid.compute_contour(neighbor);
          std::sort(neighbor_contour.begin(), neighbor_contour.end(), vec_compare);

          std::vector<gf::Vec2I> intersection;
          std::set_intersection(contour.begin(), contour.end(), neighbor_contour.begin(), neighbor_contour.end(), std::back_inserter(intersection), vec_compare);

          if (intersection.size() == 2) {
            segments.push_back({ intersection[0], intersection[1] });
          } else {
            assert(intersection.empty());
          }
        }
      }

      auto lines = gf::compute_lines(segments);
      gf::Log::info("Number of segments: {}", segments.size());
      gf::Log::info("Number of lines: {}", lines.size());
      return lines;
    }

  }

  MapEntity::MapEntity(GameHub* hub, const WorldData& data, gf::PhysicsWorld* physics_world)
  : m_map_renderer(data.map, hub->render_manager(), hub->resource_manager())
  {

    const gf::TiledMap* tiled_map = m_map_renderer.tiled_map();
    assert(tiled_map->orientation == gf::GridOrientation::Staggered);

    const gf::StaggeredGrid grid(tiled_map->map_size, tiled_map->tile_size, tiled_map->cell_axis, tiled_map->cell_index);
    auto contour = find_contour(tiled_map->tile_layers.front(), grid);

    gf::PhysicsBody body = physics_world->static_body();

    for (auto& polyline : contour) {
      for (auto& point : polyline.points) {
        point.y -= static_cast<float>(tiled_map->tile_size.h) / 2;
      }

      auto shapes = make_polyline_shapes(&body, polyline, 1.0f);

      for (auto& shape : shapes) {
        physics_world->add_shape(std::move(shape));
      }
    }

    for (const auto& object_layer : tiled_map->object_layers) {
      if (object_layer.layer.name != "Trees") {
        continue;
      }

      for (const auto& object : object_layer.objects) {
        if (object.type != gf::MapObjectType::Tile) {
          continue;
        }

        const gf::Vec2F location = object.location + object_layer.layer.offset + gf::vec(384.0f / 2, -384.0f / 2);
        auto shape = gf::PhysicsShape::make_circle(&body, 150.0f, location);
        physics_world->add_shape(std::move(shape));
      }
    }
  }

  void MapEntity::set_hero_location(gf::Vec2F location)
  {
    m_location = location;
  }

  void MapEntity::render(gf::RenderRecorder& recorder)
  {
    auto position = m_map_renderer.compute_position(m_location);
    auto geometries = m_map_renderer.select_geometry(position, "Scenery");

    for (auto& geometry : geometries) {
      gf::RenderObject object = {};
      object.priority = priority();
      object.geometry = geometry;
      object.transform = gf::Identity3F;
      recorder.record(object);
    }
  }

}
