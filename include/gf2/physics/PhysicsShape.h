// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_SHAPE_H
#define GF_PHYSICS_SHAPE_H

#include <chipmunk/chipmunk.h>

#include <gf2/core/Rect.h>
#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsHandle.h"

#include "gf2/core/Mat3.h"

namespace gf {
  class PhysicsBody;
  class PhysicsWorld;

  struct GF_PHYSICS_API PhysicsShapeFilter {
    uintptr_t group = 0;
    unsigned categories = 0;
    unsigned mask = 0;
  };

  constexpr PhysicsShapeFilter ShapeFilterAll = { 0, ~0u, ~0u };
  constexpr PhysicsShapeFilter ShapeFilterNone = { 0, 0u, 0u };

  class GF_PHYSICS_API PhysicsShape {
  public:
    PhysicsShape() = default;

    static PhysicsShape make_circle(PhysicsBody* body, float radius, Vec2F offset);
    static PhysicsShape make_segment(PhysicsBody* body, Vec2F a, Vec2F b, float radius);
    static PhysicsShape make_segment(PhysicsBody* body, Vec2F a, Vec2F b, float radius, Vec2F prev, Vec2F next);
    static PhysicsShape make_polygon(PhysicsBody* body, Span<const Vec2F> vertices, Mat3F transform, float radius);
    static PhysicsShape make_box(PhysicsBody* body, RectF box, float radius);

    PhysicsWorld world() const;

    PhysicsBody body() const;
    void set_body(PhysicsBody body);

    float mass();
    void set_mass(float mass);

    float density();
    void set_density(float density);

    float moment();
    float area();
    Vec2F center_of_gravity();

    RectF bounding_box() const;

    bool sensor() const;
    void set_sensor(bool sensor);

    float elasticity() const;
    void set_elasticity(float elasticity);

    float friction() const;
    void set_friction(float friction);

    Vec2F surface_velocity() const;
    void set_surface_velocity(Vec2F velocity);

    uintptr_t collision_type() const;
    void set_collision_type(uintptr_t type);

    PhysicsShapeFilter shape_filter() const;
    void set_shape_filter(PhysicsShapeFilter filter);

    using Handle = details::PhysicsHandle<cpShape, cpShapeGetUserData, cpShapeSetUserData, cpShapeFree>;

  private:
    friend class PhysicsArbiter;
    friend class PhysicsBody;
    friend class PhysicsWorld;

    PhysicsShape(cpShape* shape);
    PhysicsShape(details::PhysicsExistingType existing, cpShape* shape);

    Handle m_shape;
  };

  GF_PHYSICS_API float compute_moment_for_circle(float m, float r1, float r2, Vec2F offset);
  GF_PHYSICS_API float compute_moment_for_segment(float m, Vec2F a, Vec2F b, float radius);
  GF_PHYSICS_API float compute_moment_for_polygon(float m, Span<const Vec2F> vertices, Vec2F offset, float radius);
  GF_PHYSICS_API float compute_moment_for_box(float m, RectF box);

}

#endif // GF_PHYSICS_SHAPE_H
