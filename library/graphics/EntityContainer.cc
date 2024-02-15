// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/EntityContainer.h>

#include <cassert>

#include <gf2/graphics/Entity.h>

namespace gf {

  void EntityContainer::update(Time time)
  {
    for (auto* entity : m_entities) {
      entity->update(time);
    }
  }

  void EntityContainer::render(RenderRecorder& recorder)
  {
    for (auto* entity : m_entities) {
      entity->render(recorder);
    }
  }

  bool EntityContainer::empty() const
  {
    return m_entities.empty();
  }

  void EntityContainer::add_entity(Entity* entity)
  {
    assert(entity);
    m_entities.push_back(entity);
  }

  void EntityContainer::clear()
  {
    m_entities.clear();
  }

}
