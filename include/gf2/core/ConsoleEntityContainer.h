// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_ELEMENT_CONTAINER_H
#define GF_CONSOLE_ELEMENT_CONTAINER_H

#include <vector>

#include "Console.h"
#include "ConsoleEntity.h"
#include "CoreApi.h"

namespace gf {

  class GF_CORE_API ConsoleEntityContainer {
  public:

    void add_entity(ConsoleEntity* entity);

    void update(Time time);
    void render(Console& buffer);

  private:
    std::vector<ConsoleEntity*> m_entities;
  };

}

#endif // GF_CONSOLE_ELEMENT_CONTAINER_H
