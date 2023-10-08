// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ENTITY_H
#define GF_ENTITY_H

#include <gf2/core/Time.h>

#include "GraphicsApi.h"

namespace gf {
  class RenderRecorder;

  class GF_GRAPHICS_API Entity {
  public:
    Entity(int32_t priority = 0);
    Entity(const Entity&) = delete;
    Entity(Entity&&) noexcept = default;
    virtual ~Entity();

    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) noexcept = default;

    virtual void update(Time time);
    virtual void render(RenderRecorder& recorder);

    int32_t priority() const
    {
      return m_priority;
    }

    void set_priority(int32_t priority)
    {
      m_priority = priority;
    }

  private:
    int32_t m_priority = 0;
  };

}

#endif // GF_ENTITY_H
