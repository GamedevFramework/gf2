// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_ELEMENT_CONTAINER_H
#define GF_CONSOLE_ELEMENT_CONTAINER_H

#include <vector>

#include "Console.h"
#include "ConsoleElement.h"
#include "CoreApi.h"

namespace gf {

  class GF_CORE_API ConsoleElementContainer {
  public:

    void add_element(ConsoleElement* element);

    void update(Time time);
    void render(Console& buffer);

  private:
    std::vector<ConsoleElement*> m_elements;
  };

}

#endif // GF_CONSOLE_ELEMENT_CONTAINER_H
