// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_JOINT_H
#define GF_PHYSICS_JOINT_H

#include <type_traits>
#include <limits>

#include <box2d/box2d.h>

#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsId.h"
#include "PhysicsOwner.h"

namespace gf {
  class PhysicsBody;
  class PhysicsWorld;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class PhysicsJointType : std::underlying_type_t<b2JointType> {
    Distance = b2_distanceJoint,
    Filter = b2_filterJoint,
    Motor = b2_motorJoint,
    Prismatic = b2_prismaticJoint,
    Revolute = b2_revoluteJoint,
    Weld = b2_weldJoint,
    Wheel = b2_wheelJoint,
  };

  struct GF_PHYSICS_API PhysicsJointData {
    PhysicsBodyId body0 = {};
    Vec2F location0;
    float rotation0;

    PhysicsBodyId body1 = {};
    Vec2F location1;
    float rotation1;

    float force_threshold = std::numeric_limits<float>::max();
    float torque_threshold = std::numeric_limits<float>::max();;
    void* user_data = nullptr;
  };

  struct GF_PHYSICS_API PhysicsDistanceJointData : PhysicsJointData {
    float length = 1.0f;
    bool enable_spring = false;
    float lower_spring_force = std::numeric_limits<float>::lowest();
    float upper_spring_force = std::numeric_limits<float>::max();
    float hertz = 0.0f;
    float damping_ratio = 0.0f;
    bool enable_limit = false;
    float min_length = 0.0f;
    float max_length = 100000.0f;
    bool enable_motor = false;
    float max_motor_force = 0.0f;
    float motor_speed = 0.0f;
  };

  struct GF_PHYSICS_API PhysicsMotorJointData : PhysicsJointData {
    Vec2F linear_velocity = { 0.0f, 0.0f };
    float max_velocity_force = 0.0f;
    float angular_velocity = 0.0f;
    float max_velocity_torque = 0.0f;
    float linear_hertz = 0.0f;
    float linear_damping_ratio = 0.0f;
    float max_spring_force = 0.0f;
    float angular_hertz = 0.0f;
    float angular_damping_ratio = 0.0f;
    float max_spring_torque = 0.0f;
  };

  struct GF_PHYSICS_API PhysicsFilterJointData : PhysicsJointData {
  };

  struct GF_PHYSICS_API PhysicsPrismaticJointData : PhysicsJointData {
    bool enable_spring = false;
    float hertz = 0.0f;
    float damping_ratio = 0.0f;
    float target_translation = 0.0f;
    bool enable_limit = false;
    float lower_translation = 0.0f;
    float upper_translation = 0.0f;
    bool enable_motor = false;
    float max_motor_force = 0.0f;
    float motor_speed = 0.0f;
  };

  struct GF_PHYSICS_API PhysicsRevoluteJointData : PhysicsJointData {
    float target_angle = 0.0f;
    bool enable_spring = false;
    float hertz = 0.0f;
    float damping_ratio = 0.0f;
    bool enable_limit = false;
    float lower_angle = 0.0f;
    float upper_angle = 0.0f;
    bool enable_motor = false;
    float max_motor_torque = 0.0f;
    float motor_speed = 0.0f;
  };

  struct GF_PHYSICS_API PhysicsWeldJointData : PhysicsJointData {
    float linear_hertz = 0.0f;
    float angular_hertz = 0.0f;
    float linear_damping_ratio = 0.0f;
    float angular_damping_ratio = 0.0f;
  };

  struct GF_PHYSICS_API PhysicsWheelJointData : PhysicsJointData {
    bool enable_spring = true;
    float hertz = 1.0f;
    float damping_ratio = 0.7f;
    bool enable_limit = false;
    float lower_translation = 0.0f;
    float upper_translation = 0.0f;
    bool enable_motor = false;
    float max_motor_torque = 0.0f;
    float motor_speed = 0.0f;
  };

  class GF_PHYSICS_API PhysicsJoint {
  public:
    PhysicsJoint() = default;
    PhysicsJoint(const PhysicsJoint&) = delete;
    PhysicsJoint(PhysicsJoint&& other) noexcept;
    ~PhysicsJoint();

    PhysicsJoint& operator=(const PhysicsJoint&) = delete;
    PhysicsJoint& operator=(PhysicsJoint&& other) noexcept;

    PhysicsJointId id() const;
    static PhysicsJoint from_id(PhysicsJointId id);

    void acquire();
    void release();

    bool valid() const;

    PhysicsJointType type() const;

    PhysicsBody body0() const;
    PhysicsBody body1() const;

    PhysicsWorld world() const;

    Vec2F location0() const;
    float rotation0() const;

    void set_transform0(Vec2F location, float rotation);

    Vec2F location1() const;
    float rotation1() const;

    void set_transform1(Vec2F location, float rotation);

    Vec2F constraint_force() const;
    float constraint_torque() const;
    float linear_separation() const;
    float angular_separation() const;

    float force_threshold() const;
    void set_force_threshold(float threshold);

    float torque_threshold() const;
    void set_torque_threshold(float threshold);

    void set_user_data(void* data);
    void* user_data();

    template<typename T>
    T* user_data_as()
    {
      return static_cast<T*>(user_data());
    }

    static PhysicsJoint create_distance_joint(PhysicsWorld* world, const PhysicsDistanceJointData& data);
    static PhysicsJoint create_motor_joint(PhysicsWorld* world, const PhysicsMotorJointData& data);
    static PhysicsJoint create_filter_joint(PhysicsWorld* world, const PhysicsFilterJointData& data);
    static PhysicsJoint create_prismatic_joint(PhysicsWorld* world, const PhysicsPrismaticJointData& data);
    static PhysicsJoint create_revolute_joint(PhysicsWorld* world, const PhysicsRevoluteJointData& data);
    static PhysicsJoint create_weld_joint(PhysicsWorld* world, const PhysicsWeldJointData& data);
    static PhysicsJoint create_wheel_joint(PhysicsWorld* world, const PhysicsWheelJointData& data);

  private:
    constexpr PhysicsJoint(details::PhysicsOwner owner, b2JointId id)
    : m_owner(owner)
    , m_id(id)
    {
    }

    details::PhysicsOwner m_owner = details::PhysicsOwner::None;
    b2JointId m_id = {};
  };


}

#endif // GF_PHYSICS_JOINT_H
