// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/EntityContainer.h>

#include <cassert>

#include <algorithm>

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

    // erase-remove idiom
    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
  }

  void EntityContainer::clear()
  {
    m_entities.clear();
  }

}
