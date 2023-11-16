// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/physics/PhysicsBody.h>
// clang-format on

#include <gf2/physics/PhysicsArbiter.h>
#include <gf2/physics/PhysicsConstraint.h>
#include <gf2/physics/PhysicsShape.h>
#include <gf2/physics/PhysicsWorld.h>

namespace gf {
  static_assert(static_cast<cpBodyType>(PhysicsBodyType::Dynamic) == CP_BODY_TYPE_DYNAMIC);
  static_assert(static_cast<cpBodyType>(PhysicsBodyType::Kinematic) == CP_BODY_TYPE_KINEMATIC);
  static_assert(static_cast<cpBodyType>(PhysicsBodyType::Static) == CP_BODY_TYPE_STATIC);

  PhysicsBody PhysicsBody::make_dynamic(float mass, float moment)
  {
    return { cpBodyNew(mass, moment) };
  }

  PhysicsBody PhysicsBody::make_kinematic()
  {
    return { cpBodyNewKinematic() };
  }

  PhysicsBody PhysicsBody::make_static()
  {
    return { cpBodyNewStatic() };
  }

  void PhysicsBody::activate()
  {
    cpBodyActivate(m_body);
  }

  void PhysicsBody::activate_static(PhysicsShape filter)
  {
    cpBodyActivateStatic(m_body, filter.m_shape);
  }

  void PhysicsBody::sleep()
  {
    cpBodySleep(m_body);
  }

  void PhysicsBody::sleep_with_group(PhysicsBody group)
  {
    cpBodySleepWithGroup(m_body, group.m_body);
  }

  bool PhysicsBody::sleeping() const
  {
    return cpBodyIsSleeping(m_body) == cpTrue;
  }

  PhysicsBodyType PhysicsBody::type()
  {
    return static_cast<PhysicsBodyType>(cpBodyGetType(m_body));
  }

  void PhysicsBody::set_type(PhysicsBodyType type)
  {
    cpBodySetType(m_body, static_cast<cpBodyType>(type));
  }

  PhysicsWorld PhysicsBody::world() const
  {
    return { details::PhysicsExisting, cpBodyGetSpace(m_body) };
  }

  float PhysicsBody::mass() const
  {
    return static_cast<float>(cpBodyGetMass(m_body));
  }

  void PhysicsBody::set_mass(float mass)
  {
    cpBodySetMass(m_body, mass);
  }

  float PhysicsBody::moment() const
  {
    return static_cast<float>(cpBodyGetMoment(m_body));
  }

  void PhysicsBody::set_moment(float moment)
  {
    cpBodySetMoment(m_body, moment);
  }

  Vec2F PhysicsBody::location() const
  {
    auto position = cpBodyGetPosition(m_body);
    return gf::vec(position.x, position.y);
  }

  void PhysicsBody::set_location(Vec2F location)
  {
    cpBodySetPosition(m_body, cpv(location.x, location.y));
  }

  Vec2F PhysicsBody::center_of_gravity() const
  {
    auto cog = cpBodyGetCenterOfGravity(m_body);
    return gf::vec(cog.x, cog.y);
  }

  void PhysicsBody::set_center_of_gravity(Vec2F cog)
  {
    cpBodySetCenterOfGravity(m_body, cpv(cog.x, cog.y));
  }

  Vec2F PhysicsBody::velocity() const
  {
    auto velocity = cpBodyGetVelocity(m_body);
    return gf::vec(velocity.x, velocity.y);
  }

  void PhysicsBody::set_velocity(Vec2F velocity)
  {
    cpBodySetVelocity(m_body, cpv(velocity.x, velocity.y));
  }

  Vec2F PhysicsBody::force() const
  {
    auto force = cpBodyGetForce(m_body);
    return gf::vec(force.x, force.y);
  }

  void PhysicsBody::set_force(Vec2F force)
  {
    cpBodySetForce(m_body, cpv(force.x, force.y));
  }

  float PhysicsBody::angle() const
  {
    return static_cast<float>(cpBodyGetAngle(m_body));
  }

  void PhysicsBody::set_angle(float angle)
  {
    cpBodySetAngle(m_body, angle);
  }

  float PhysicsBody::angular_velocity() const
  {
    return static_cast<float>(cpBodyGetAngularVelocity(m_body));
  }

  void PhysicsBody::set_angular_velocity(float velocity)
  {
    cpBodySetAngularVelocity(m_body, velocity);
  }

  float PhysicsBody::torque() const
  {
    return static_cast<float>(cpBodyGetTorque(m_body));
  }

  void PhysicsBody::set_torque(float torque)
  {
    cpBodySetTorque(m_body, torque);
  }

  Vec2F PhysicsBody::compute_local_to_world(Vec2F point) const
  {
    auto coords = cpBodyLocalToWorld(m_body, cpv(point.x, point.y));
    return gf::vec(coords.x, coords.y);
  }

  Vec2F PhysicsBody::compute_world_to_local(Vec2F point) const
  {
    auto coords = cpBodyWorldToLocal(m_body, cpv(point.x, point.y));
    return gf::vec(coords.x, coords.y);
  }

  void PhysicsBody::apply_force_at_world_point(Vec2F force, Vec2F point)
  {
    cpBodyApplyForceAtWorldPoint(m_body, cpv(force.x, force.y), cpv(point.x, point.y));
  }

  void PhysicsBody::apply_force_at_local_point(Vec2F force, Vec2F point)
  {
    cpBodyApplyForceAtLocalPoint(m_body, cpv(force.x, force.y), cpv(point.x, point.y));
  }

  void PhysicsBody::apply_impulse_at_world_point(Vec2F impulse, Vec2F point)
  {
    cpBodyApplyImpulseAtWorldPoint(m_body, cpv(impulse.x, impulse.y), cpv(point.x, point.y));
  }

  void PhysicsBody::apply_impulse_at_local_point(Vec2F impulse, Vec2F point)
  {
    cpBodyApplyImpulseAtLocalPoint(m_body, cpv(impulse.x, impulse.y), cpv(point.x, point.y));
  }

  Vec2F PhysicsBody::velocity_at_world_point(Vec2F point) const
  {
    auto velocity = cpBodyGetVelocityAtWorldPoint(m_body, cpv(point.x, point.y));
    return gf::vec(velocity.x, velocity.y);
  }

  Vec2F PhysicsBody::velocity_at_local_point(Vec2F point) const
  {
    auto velocity = cpBodyGetVelocityAtLocalPoint(m_body, cpv(point.x, point.y));
    return gf::vec(velocity.x, velocity.y);
  }

  float PhysicsBody::kinematic_energy() const
  {
    return static_cast<float>(cpBodyKineticEnergy(m_body));
  }

  void PhysicsBody::each_shape(std::function<void(const PhysicsBody, PhysicsShape)> func)
  {
    cpBodyEachShape(m_body, shape_iterator, &func);
  }

  void PhysicsBody::each_constraint(std::function<void(PhysicsBody, PhysicsConstraint)> func)
  {
    cpBodyEachConstraint(m_body, constraint_iterator, &func);
  }

  void PhysicsBody::each_arbiter(std::function<void(PhysicsBody, PhysicsArbiter)> func)
  {
    cpBodyEachArbiter(m_body, arbiter_iterator, &func);
  }

  void PhysicsBody::shape_iterator(cpBody* body, cpShape* shape, void* data)
  {
    auto* func = static_cast<std::function<void(PhysicsBody, PhysicsShape)>*>(data);
    (*func)(PhysicsBody(details::PhysicsExisting, body), PhysicsShape(details::PhysicsExisting, shape));
  }

  void PhysicsBody::constraint_iterator(cpBody* body, cpConstraint* constraint, void* data)
  {
    auto* func = static_cast<std::function<void(PhysicsBody, PhysicsConstraint)>*>(data);
    (*func)(PhysicsBody(details::PhysicsExisting, body), PhysicsConstraint(details::PhysicsExisting, constraint));
  }

  void PhysicsBody::arbiter_iterator(cpBody* body, cpArbiter* arbiter, void* data)
  {
    auto* func = static_cast<std::function<void(PhysicsBody, PhysicsArbiter)>*>(data);
    (*func)(PhysicsBody(details::PhysicsExisting, body), PhysicsArbiter(arbiter));
  }

  PhysicsBody::PhysicsBody(cpBody* body)
  : m_body(body)
  {
  }

  PhysicsBody::PhysicsBody(details::PhysicsExistingType existing, cpBody* body)
  : m_body(existing, body)
  {
  }

}
