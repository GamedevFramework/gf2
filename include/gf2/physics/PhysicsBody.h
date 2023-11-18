// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_BODY_H
#define GF_PHYSICS_BODY_H

#include <functional>
#include <type_traits>

#include <chipmunk/chipmunk.h>

#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsHandle.h"

namespace gf {
  class PhysicsArbiter;
  class PhysicsShape;
  class PhysicsWorld;
  class PhysicsConstraint;

  enum class PhysicsBodyType : std::underlying_type_t<cpBodyType> {
    Dynamic,
    Kinematic,
    Static,
  };

  class GF_PHYSICS_API PhysicsBody {
  public:
    static PhysicsBody make_dynamic(float mass, float moment);
    static PhysicsBody make_kinematic();
    static PhysicsBody make_static();

    void activate();
    void activate_static(PhysicsShape filter);

    void sleep();
    void sleep_with_group(PhysicsBody group);
    bool sleeping() const;

    PhysicsBodyType type();
    void set_type(PhysicsBodyType type);

    PhysicsWorld world() const;

    float mass() const;
    void set_mass(float mass);

    float moment() const;
    void set_moment(float moment);

    Vec2F location() const;
    void set_location(Vec2F location);

    Vec2F center_of_gravity() const;
    void set_center_of_gravity(Vec2F cog);

    Vec2F velocity() const;
    void set_velocity(Vec2F velocity);

    Vec2F force() const;
    void set_force(Vec2F force);

    float angle() const;
    void set_angle(float angle);

    float angular_velocity() const;
    void set_angular_velocity(float velocity);

    float torque() const;
    void set_torque(float torque);

    Vec2F compute_local_to_world(Vec2F point) const;
    Vec2F compute_world_to_local(Vec2F point) const;

    void apply_force_at_world_point(Vec2F force, Vec2F point);
    void apply_force_at_local_point(Vec2F force, Vec2F point);

    void apply_impulse_at_world_point(Vec2F impulse, Vec2F point);
    void apply_impulse_at_local_point(Vec2F impulse, Vec2F point);

    Vec2F velocity_at_world_point(Vec2F point) const;
    Vec2F velocity_at_local_point(Vec2F point) const;

    float kinematic_energy() const;

    void each_shape(std::function<void(const PhysicsBody, PhysicsShape)> func);
    void each_constraint(std::function<void(PhysicsBody, PhysicsConstraint)> func);
    void each_arbiter(std::function<void(PhysicsBody, PhysicsArbiter)> func);

  private:
    friend class PhysicsArbiter;
    friend class PhysicsConstraint;
    friend class PhysicsShape;
    friend class PhysicsWorld;

    static void shape_iterator(cpBody* body, cpShape* shape, void* data);
    static void constraint_iterator(cpBody* body, cpConstraint* constraint, void* data);
    static void arbiter_iterator(cpBody* body, cpArbiter* arbiter, void* data);

    PhysicsBody(cpBody* body);
    PhysicsBody(details::PhysicsExistingType existing, cpBody* body);

    details::PhysicsHandle<cpBody, cpBodyGetUserData, cpBodySetUserData, cpBodyDestroy> m_body;
  };

}

#endif // GF_PHYSICS_BODY_H
