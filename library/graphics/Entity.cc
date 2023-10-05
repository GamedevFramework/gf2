// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Entity.h>
// clang-format on

namespace gf {

  Entity::Entity(int32_t priority)
  : m_priority(priority)
  {
  }

  Entity::~Entity() = default;

  void Entity::update([[maybe_unused]] Time time)
  {
    // do nothing by default
  }

  void Entity::render([[maybe_unused]] RenderRecorder& recorder)
  {
    // do nothing by default
  }

}
