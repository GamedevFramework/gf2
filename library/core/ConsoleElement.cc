// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ConsoleElement.h>

namespace gf {

  ConsoleElement::~ConsoleElement() = default;

  void ConsoleElement::update()
  {
    // do nothing by default
  }

  void ConsoleElement::render([[maybe_unused]] Console& buffer)
  {
    // do nothing by default
  }

}
