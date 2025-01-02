// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_ELEMENT_H
#define GF_CONSOLE_ELEMENT_H

#include "CoreApi.h"

namespace gf {
  class Console;

  class GF_CORE_API ConsoleElement {
  public:
    ConsoleElement() = default;
    ConsoleElement(const ConsoleElement&) = delete;
    ConsoleElement(ConsoleElement&&) noexcept = default;
    virtual ~ConsoleElement();

    ConsoleElement& operator=(const ConsoleElement&) = delete;
    ConsoleElement& operator=(ConsoleElement&&) noexcept = default;

    virtual void update();
    virtual void render(Console& buffer);
  };

}

#endif // GF_CONSOLE_ELEMENT_H
