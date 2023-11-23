#include "MapEntity.h"

#include <gf2/core/ResourceManager.h>

#include "GameHub.h"

namespace home {

  MapEntity::MapEntity(GameHub* hub, const WorldData& data)
  : m_map(hub->resource_manager()->get<gf::TiledMap>(data.map))
  {
  }

}
