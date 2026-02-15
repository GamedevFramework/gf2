// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CONSOLE_ENTITY_H
#define GF_CONSOLE_ENTITY_H

#include "Console.h"
#include "CoreApi.h"
#include "Time.h"

namespace gf {

  class GF_CORE_API ConsoleEntity {
  public:
    ConsoleEntity() = default;
    ConsoleEntity(const ConsoleEntity&) = delete;
    ConsoleEntity(ConsoleEntity&&) noexcept = default;
    virtual ~ConsoleEntity();

    ConsoleEntity& operator=(const ConsoleEntity&) = delete;
    ConsoleEntity& operator=(ConsoleEntity&&) noexcept = default;

    virtual void update(Time time);
    virtual void render(Console& buffer);
  };

}

#endif // GF_CONSOLE_ENTITY_H
