// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/physics/PhysicsConstraint.h>

#include <gf2/core/Log.h>

#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsWorld.h>

namespace gf {

  PhysicsConstraint PhysicsConstraint::make_pin_joint(PhysicsBody* body_a, PhysicsBody* body_b, Vec2F anchor_a, Vec2F anchor_b)
  {
    return { cpPinJointNew(body_a->m_body, body_b->m_body, cpv(anchor_a.x, anchor_a.y), cpv(anchor_b.x, anchor_b.y)) };
  }

  PhysicsConstraint PhysicsConstraint::make_slide_joint(PhysicsBody* body_a, PhysicsBody* body_b, Vec2F anchor_a, Vec2F anchor_b, float min, float max)
  {
    return { cpSlideJointNew(body_a->m_body, body_b->m_body, cpv(anchor_a.x, anchor_a.y), cpv(anchor_b.x, anchor_b.y), min, max) };
  }

  PhysicsConstraint PhysicsConstraint::make_pivot_joint(PhysicsBody* body_a, PhysicsBody* body_b, Vec2F pivot)
  {
    return { cpPivotJointNew(body_a->m_body, body_b->m_body, cpv(pivot.x, pivot.y)) };
  }

  PhysicsConstraint PhysicsConstraint::make_pivot_joint(PhysicsBody* body_a, PhysicsBody* body_b, Vec2F anchor_a, Vec2F anchor_b)
  {
    return { cpPivotJointNew2(body_a->m_body, body_b->m_body, cpv(anchor_a.x, anchor_a.y), cpv(anchor_b.x, anchor_b.y)) };
  }

  PhysicsConstraint PhysicsConstraint::make_groove_joint(PhysicsBody* body_a, PhysicsBody* body_b, Vec2F groove_a, Vec2F groove_b, Vec2F anchor_b)
  {
    return { cpGrooveJointNew(body_a->m_body, body_b->m_body, cpv(groove_a.x, groove_a.y), cpv(groove_b.x, groove_b.y), cpv(anchor_b.x, anchor_b.y)) };
  }

  PhysicsConstraint PhysicsConstraint::make_damped_spring(PhysicsBody* body_a, PhysicsBody* body_b, Vec2F anchor_a, Vec2F anchor_b, float rest_length, float stiffness, float damping)
  {
    return { cpDampedSpringNew(body_a->m_body, body_b->m_body, cpv(anchor_a.x, anchor_a.y), cpv(anchor_b.x, anchor_b.y), rest_length, stiffness, damping) };
  }

  PhysicsConstraint PhysicsConstraint::make_damped_rotary_spring(PhysicsBody* body_a, PhysicsBody* body_b, float rest_angle, float stiffness, float damping)
  {
    return { cpDampedRotarySpringNew(body_a->m_body, body_b->m_body, rest_angle, stiffness, damping) };
  }

  PhysicsConstraint PhysicsConstraint::make_rotary_limit_joint(PhysicsBody* body_a, PhysicsBody* body_b, float min, float max)
  {
    return { cpRotaryLimitJointNew(body_a->m_body, body_b->m_body, min, max) };
  }

  PhysicsConstraint PhysicsConstraint::make_ratchet_joint(PhysicsBody* body_a, PhysicsBody* body_b, float phase, float ratchet)
  {
    return { cpRatchetJointNew(body_a->m_body, body_b->m_body, phase, ratchet) };
  }

  PhysicsConstraint PhysicsConstraint::make_gear_joint(PhysicsBody* body_a, PhysicsBody* body_b, float phase, float ratio)
  {
    return { cpGearJointNew(body_a->m_body, body_b->m_body, phase, ratio) };
  }

  PhysicsConstraint PhysicsConstraint::make_simple_motor(PhysicsBody* body_a, PhysicsBody* body_b, float rate)
  {
    return { cpSimpleMotorNew(body_a->m_body, body_b->m_body, rate) };
  }

  PhysicsConstraintType PhysicsConstraint::type() const
  {
    if (cpConstraintIsPinJoint(m_constraint) == cpTrue) {
      return PhysicsConstraintType::PinJoint;
    }

    if (cpConstraintIsSlideJoint(m_constraint) == cpTrue) {
      return PhysicsConstraintType::SlideJoint;
    }

    if (cpConstraintIsPivotJoint(m_constraint) == cpTrue) {
      return PhysicsConstraintType::PivotJoint;
    }

    if (cpConstraintIsGrooveJoint(m_constraint) == cpTrue) {
      return PhysicsConstraintType::GrooveJoint;
    }

    if (cpConstraintIsDampedSpring(m_constraint) == cpTrue) {
      return PhysicsConstraintType::DampedSpring;
    }

    if (cpConstraintIsDampedRotarySpring(m_constraint) == cpTrue) {
      return PhysicsConstraintType::DampedRotarySpring;
    }

    if (cpConstraintIsRotaryLimitJoint(m_constraint) == cpTrue) {
      return PhysicsConstraintType::RotaryLimitJoint;
    }

    if (cpConstraintIsRatchetJoint(m_constraint) == cpTrue) {
      return PhysicsConstraintType::RatchetJoint;
    }

    if (cpConstraintIsGearJoint(m_constraint) == cpTrue) {
      return PhysicsConstraintType::GearJoint;
    }

    if (cpConstraintIsSimpleMotor(m_constraint) == cpTrue) {
      return PhysicsConstraintType::SimpleMotor;
    }

    Log::fatal("Unknown constraint type.");
  }

  PhysicsWorld PhysicsConstraint::world() const
  {
    return { details::PhysicsExisting, cpConstraintGetSpace(m_constraint) };
  }

  PhysicsBody PhysicsConstraint::body_a() const
  {
    return { details::PhysicsExisting, cpConstraintGetBodyA(m_constraint) };
  }

  PhysicsBody PhysicsConstraint::body_b() const
  {
    return { details::PhysicsExisting, cpConstraintGetBodyB(m_constraint) };
  }

  float PhysicsConstraint::max_force() const
  {
    return static_cast<float>(cpConstraintGetMaxForce(m_constraint));
  }

  void PhysicsConstraint::set_max_force(float force)
  {
    cpConstraintSetMaxForce(m_constraint, force);
  }

  float PhysicsConstraint::error_bias() const
  {
    return static_cast<float>(cpConstraintGetErrorBias(m_constraint));
  }

  void PhysicsConstraint::set_error_bias(float bias)
  {
    cpConstraintSetErrorBias(m_constraint, bias);
  }

  float PhysicsConstraint::max_bias() const
  {
    return static_cast<float>(cpConstraintGetMaxBias(m_constraint));
  }

  void PhysicsConstraint::set_max_bias(float bias)
  {
    cpConstraintSetMaxBias(m_constraint, bias);
  }

  bool PhysicsConstraint::collide_bodies() const
  {
    return cpConstraintGetCollideBodies(m_constraint) == cpTrue;
  }

  void PhysicsConstraint::set_collide_bodies(bool collide)
  {
    cpConstraintSetCollideBodies(m_constraint, collide ? cpTrue : cpFalse);
  }

  float PhysicsConstraint::impulse()
  {
    return static_cast<float>(cpConstraintGetImpulse(m_constraint));
  }

  PhysicsConstraint::PhysicsConstraint(cpConstraint* constraint)
  : m_constraint(constraint)
  {
  }

  PhysicsConstraint::PhysicsConstraint(details::PhysicsExistingType existing, cpConstraint* constraint)
  : m_constraint(existing, constraint)
  {
  }

}
