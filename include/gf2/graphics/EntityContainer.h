// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ENTITY_CONTAINER_H
#define GF_ENTITY_CONTAINER_H

#include <vector>

#include <gf2/core/Time.h>

#include "GraphicsApi.h"

namespace gf {
  class Entity;
  class RenderRecorder;

  class GF_GRAPHICS_API EntityContainer {
  public:
    void update(Time time);
    void render(RenderRecorder& recorder);

    bool empty() const;
    std::size_t size() const;
    void add_entity(Entity* entity);
    void remove_entity(Entity* entity);
    void clear();

  private:
    std::vector<Entity*> m_entities;
  };

}

#endif // GF_ENTITY_CONTAINER_H
