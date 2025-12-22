// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsArbiter.h>

#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsShape.h>
#include <gf2/physics/PhysicsWorld.h>

#include "gf2/physics/PhysicsHandle.h"

namespace gf {

  float PhysicsArbiter::restitution() const
  {
    return static_cast<float>(cpArbiterGetRestitution(m_arbiter));
  }

  void PhysicsArbiter::set_restitution(float restitution)
  {
    cpArbiterSetRestitution(m_arbiter, restitution);
  }

  float PhysicsArbiter::friction() const
  {
    return static_cast<float>(cpArbiterGetFriction(m_arbiter));
  }

  void PhysicsArbiter::set_friction(float friction)
  {
    cpArbiterSetFriction(m_arbiter, friction);
  }

  Vec2F PhysicsArbiter::surface_velocity()
  {
    auto velocity = cpArbiterGetSurfaceVelocity(m_arbiter);
    return gf::vec(velocity.x, velocity.y);
  }

  void PhysicsArbiter::set_surface_velocity(Vec2F velocity)
  {
    cpArbiterSetSurfaceVelocity(m_arbiter, cpv(velocity.x, velocity.y));
  }

  Vec2F PhysicsArbiter::compute_total_impulse() const
  {
    auto impulse = cpArbiterTotalImpulse(m_arbiter);
    return gf::vec(impulse.x, impulse.y);
  }

  float PhysicsArbiter::compute_total_kinetic_energy() const
  {
    return static_cast<float>(cpArbiterTotalKE(m_arbiter));
  }

  bool PhysicsArbiter::ignore()
  {
    return cpArbiterIgnore(m_arbiter) == cpTrue;
  }

  std::tuple<PhysicsShape, PhysicsShape> PhysicsArbiter::shapes() const
  {
    cpShape* a = nullptr;
    cpShape* b = nullptr;
    cpArbiterGetShapes(m_arbiter, &a, &b);
    return std::make_tuple(PhysicsShape(details::PhysicsExisting, a), PhysicsShape(details::PhysicsExisting, b));
  }

  std::tuple<PhysicsBody, PhysicsBody> PhysicsArbiter::bodies() const
  {
    cpBody* a = nullptr;
    cpBody* b = nullptr;
    cpArbiterGetBodies(m_arbiter, &a, &b);
    return std::make_tuple(PhysicsBody(details::PhysicsExisting, a), PhysicsBody(details::PhysicsExisting, b));
  }

  PhysicsContactPointSet PhysicsArbiter::contact_points() const
  {
    PhysicsContactPointSet set = {};
    auto raw = cpArbiterGetContactPointSet(m_arbiter);

    set.size = raw.count;
    set.normal = gf::vec(raw.normal.x, raw.normal.y);
    assert(0 <= raw.count && raw.count < CP_MAX_CONTACTS_PER_ARBITER);

    for (int i = 0; i < raw.count; ++i) {
      // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
      set.points[i].distance = static_cast<float>(raw.points[i].distance);
      auto point_a = raw.points[i].pointA;
      set.points[i].point_a = gf::vec(point_a.x, point_a.y);
      auto point_b = raw.points[i].pointB;
      set.points[i].point_b = gf::vec(point_b.x, point_b.y);
      // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return set;
  }

  void PhysicsArbiter::set_contact_points(const PhysicsContactPointSet& set)
  {
    cpContactPointSet raw = {};
    raw.count = static_cast<int>(set.size);
    raw.normal = cpv(set.normal.x, set.normal.y);

    for (std::size_t i = 0; i < set.size; ++i) {
      // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
      raw.points[i].distance = set.points[i].distance;
      auto point_a = set.points[i].point_a;
      raw.points[i].pointA = cpv(point_a.x, point_a.y);
      auto point_b = set.points[i].point_b;
      raw.points[i].pointB = cpv(point_b.x, point_b.y);
      // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    cpArbiterSetContactPointSet(m_arbiter, &raw);
  }

  bool PhysicsArbiter::first_contact() const
  {
    return cpArbiterIsFirstContact(m_arbiter) == cpTrue;
  }

  bool PhysicsArbiter::removal() const
  {
    return cpArbiterIsRemoval(m_arbiter) == cpTrue;
  }

  int PhysicsArbiter::count() const
  {
    return cpArbiterGetCount(m_arbiter);
  }

  Vec2F PhysicsArbiter::normal() const
  {
    auto normal = cpArbiterGetNormal(m_arbiter);
    return gf::vec(normal.x, normal.y);
  }

  Vec2F PhysicsArbiter::point_a(int i) const
  {
    auto a = cpArbiterGetPointA(m_arbiter, i);
    return gf::vec(a.x, a.y);
  }

  Vec2F PhysicsArbiter::point_b(int i) const
  {
    auto b = cpArbiterGetPointB(m_arbiter, i);
    return gf::vec(b.x, b.y);
  }

  float PhysicsArbiter::depth(int i) const
  {
    return static_cast<float>(cpArbiterGetDepth(m_arbiter, i));
  }

  bool PhysicsArbiter::call_wildcard_begin_a(PhysicsWorld* world)
  {
    return cpArbiterCallWildcardBeginA(m_arbiter, world->m_space) == cpTrue;
  }

  bool PhysicsArbiter::call_wildcard_begin_b(PhysicsWorld* world)
  {
    return cpArbiterCallWildcardBeginB(m_arbiter, world->m_space) == cpTrue;
  }

  bool PhysicsArbiter::call_wildcard_pre_solve_a(PhysicsWorld* world)
  {
    return cpArbiterCallWildcardPreSolveA(m_arbiter, world->m_space) == cpTrue;
  }

  bool PhysicsArbiter::call_wildcard_pre_solve_b(PhysicsWorld* world)
  {
    return cpArbiterCallWildcardPreSolveB(m_arbiter, world->m_space) == cpTrue;
  }

  void PhysicsArbiter::call_wildcard_post_solve_a(PhysicsWorld* world)
  {
    cpArbiterCallWildcardPostSolveA(m_arbiter, world->m_space);
  }

  void PhysicsArbiter::call_wildcard_post_solve_b(PhysicsWorld* world)
  {
    cpArbiterCallWildcardPostSolveB(m_arbiter, world->m_space);
  }

  void PhysicsArbiter::call_wildcard_separate_a(PhysicsWorld* world)
  {
    cpArbiterCallWildcardSeparateA(m_arbiter, world->m_space);
  }

  void PhysicsArbiter::call_wildcard_separate_b(PhysicsWorld* world)
  {
    cpArbiterCallWildcardSeparateB(m_arbiter, world->m_space);
  }

  PhysicsArbiter::PhysicsArbiter(cpArbiter* arbiter)
  : m_arbiter(arbiter)
  {
  }

}
