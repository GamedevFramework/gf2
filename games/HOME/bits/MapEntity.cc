#include "MapEntity.h"

#include <gf2/core/ResourceManager.h>

#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>

#include "GameHub.h"

namespace home {

  MapEntity::MapEntity(GameHub* hub, const WorldData& data)
  : m_map(hub->resource_manager()->get<gf::TiledMap>(data.map))
  {
  }

  void MapEntity::render(gf::RenderRecorder& recorder)
  {
    auto geometries = m_map->select_geometry({ 0, 0 }, "");

    for (auto& geometry : geometries) {
      gf::RenderObject object = {};
      object.priority = priority();
      object.geometry = geometry;
      object.transform = gf::Identity3F;
      recorder.record(object);
    }
  }
}
