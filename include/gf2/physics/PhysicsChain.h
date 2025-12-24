// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_CHAIN_H
#define GF_PHYSICS_CHAIN_H

#include <vector>

#include <box2d/box2d.h>

#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsId.h"
#include "PhysicsFilter.h"
#include "PhysicsOwner.h"
#include "PhysicsSurfaceMaterial.h"

namespace gf {
  class PhysicsBody;
  class PhysicsShape;
  class PhysicsWorld;

  struct GF_PHYSICS_API PhysicsChainData {
    Span<const Vec2F> points;
    PhysicsSurfaceMaterial material;
    PhysicsFilter filter;
    bool loop = false;
    bool enable_sensor_events = false;
    void* user_data = nullptr;
  };

  class GF_PHYSICS_API PhysicsChain {
  public:
    PhysicsChain() = default;
    PhysicsChain(PhysicsBody* body, const PhysicsChainData& data);
    PhysicsChain(const PhysicsChain&) = delete;
    PhysicsChain(PhysicsChain&& other) noexcept;
    ~PhysicsChain();

    PhysicsChain& operator=(const PhysicsChain&) = delete;
    PhysicsChain& operator=(PhysicsChain&& other) noexcept;

    PhysicsChainId id() const;
    static PhysicsChain from_id(PhysicsChainId id);

    PhysicsWorld world() const;

    bool valid() const;

    std::vector<PhysicsShape> segments() const;

    PhysicsSurfaceMaterial surface_material() const;
    void set_surface_materiel(const PhysicsSurfaceMaterial& material);

  private:
    constexpr PhysicsChain(details::PhysicsOwner owner, b2ChainId id)
    : m_owner(owner)
    , m_id(id)
    {
    }

    details::PhysicsOwner m_owner = details::PhysicsOwner::None;
    b2ChainId m_id = {};
  };

}

#endif // GF_PHYSICS_CHAIN_H
