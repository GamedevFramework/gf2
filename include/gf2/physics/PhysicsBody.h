// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_BODY_H
#define GF_PHYSICS_BODY_H

#include <string>
#include <type_traits>
#include <vector>

#include <box2d/box2d.h>

#include <gf2/core/Rect.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsId.h"
#include "PhysicsMassData.h"
#include "PhysicsOwner.h"

namespace gf {
  struct PhysicsContactFeatures;
  class PhysicsJoint;
  class PhysicsShape;
  class PhysicsWorld;

  // NOLINTNEXTLINE(performance-enum-size)
  enum class PhysicsBodyType : std::underlying_type_t<b2BodyType> {
    Static = b2_staticBody,
    Kinematic = b2_kinematicBody,
    Dynamic = b2_dynamicBody,
  };

  struct GF_PHYSICS_API PhysicsMotionLocks {
    bool linear_x = false;
    bool linear_y = false;
    bool angular_z = false;
  };

  struct GF_PHYSICS_API PhysicsBodyData {
    PhysicsBodyType type = PhysicsBodyType::Static;
    Vec2F location = { 0.0f, 0.0f };
    float rotation = 0.0f;
    Vec2F linear_velocity = { 0.0f, 0.0f };
    float angular_velocity = 0.0f;
    float linear_damping = 0.0f;
    float angular_damping = 0.0f;
    float gravity_scale = 1.0f;
    PhysicsMotionLocks motion_locks;
    bool enable_sleep = true;
    void* user_data = nullptr;
  };

  class GF_PHYSICS_API PhysicsBody {
  public:
    constexpr PhysicsBody() = default;
    PhysicsBody(PhysicsWorld* world, const PhysicsBodyData& data);
    PhysicsBody(const PhysicsBody&) = delete;
    PhysicsBody(PhysicsBody&& other) noexcept;
    ~PhysicsBody();

    PhysicsBody& operator=(const PhysicsBody&) = delete;
    PhysicsBody& operator=(PhysicsBody&& other) noexcept;

    PhysicsBodyId id() const;
    static PhysicsBody from_id(PhysicsBodyId id);

    void acquire();
    void release();

    bool valid() const;

    PhysicsBodyType type() const;
    void set_type(PhysicsBodyType type);

    std::string name() const;
    void set_string(const std::string& name);

    Vec2F location() const;
    void set_location(Vec2F location);

    float rotation() const;
    void set_rotation(float rotation);

    void set_transform(Vec2F location, float rotation);

    Vec2F compute_world_to_local_point(Vec2F world_point) const;
    Vec2F compute_local_to_world_point(Vec2F local_point) const;

    Vec2F compute_world_to_local_vector(Vec2F world_vector) const;
    Vec2F compute_local_to_world_vector(Vec2F local_vector) const;

    Vec2F linear_velocity() const;
    void set_linear_velocity(Vec2F linear_velocity);

    float angular_velocity() const;
    void set_angular_velocity(float angular_velocity);

    Vec2F velocity_at_world_point(Vec2F world_point) const;
    Vec2F velocity_at_local_point(Vec2F local_point) const;

    void apply_force_at_world_point(Vec2F force, Vec2F point);
    void apply_force_at_local_point(Vec2F force, Vec2F point);
    void apply_force_at_center(Vec2F force);

    void apply_torque(float torque);

    void clear_forces();

    void apply_linear_impulse_at_world_point(Vec2F impulse, Vec2F point);
    void apply_linear_impulse_at_local_point(Vec2F impulse, Vec2F point);
    void apply_linear_impulse_at_center(Vec2F impulse);

    void apply_angular_impulse(float impulse);

    float mass() const;
    float rotational_inertia() const;

    Vec2F local_center_of_mass() const;
    Vec2F world_center_of_mass() const;

    PhysicsMassData mass_data() const;
    void set_mass_data(const PhysicsMassData& data);

    float linear_damping() const;
    void set_linear_damping(float linear_damping);

    float angular_damping() const;
    void set_angular_damping(float angular_damping);

    float gravity_scale() const;
    void set_gravity_scale(float gravity_scale);

    PhysicsMotionLocks motion_locks() const;
    void set_motion_locks(PhysicsMotionLocks motion_locks);

    void enable_contact_events(bool flag);
    void enable_hit_events(bool flag);

    PhysicsWorld world() const;
    std::vector<PhysicsShape> shapes() const;
    std::vector<PhysicsJoint> joints() const;
    std::vector<PhysicsContactFeatures> contact_features() const;

    RectF bouding_box() const;

    void set_user_data(void* data);
    void* user_data();

    template<typename T>
    T* user_data_as()
    {
      return static_cast<T*>(user_data());
    }

  private:
    constexpr PhysicsBody(details::PhysicsOwner owner, b2BodyId id)
    : m_owner(owner)
    , m_id(id)
    {
    }

    details::PhysicsOwner m_owner = details::PhysicsOwner::None;
    b2BodyId m_id = {};
  };


}

#endif // GF_PHYSICS_BODY_H
