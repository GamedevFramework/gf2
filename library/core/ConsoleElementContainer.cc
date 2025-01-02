// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ConsoleElementContainer.h>

namespace gf {

  void ConsoleElementContainer::add_element(ConsoleElement* element)
  {
    m_elements.push_back(element);
  }

  void ConsoleElementContainer::update(Time time)
  {
    for (ConsoleElement* element : m_elements) {
      element->update(time);
    }
  }

  void ConsoleElementContainer::render(Console& buffer)
  {
    for (ConsoleElement* element : m_elements) {
      element->render(buffer);
    }
  }

}
