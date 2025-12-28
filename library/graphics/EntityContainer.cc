// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/EntityContainer.h>

#include <cassert>

#include <gf2/graphics/Entity.h>

namespace gf {

  void EntityContainer::update(Time time)
  {
    for (Entity* entity : m_entities) {
      entity->update(time);
    }
  }

  void EntityContainer::render(RenderRecorder& recorder)
  {
    for (Entity* entity : m_entities) {
      entity->render(recorder);
    }
  }

  bool EntityContainer::empty() const
  {
    return m_entities.empty();
  }

  std::size_t EntityContainer::size() const
  {
    return m_entities.size();
  }

  void EntityContainer::add_entity(Entity* entity)
  {
    if (entity == nullptr) {
      return;
    }

    m_entities.push_back(entity);
  }

  void EntityContainer::remove_entity(Entity* entity)
  {
    if (entity == nullptr) {
      return;
    }

    std::erase(m_entities, entity);
  }

  void EntityContainer::clear()
  {
    m_entities.clear();
  }

}
