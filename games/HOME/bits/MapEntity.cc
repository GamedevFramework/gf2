#include "MapEntity.h"

#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

#include "GameHub.h"

namespace home {

  MapEntity::MapEntity(GameHub* hub, const WorldData& data)
  : m_map_resource(hub->resource_manager()->get<gf::TiledMapResource>(data.map))
  , m_map(hub->resource_manager()->get<gf::TiledMap>(data.map))
  {
  }

  void MapEntity::set_hero_location(gf::Vec2F location)
  {
    m_location = location;
  }

  void MapEntity::render(gf::RenderRecorder& recorder)
  {
    auto position = m_map->compute_position(m_location);
    auto geometries = m_map->select_geometry(position, "");

    for (auto& geometry : geometries) {
      gf::RenderObject object = {};
      object.priority = priority();
      object.geometry = geometry;
      object.transform = gf::Identity3F;
      recorder.record(object);
    }
  }

}
