// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_ARBITER_H
#define GF_PHYSICS_ARBITER_H

#include <array>
#include <tuple>

#include <chipmunk/chipmunk.h>

#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"

namespace gf {
  class PhysicsBody;
  class PhysicsShape;
  class PhysicsWorld;

  struct PhysicsContactPoint {
    Vec2F point_a;
    Vec2F point_b;
    float distance;
  };

  struct GF_PHYSICS_API PhysicsContactPointSet {
    std::size_t size;
    Vec2F normal;
    std::array<PhysicsContactPoint, 2> points;
  };

  class GF_PHYSICS_API PhysicsArbiter {
  public:
    PhysicsArbiter(const PhysicsArbiter&) = delete;
    PhysicsArbiter(PhysicsArbiter&&) noexcept = delete;
    ~PhysicsArbiter() = default;

    PhysicsArbiter& operator=(const PhysicsArbiter&) = delete;
    PhysicsArbiter& operator=(PhysicsArbiter&&) noexcept = delete;

    float restitution() const;
    void set_restitution(float restitution);

    float friction() const;
    void set_friction(float friction);

    Vec2F surface_velocity();
    void set_surface_velocity(Vec2F velocity);

    Vec2F compute_total_impulse() const;
    float compute_total_kinetic_energy() const;

    bool ignore();

    std::tuple<PhysicsShape, PhysicsShape> shapes() const;
    std::tuple<PhysicsBody, PhysicsBody> bodies() const;

    PhysicsContactPointSet contact_points() const;
    void set_contact_points(const PhysicsContactPointSet& set);

    bool first_contact() const;
    bool removal() const;

    int count() const;
    Vec2F normal() const;
    Vec2F point_a(int i) const;
    Vec2F point_b(int i) const;
    float depth(int i) const;

    bool call_wildcard_begin_a(PhysicsWorld world);
    bool call_wildcard_begin_b(PhysicsWorld world);

    bool call_wildcard_pre_solve_a(PhysicsWorld world);
    bool call_wildcard_pre_solve_b(PhysicsWorld world);

    void call_wildcard_post_solve_a(PhysicsWorld world);
    void call_wildcard_post_solve_b(PhysicsWorld world);

    void call_wildcard_separate_a(PhysicsWorld world);
    void call_wildcard_separate_b(PhysicsWorld world);

  private:
    friend class PhysicsBody;
    friend class PhysicsWorld;

    PhysicsArbiter(cpArbiter* arbiter);

    cpArbiter* m_arbiter = nullptr;
  };

}

#endif // GF_PHYSICS_ARBITER_H
