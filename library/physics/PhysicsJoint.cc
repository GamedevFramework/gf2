// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsJoint.h>

#include <cassert>

#include <utility>

#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsWorld.h>

namespace gf {

  /*
   * PhysicsJoint
   */

  PhysicsJoint::PhysicsJoint(PhysicsJoint&& other) noexcept
  : m_owner(std::exchange(other.m_owner, details::PhysicsOwner::None))
  , m_id(std::exchange(other.m_id, b2_nullJointId))
  {
  }

  PhysicsJoint::~PhysicsJoint()
  {
    if (m_owner == details::PhysicsOwner::Object) {
      b2DestroyJoint(m_id, true);
    }
  }

  PhysicsJoint& PhysicsJoint::operator=(PhysicsJoint&& other) noexcept
  {
    std::swap(other.m_owner, m_owner);
    std::swap(other.m_id, m_id);
    return *this;
  }

  PhysicsJointId PhysicsJoint::id() const
  {
    return details::to_id(m_id);
  }

  PhysicsJoint PhysicsJoint::from_id(PhysicsJointId id)
  {
    return { details::PhysicsOwner::None, details::to_raw(id) };
  }

  void PhysicsJoint::acquire()
  {
    m_owner = details::PhysicsOwner::Object;
  }

  void PhysicsJoint::release()
  {
    m_owner = details::PhysicsOwner::None;
  }

  bool PhysicsJoint::valid() const
  {
    return b2Joint_IsValid(m_id);
  }

  PhysicsJointType PhysicsJoint::type() const
  {
    return static_cast<PhysicsJointType>(b2Joint_GetType(m_id));
  }

  PhysicsBody PhysicsJoint::body0() const
  {
    return PhysicsBody::from_id(details::to_id(b2Joint_GetBodyA(m_id)));
  }

  PhysicsBody PhysicsJoint::body1() const
  {
    return PhysicsBody::from_id(details::to_id(b2Joint_GetBodyB(m_id)));
  }

  PhysicsWorld PhysicsJoint::world() const
  {
    return PhysicsWorld::from_id(details::to_id(b2Joint_GetWorld(m_id)));
  }

  PhysicsTransform PhysicsJoint::frame0() const
  {
    return details::to_transform(b2Joint_GetLocalFrameA(m_id));
  }

  void PhysicsJoint::set_frame0(const PhysicsTransform& frame)
  {
    b2Joint_SetLocalFrameA(m_id, details::to_raw(frame));
  }

  PhysicsTransform PhysicsJoint::frame1() const
  {
    return details::to_transform(b2Joint_GetLocalFrameB(m_id));
  }

  void PhysicsJoint::set_frame1(const PhysicsTransform& frame)
  {
    b2Joint_SetLocalFrameB(m_id, details::to_raw(frame));
  }

  Vec2F PhysicsJoint::constraint_force() const
  {
    const b2Vec2 raw = b2Joint_GetConstraintForce(m_id);
    return { raw.x, raw.y };
  }

  float PhysicsJoint::constraint_torque() const
  {
    return b2Joint_GetConstraintTorque(m_id);
  }

  float PhysicsJoint::linear_separation() const
  {
    return b2Joint_GetLinearSeparation(m_id);
  }

  float PhysicsJoint::angular_separation() const
  {
    return b2Joint_GetAngularSeparation(m_id);
  }

  float PhysicsJoint::force_threshold() const
  {
    return b2Joint_GetForceThreshold(m_id);
  }

  void PhysicsJoint::set_force_threshold(float threshold)
  {
    b2Joint_SetForceThreshold(m_id, threshold);
  }

  float PhysicsJoint::torque_threshold() const
  {
    return b2Joint_GetTorqueThreshold(m_id);
  }

  void PhysicsJoint::set_torque_threshold(float threshold)
  {
    b2Joint_SetTorqueThreshold(m_id, threshold);
  }

  void PhysicsJoint::set_user_data(void* data)
  {
    b2Joint_SetUserData(m_id, data);
  }

  void* PhysicsJoint::user_data()
  {
    return b2Joint_GetUserData(m_id);
  }

  namespace {

    b2JointDef compute_base(const PhysicsJointData& data)
    {
      b2JointDef def = {};
      def.bodyIdA = details::to_raw(data.body0);
      def.bodyIdB = details::to_raw(data.body1);
      def.localFrameA = details::to_raw(data.frame0);
      def.localFrameB = details::to_raw(data.frame1);
      def.forceThreshold = data.force_threshold;
      def.torqueThreshold = data.torque_threshold;
      def.userData = data.user_data;
      return def;
    }

  }

  PhysicsJoint PhysicsJoint::create_distance_joint(PhysicsWorld* world, const PhysicsDistanceJointData& data)
  {
    assert(world != nullptr);
    assert(world->valid());

    b2DistanceJointDef def = b2DefaultDistanceJointDef();
    def.base = compute_base(data);
    def.length = data.length;
    def.enableSpring = data.enable_spring;
    def.lowerSpringForce = data.lower_spring_force;
    def.upperSpringForce = data.upper_spring_force;
    def.hertz = data.hertz;
    def.dampingRatio = data.damping_ratio;
    def.enableLimit = data.enable_limit;
    def.minLength = data.min_length;
    def.maxLength = data.max_length;
    def.enableMotor = data.enable_motor;
    def.maxMotorForce = data.max_motor_force;
    def.motorSpeed = data.motor_speed;

    return { details::PhysicsOwner::Object, b2CreateDistanceJoint(details::to_raw(world->id()), &def) };
  }

  PhysicsJoint PhysicsJoint::create_motor_joint(PhysicsWorld* world, const PhysicsMotorJointData& data)
  {
    assert(world != nullptr);
    assert(world->valid());

    b2MotorJointDef def = b2DefaultMotorJointDef();
    def.base = compute_base(data);
    def.linearVelocity = { data.linear_velocity.x, data.linear_velocity.y };
    def.maxVelocityForce = data.max_velocity_force;
    def.angularVelocity = data.angular_velocity;
    def.maxVelocityTorque = data.max_velocity_torque;
    def.linearHertz = data.linear_hertz;
    def.linearDampingRatio = data.linear_damping_ratio;
    def.maxSpringForce = data.max_spring_force;
    def.angularHertz = data.angular_hertz;
    def.angularDampingRatio = data.angular_damping_ratio;
    def.maxSpringTorque = data.max_spring_torque;

    return { details::PhysicsOwner::Object, b2CreateMotorJoint(details::to_raw(world->id()), &def) };
  }

  PhysicsJoint PhysicsJoint::create_filter_joint(PhysicsWorld* world, const PhysicsFilterJointData& data)
  {
    assert(world != nullptr);
    assert(world->valid());

    b2FilterJointDef def = b2DefaultFilterJointDef();
    def.base = compute_base(data);

    return { details::PhysicsOwner::Object, b2CreateFilterJoint(details::to_raw(world->id()), &def) };
  }

  PhysicsJoint PhysicsJoint::create_prismatic_joint(PhysicsWorld* world, const PhysicsPrismaticJointData& data)
  {
    assert(world != nullptr);
    assert(world->valid());

    b2PrismaticJointDef def = b2DefaultPrismaticJointDef();
    def.base = compute_base(data);
    def.enableSpring = data.enable_spring;
    def.hertz = data.hertz;
    def.dampingRatio = data.damping_ratio;
    def.targetTranslation = data.target_translation;
    def.enableLimit = data.enable_limit;
    def.lowerTranslation = data.lower_translation;
    def.upperTranslation = data.upper_translation;
    def.enableMotor = data.enable_motor;
    def.maxMotorForce = data.max_motor_force;
    def.motorSpeed = data.motor_speed;

    return { details::PhysicsOwner::Object, b2CreatePrismaticJoint(details::to_raw(world->id()), &def) };
  }

  PhysicsJoint PhysicsJoint::create_revolute_joint(PhysicsWorld* world, const PhysicsRevoluteJointData& data)
  {
    assert(world != nullptr);
    assert(world->valid());

    b2RevoluteJointDef def = b2DefaultRevoluteJointDef();
    def.base = compute_base(data);
    def.targetAngle = data.target_angle;
    def.enableSpring = data.enable_spring;
    def.hertz = data.hertz;
    def.dampingRatio = data.damping_ratio;
    def.enableLimit = data.enable_limit;
    def.lowerAngle = data.lower_angle;
    def.upperAngle = data.upper_angle;
    def.enableMotor = data.enable_motor;
    def.maxMotorTorque = data.max_motor_torque;
    def.motorSpeed = data.motor_speed;

    return { details::PhysicsOwner::Object, b2CreateRevoluteJoint(details::to_raw(world->id()), &def) };
  }

  PhysicsJoint PhysicsJoint::create_weld_joint(PhysicsWorld* world, const PhysicsWeldJointData& data)
  {
    assert(world != nullptr);
    assert(world->valid());

    b2WeldJointDef def = b2DefaultWeldJointDef();
    def.base = compute_base(data);
    def.linearHertz = data.linear_hertz;
    def.angularHertz = data.angular_hertz;
    def.linearDampingRatio = data.linear_damping_ratio;
    def.angularDampingRatio = data.angular_damping_ratio;

    return { details::PhysicsOwner::Object, b2CreateWeldJoint(details::to_raw(world->id()), &def) };
  }

  PhysicsJoint PhysicsJoint::create_wheel_joint(PhysicsWorld* world, const PhysicsWheelJointData& data)
  {
    assert(world != nullptr);
    assert(world->valid());

    b2WheelJointDef def = b2DefaultWheelJointDef();
    def.base = compute_base(data);
    def.enableSpring = data.enable_spring;
    def.hertz = data.hertz;
    def.dampingRatio = data.damping_ratio;
    def.enableLimit = data.enable_limit;
    def.lowerTranslation = data.lower_translation;
    def.upperTranslation = data.upper_translation;
    def.enableMotor = data.enable_motor;
    def.maxMotorTorque = data.max_motor_torque;
    def.motorSpeed = data.motor_speed;

    return { details::PhysicsOwner::Object, b2CreateWheelJoint(details::to_raw(world->id()), &def) };
  }


}

