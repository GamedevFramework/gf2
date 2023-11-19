// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_CONSTRAINT_H
#define GF_PHYSICS_CONSTRAINT_H

#include <cstdint>

#include <chipmunk/chipmunk.h>

#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsHandle.h"

namespace gf {
  class PhysicsBody;
  class PhysicsWorld;

  enum class PhysicsConstraintType : uint8_t {
    PinJoint,
    SlideJoint,
    PivotJoint,
    GrooveJoint,
    DampedSpring,
    DampedRotarySpring,
    RotaryLimitJoint,
    RatchetJoint,
    GearJoint,
    SimpleMotor,
  };

  class GF_PHYSICS_API PhysicsConstraint {
  public:
    PhysicsConstraint() = default;

    static PhysicsConstraint make_pin_joint(PhysicsBody body_a, PhysicsBody body_b, Vec2F anchor_a, Vec2F anchor_b);
    static PhysicsConstraint make_slide_joint(PhysicsBody body_a, PhysicsBody body_b, Vec2F anchor_a, Vec2F anchor_b, float min, float max);
    static PhysicsConstraint make_pivot_joint(PhysicsBody body_a, PhysicsBody body_b, Vec2F pivot);
    static PhysicsConstraint make_pivot_joint(PhysicsBody body_a, PhysicsBody body_b, Vec2F anchor_a, Vec2F anchor_b);
    static PhysicsConstraint make_groove_joint(PhysicsBody body_a, PhysicsBody body_b, Vec2F groove_a, Vec2F groove_b, Vec2F anchor_b);
    static PhysicsConstraint make_damped_spring(PhysicsBody body_a, PhysicsBody body_b, Vec2F anchor_a, Vec2F anchor_b, float rest_length, float stiffness, float damping);
    static PhysicsConstraint make_damped_rotary_spring(PhysicsBody body_a, PhysicsBody body_b, float rest_angle, float stiffness, float damping);
    static PhysicsConstraint make_rotary_limit_joint(PhysicsBody body_a, PhysicsBody body_b, float min, float max);
    static PhysicsConstraint make_ratchet_joint(PhysicsBody body_a, PhysicsBody body_b, float phase, float ratchet);
    static PhysicsConstraint make_gear_joint(PhysicsBody body_a, PhysicsBody body_b, float phase, float ratio);
    static PhysicsConstraint make_simple_motor(PhysicsBody body_a, PhysicsBody body_b, float rate);

    PhysicsConstraintType type() const;

    PhysicsWorld world() const;
    PhysicsBody body_a() const;
    PhysicsBody body_b() const;

    float max_force() const;
    void set_max_force(float force);

    float error_bias() const;
    void set_error_bias(float bias);

    float max_bias() const;
    void set_max_bias(float bias);

    bool collide_bodies() const;
    void set_collide_bodies(bool collide);

    // TODO: pre-solve and post-solve

    float impulse();

  private:
    friend class PhysicsBody;
    friend class PhysicsWorld;

    PhysicsConstraint(cpConstraint* constraint);
    PhysicsConstraint(details::PhysicsExistingType existing, cpConstraint* constraint);

    details::PhysicsHandle<cpConstraint, cpConstraintGetUserData, cpConstraintSetUserData, cpConstraintDestroy> m_constraint;
  };

}

#endif // GF_PHYSICS_CONSTRAINT_H
