// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ConsoleEntity.h>

namespace gf {

  ConsoleEntity::~ConsoleEntity() = default;

  void ConsoleEntity::update([[maybe_unused]] Time time)
  {
    // do nothing by default
  }

  void ConsoleEntity::render([[maybe_unused]] Console& buffer)
  {
    // do nothing by default
  }

}
