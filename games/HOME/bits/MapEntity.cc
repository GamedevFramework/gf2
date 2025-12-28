#include "MapEntity.h"

#include <cassert>

#include <algorithm>
#include <iterator>

#include <gf2/core/Geometry.h>
#include <gf2/core/Grids.h>
#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/physics/PhysicsChain.h>
#include <gf2/physics/PhysicsShape.h>

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

      for (const gf::Vec2I position : tile_layer.tiles.position_range()) {
        if (tile_layer.tiles(position).gid != 0) {
          continue;
        }

        std::vector<gf::Vec2I> contour = grid.compute_contour(position);
        std::sort(contour.begin(), contour.end(), vec_compare);

        const std::vector<gf::Vec2I> neighbors = grid.compute_neighbors(position, gf::CellNeighborQuery::Valid);

        for (const gf::Vec2I neighbor : neighbors) {
          if (tile_layer.tiles(neighbor).gid == 0) {
            continue;
          }

          std::vector<gf::Vec2I> neighbor_contour = grid.compute_contour(neighbor);
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

      std::vector<gf::Polyline> lines = gf::compute_lines(segments);
      gf::Log::info("Number of segments: {}", segments.size());
      gf::Log::info("Number of lines: {}", lines.size());
      return lines;
    }

    gf::PhysicsBodyData compute_map_body_data()
    {
      gf::PhysicsBodyData data;
      data.type = gf::PhysicsBodyType::Static;
      return data;
    }

  }

  MapEntity::MapEntity(GameHub* hub, const WorldResources& resources, gf::PhysicsWorld* physics_world)
  : m_map_renderer(resources.map, hub->render_manager(), hub->resource_manager())
  , m_body(physics_world, compute_map_body_data())
  {

    const gf::TiledMap* tiled_map = m_map_renderer.tiled_map();
    assert(tiled_map->orientation == gf::GridOrientation::Staggered);

    const gf::StaggeredGrid grid(tiled_map->map_size, tiled_map->tile_size, tiled_map->cell_axis, tiled_map->cell_index);
    std::vector<gf::Polyline> contour = find_contour(tiled_map->tile_layers.front(), grid);

    for (gf::Polyline& polyline : contour) {
      for (gf::Vec2F& point : polyline.points) {
        point.y -= static_cast<float>(tiled_map->tile_size.h) / 2;
      }

      gf::PhysicsChainData data;
      data.loop = polyline.type == gf::PolylineType::Loop;
      data.points = polyline.points;

      gf::PhysicsChain chain_ccw(&m_body, data);
      m_chains.push_back(std::move(chain_ccw));

      std::reverse(polyline.points.begin(), polyline.points.end());
      data.points = polyline.points;

      gf::PhysicsChain chain_cw(&m_body, data);
      m_chains.push_back(std::move(chain_cw));
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

        gf::PhysicsShapeData data;

        gf::PhysicsShape shape = gf::PhysicsShape::create_circle(&m_body, data, gf::CircF::from_center_radius(location, 150.0f));
        m_shapes.push_back(std::move(shape));
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
