// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ConsoleEntityContainer.h>

namespace gf {

  void ConsoleEntityContainer::add_entity(ConsoleEntity* entity)
  {
    m_entities.push_back(entity);
  }

  void ConsoleEntityContainer::update(Time time)
  {
    for (ConsoleEntity* entity : m_entities) {
      entity->update(time);
    }
  }

  void ConsoleEntityContainer::render(Console& buffer)
  {
    for (ConsoleEntity* entity : m_entities) {
      entity->render(buffer);
    }
  }

}
