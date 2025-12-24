// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsBody.h>

#include <cassert>

#include <utility>

#include <gf2/physics/PhysicsContact.h>
#include <gf2/physics/PhysicsJoint.h>
#include <gf2/physics/PhysicsShape.h>
#include <gf2/physics/PhysicsWorld.h>

namespace gf {

  PhysicsBody::PhysicsBody(PhysicsWorld* world, const PhysicsBodyData& data)
  : m_owner(details::PhysicsOwner::Object)
  {
    assert(world != nullptr);
    assert(world->valid());

    b2BodyDef def = b2DefaultBodyDef();
    def.type = static_cast<b2BodyType>(data.type);
    def.position = { data.location.x, data.location.y };
    def.rotation = b2MakeRot(data.rotation);
    def.linearVelocity = { data.linear_velocity.x, data.linear_velocity.y };
    def.angularVelocity = data.angular_velocity;
    def.linearDamping = data.linear_damping;
    def.angularDamping = data.angular_damping;
    def.gravityScale = data.gravity_scale;
    def.motionLocks.linearX = data.motion_locks.linear_x;
    def.motionLocks.linearY = data.motion_locks.linear_y;
    def.motionLocks.angularZ = data.motion_locks.angular_z;
    def.enableSleep = data.enable_sleep;
    def.userData = data.user_data;

    m_id = b2CreateBody(details::to_raw(world->id()), &def);
  }

  PhysicsBody::PhysicsBody(PhysicsBody&& other) noexcept
  : m_owner(std::exchange(other.m_owner, details::PhysicsOwner::None))
  , m_id(std::exchange(other.m_id, b2_nullBodyId))
  {
  }

  PhysicsBody::~PhysicsBody()
  {
    if (m_owner == details::PhysicsOwner::Object) {
      b2DestroyBody(m_id);
    }
  }

  PhysicsBody& PhysicsBody::operator=(PhysicsBody&& other) noexcept
  {
    std::swap(other.m_owner, m_owner);
    std::swap(other.m_id, m_id);
    return *this;
  }

  PhysicsBodyId PhysicsBody::id() const
  {
    return details::to_id(m_id);
  }

  PhysicsBody PhysicsBody::from_id(PhysicsBodyId id)
  {
    return { details::PhysicsOwner::None, details::to_raw(id) };
  }

  bool PhysicsBody::valid() const
  {
    return b2Body_IsValid(m_id);
  }

  PhysicsBodyType PhysicsBody::type() const
  {
    return static_cast<PhysicsBodyType>(b2Body_GetType(m_id));
  }

  void PhysicsBody::set_type(PhysicsBodyType type)
  {
    b2Body_SetType(m_id, static_cast<b2BodyType>(type));
  }

  std::string PhysicsBody::name() const
  {
    return b2Body_GetName(m_id);
  }

  void PhysicsBody::set_string(const std::string& name)
  {
    b2Body_SetName(m_id, name.c_str());
  }

  Vec2F PhysicsBody::location() const
  {
    const b2Vec2 raw = b2Body_GetPosition(m_id);
    return { raw.x, raw.y };
  }

  PhysicsRotation PhysicsBody::rotation() const
  {
    const b2Rot raw = b2Body_GetRotation(m_id);
    return { raw.c, raw.s };
  }

  PhysicsTransform PhysicsBody::transform() const
  {
    return details::to_transform(b2Body_GetTransform(m_id));
  }

  void PhysicsBody::set_transform(Vec2F location, PhysicsRotation rotation)
  {
    b2Body_SetTransform(m_id, { location.x, location.y }, { rotation.cosine, rotation.sine });
  }

  Vec2F PhysicsBody::compute_world_to_local_point(Vec2F world_point) const
  {
    const b2Vec2 raw = b2Body_GetLocalPoint(m_id, { world_point.x, world_point.y });
    return { raw.x, raw.y };
  }

  Vec2F PhysicsBody::compute_local_to_world_point(Vec2F local_point) const
  {
    const b2Vec2 raw = b2Body_GetWorldPoint(m_id, { local_point.x, local_point.y });
    return { raw.x, raw.y };
  }

  Vec2F PhysicsBody::compute_world_to_local_vector(Vec2F world_vector) const
  {
    const b2Vec2 raw = b2Body_GetLocalVector(m_id, { world_vector.x, world_vector.y });
    return { raw.x, raw.y };
  }

  Vec2F PhysicsBody::compute_local_to_world_vector(Vec2F local_vector) const
  {
    const b2Vec2 raw = b2Body_GetWorldVector(m_id, { local_vector.x, local_vector.y });
    return { raw.x, raw.y };
  }

  Vec2F PhysicsBody::linear_velocity() const
  {
    const b2Vec2 raw = b2Body_GetLinearVelocity(m_id);
    return { raw.x, raw.y };
  }

  void PhysicsBody::set_linear_velocity(Vec2F linear_velocity)
  {
    b2Body_SetLinearVelocity(m_id, { linear_velocity.x, linear_velocity.y });
  }

  float PhysicsBody::angular_velocity() const
  {
    return b2Body_GetAngularVelocity(m_id);
  }

  void PhysicsBody::set_angular_velocity(float angular_velocity)
  {
    b2Body_SetAngularVelocity(m_id, angular_velocity);
  }

  Vec2F PhysicsBody::velocity_at_world_point(Vec2F world_point) const
  {
    const b2Vec2 raw = b2Body_GetWorldPointVelocity(m_id, { world_point.x, world_point.y });
    return { raw.x, raw.y };
  }

  Vec2F PhysicsBody::velocity_at_local_point(Vec2F local_point) const
  {
    const b2Vec2 raw = b2Body_GetLocalPointVelocity(m_id, { local_point.x, local_point.y });
    return { raw.x, raw.y };
  }

  void PhysicsBody::apply_force_at_world_point(Vec2F force, Vec2F point)
  {
    b2Body_ApplyForce(m_id, { force.x, force.y }, { point.x, point.y }, true);
  }

  void PhysicsBody::apply_force_at_local_point(Vec2F force, Vec2F point)
  {
    apply_force_at_world_point(force, compute_local_to_world_vector(point));
  }

  void PhysicsBody::apply_force_at_center(Vec2F force)
  {
    b2Body_ApplyForceToCenter(m_id, { force.x, force.y }, true);
  }

  void PhysicsBody::apply_torque(float torque)
  {
    b2Body_ApplyTorque(m_id, torque, true);
  }

  void PhysicsBody::clear_forces()
  {
    b2Body_ClearForces(m_id);
  }

  void PhysicsBody::apply_linear_impulse_at_world_point(Vec2F impulse, Vec2F point)
  {
    b2Body_ApplyLinearImpulse(m_id, { impulse.x, impulse.y }, { point.x, point.y }, true);
  }

  void PhysicsBody::apply_linear_impulse_at_local_point(Vec2F impulse, Vec2F point)
  {
    apply_linear_impulse_at_world_point(impulse, compute_local_to_world_vector(point));
  }

  void PhysicsBody::apply_linear_impulse_at_center(Vec2F impulse)
  {
    b2Body_ApplyLinearImpulseToCenter(m_id, { impulse.x, impulse.y }, true);
  }

  void PhysicsBody::apply_angular_impulse(float impulse)
  {
    b2Body_ApplyAngularImpulse(m_id, impulse, true);
  }

  float PhysicsBody::mass() const
  {
    return b2Body_GetMass(m_id);
  }

  float PhysicsBody::rotational_inertia() const
  {
    return b2Body_GetRotationalInertia(m_id);
  }

  Vec2F PhysicsBody::local_center_of_mass() const
  {
    const b2Vec2 raw = b2Body_GetLocalCenterOfMass(m_id);
    return { raw.x, raw.y };
  }

  Vec2F PhysicsBody::world_center_of_mass() const
  {
    const b2Vec2 raw = b2Body_GetWorldCenterOfMass(m_id);
    return { raw.x, raw.y };
  }

  PhysicsMassData PhysicsBody::mass_data() const
  {
    const b2MassData raw = b2Body_GetMassData(m_id);
    return { raw.mass, { raw.center.x, raw.center.y }, raw.rotationalInertia };
  }

  void PhysicsBody::set_mass_data(const PhysicsMassData& data)
  {
    b2Body_SetMassData(m_id, { data.mass, { data.center.x, data.center.y }, data.rotational_inertia });
  }

  float PhysicsBody::linear_damping() const
  {
    return b2Body_GetLinearDamping(m_id);
  }

  void PhysicsBody::set_linear_damping(float linear_damping)
  {
    b2Body_SetLinearDamping(m_id, linear_damping);
  }

  float PhysicsBody::angular_damping() const
  {
    return b2Body_GetAngularDamping(m_id);
  }

  void PhysicsBody::set_angular_damping(float angular_damping)
  {
    b2Body_SetAngularDamping(m_id, angular_damping);
  }

  float PhysicsBody::gravity_scale() const
  {
    return b2Body_GetGravityScale(m_id);
  }

  void PhysicsBody::set_gravity_scale(float gravity_scale)
  {
    b2Body_SetGravityScale(m_id, gravity_scale);
  }

  PhysicsMotionLocks PhysicsBody::motion_locks() const
  {
    const b2MotionLocks raw = b2Body_GetMotionLocks(m_id);
    return { raw.linearX, raw.linearY, raw.angularZ };
  }

  void PhysicsBody::set_motion_locks(PhysicsMotionLocks motion_locks)
  {
    b2Body_SetMotionLocks(m_id, { motion_locks.linear_x, motion_locks.linear_y, motion_locks.angular_z });
  }

  void PhysicsBody::enable_contact_events(bool flag)
  {
    b2Body_EnableContactEvents(m_id, flag);
  }

  void PhysicsBody::enable_hit_events(bool flag)
  {
    b2Body_EnableHitEvents(m_id, flag);
  }

  PhysicsWorld PhysicsBody::world() const
  {
    const b2WorldId world_id = b2Body_GetWorld(m_id);
    return PhysicsWorld::from_id(static_cast<PhysicsWorldId>(b2StoreWorldId(world_id)));
  }

  std::vector<PhysicsShape> PhysicsBody::shapes() const
  {
    return {}; // TODO
  }

  std::vector<PhysicsJoint> PhysicsBody::joints() const
  {
    return {}; // TODO
  }

  std::vector<PhysicsContactFeatures> PhysicsBody::contact_features() const
  {
    return {}; // TODO
  }

  RectF PhysicsBody::bouding_box() const
  {
    const b2AABB raw = b2Body_ComputeAABB(m_id);
    return RectF::from_min_max({ raw.lowerBound.x, raw.lowerBound.y }, { raw.upperBound.x, raw.upperBound.y });
  }

  void PhysicsBody::set_user_data(void* data)
  {
    b2Body_SetUserData(m_id, data);
  }

  void* PhysicsBody::user_data()
  {
    return b2Body_GetUserData(m_id);
  }

}
