// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/physics/PhysicsShape.h>
// clang-format on

#include <vector>

#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsWorld.h>

namespace gf {

  PhysicsShape PhysicsShape::make_circle(PhysicsBody* body, float radius, Vec2F offset)
  {
    return { cpCircleShapeNew(body->m_body, radius, cpv(offset.x, offset.y)) };
  }

  PhysicsShape PhysicsShape::make_segment(PhysicsBody* body, Vec2F a, Vec2F b, float radius)
  {
    return { cpSegmentShapeNew(body->m_body, cpv(a.x, a.y), cpv(b.x, b.y), radius) };
  }

  PhysicsShape PhysicsShape::make_segment(PhysicsBody* body, Vec2F a, Vec2F b, float radius, Vec2F prev, Vec2F next)
  {
    auto* shape = cpSegmentShapeNew(body->m_body, cpv(a.x, a.y), cpv(b.x, b.y), radius);
    cpSegmentShapeSetNeighbors(shape, cpv(prev.x, prev.y), cpv(next.x, next.y));
    return { shape };
  }

  PhysicsShape PhysicsShape::make_polygon(PhysicsBody* body, Span<const Vec2F> vertices, Mat3F transform, float radius)
  {
    std::vector<cpVect> new_vertices;
    new_vertices.reserve(vertices.size());

    for (auto v : vertices) {
      new_vertices.emplace_back(cpv(v.x, v.y));
    }

    // assert on t(2, *)?

    auto new_transform = cpTransformNew(transform(0, 0), transform(1, 0), transform(0, 1), transform(1, 1), transform(0, 2), transform(1, 2));
    return cpPolyShapeNew(body->m_body, static_cast<int>(new_vertices.size()), new_vertices.data(), new_transform, radius);
  }

  PhysicsShape PhysicsShape::make_box(PhysicsBody* body, RectF box, float radius)
  {
    auto min = box.min();
    auto max = box.max();
    return { cpBoxShapeNew2(body->m_body, cpBBNew(min.x, min.y, max.x, max.y), radius) };
  }

  PhysicsWorld PhysicsShape::world() const
  {
    return { details::PhysicsExisting, cpShapeGetSpace(m_shape) };
  }

  PhysicsBody PhysicsShape::body() const
  {
    return { details::PhysicsExisting, cpShapeGetBody(m_shape) };
  }

  void PhysicsShape::set_body(PhysicsBody body)
  {
    cpShapeSetBody(m_shape, body.m_body);
  }

  float PhysicsShape::mass()
  {
    return static_cast<float>(cpShapeGetMass(m_shape));
  }

  void PhysicsShape::set_mass(float mass)
  {
    cpShapeSetMass(m_shape, mass);
  }

  float PhysicsShape::density()
  {
    return static_cast<float>(cpShapeGetDensity(m_shape));
  }

  void PhysicsShape::set_density(float density)
  {
    cpShapeSetDensity(m_shape, density);
  }

  float PhysicsShape::moment()
  {
    return static_cast<float>(cpShapeGetMoment(m_shape));
  }

  float PhysicsShape::area()
  {
    return static_cast<float>(cpShapeGetArea(m_shape));
  }

  Vec2F PhysicsShape::center_of_gravity()
  {
    auto cog = cpShapeGetCenterOfGravity(m_shape);
    return gf::vec(cog.x, cog.y);
  }

  RectF PhysicsShape::bounding_box() const
  {
    auto bb = cpShapeGetBB(m_shape);
    return RectF::from_min_max(gf::vec(bb.l, bb.b), gf::vec(bb.r, bb.t));
  }

  bool PhysicsShape::sensor() const
  {
    return cpShapeGetSensor(m_shape) == cpTrue;
  }

  void PhysicsShape::set_sensor(bool sensor)
  {
    cpShapeSetSensor(m_shape, sensor ? cpTrue : cpFalse);
  }

  float PhysicsShape::elasticity() const
  {
    return static_cast<float>(cpShapeGetElasticity(m_shape));
  }

  void PhysicsShape::set_elasticity(float elasticity)
  {
    cpShapeSetElasticity(m_shape, elasticity);
  }

  float PhysicsShape::friction() const
  {
    return static_cast<float>(cpShapeGetFriction(m_shape));
  }

  void PhysicsShape::set_friction(float friction)
  {
    cpShapeSetFriction(m_shape, friction);
  }

  Vec2F PhysicsShape::surface_velocity() const
  {
    auto velocity = cpShapeGetSurfaceVelocity(m_shape);
    return gf::vec(velocity.x, velocity.y);
  }

  void PhysicsShape::set_surface_velocity(Vec2F velocity)
  {
    cpShapeSetSurfaceVelocity(m_shape, cpv(velocity.x, velocity.y));
  }

  uintptr_t PhysicsShape::collision_type() const
  {
    return cpShapeGetCollisionType(m_shape);
  }

  void PhysicsShape::set_collision_type(uintptr_t type)
  {
    cpShapeSetCollisionType(m_shape, type);
  }

  PhysicsShapeFilter PhysicsShape::shape_filter() const
  {
    auto filter = cpShapeGetFilter(m_shape);
    return { filter.group, filter.categories, filter.mask };
  }

  void PhysicsShape::set_shape_filter(PhysicsShapeFilter filter)
  {
    cpShapeSetFilter(m_shape, cpShapeFilterNew(filter.group, filter.categories, filter.mask));
  }

  PhysicsShape::PhysicsShape(cpShape* shape)
  : m_shape(shape)
  {
  }

  PhysicsShape::PhysicsShape(details::PhysicsExistingType existing, cpShape* shape)
  : m_shape(existing, shape)
  {
  }

}
