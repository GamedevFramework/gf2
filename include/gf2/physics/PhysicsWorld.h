// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_WORLD_H
#define GF_PHYSICS_WORLD_H

#include <functional>

#include <box2d/box2d.h>

#include <gf2/core/Model.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Span.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsFilter.h"
#include "PhysicsId.h"
#include "PhysicsOwner.h"
#include "PhysicsShape.h"
#include "PhysicsDebug.h"

namespace gf {
  struct PhysicsBodyEvents;
  struct PhysicsContactEvents;
  struct PhysicsJointEvents;
  struct PhysicsSensorEvents;

  GF_PHYSICS_API void set_physics_length_units_per_meter(float length_units);

  using PhysicsOverlapResultFn = std::function<bool(PhysicsShape)>;
  using PhysicsCastResultFn = std::function<float(PhysicsShape, Vec2F, Vec2F, float)>;

  inline constexpr float PhysicsCastFilter = -1.0f;
  inline constexpr float PhysicsCastTerminate = 0.0f;
  inline constexpr float PhysicsCastContinue = 1.0f;

  struct GF_PHYSICS_API PhysicsCastResult {
    PhysicsShape shape;
    Vec2F point;
    Vec2F normal;
    float fraction;
    bool hit;
  };

  struct GF_PHYSICS_API PhysicsExplosion {
    uint64_t mask_bits;
    Vec2F position;
    float radius;
    float falloff;
    float impulse_per_length;
  };

  struct GF_PHYSICS_API PhysicsWorldData {
    Vec2F gravity = { 0.0f, -10.0f };
    bool enable_sleep = true;
    bool enable_continuous = true;
    void* user_data = nullptr;
  };

  class GF_PHYSICS_API PhysicsWorld : public Model {
  public:
    constexpr PhysicsWorld() = default;
    PhysicsWorld(const PhysicsWorldData& data);
    PhysicsWorld(const PhysicsWorld&) = delete;
    PhysicsWorld(PhysicsWorld&& other) noexcept;
    ~PhysicsWorld() override;

    PhysicsWorld& operator=(const PhysicsWorld&) = delete;
    PhysicsWorld& operator=(PhysicsWorld&& other) noexcept;

    PhysicsWorldId id() const;
    static PhysicsWorld from_id(PhysicsWorldId id);

    bool valid() const;
    void step(Time time, int sub_step_count = 4);

    void debug_draw(PhysicsDebug* debug, const PhysicsDebugOptions& options);

    PhysicsBodyEvents body_events() const;
    PhysicsSensorEvents sensor_events() const;
    PhysicsContactEvents contact_events() const;
    PhysicsJointEvents joint_events() const;

    void overlap_bounds(RectF bounds, PhysicsQueryFilter filter, PhysicsOverlapResultFn fn);
    void overlap_shape(Span<const Vec2F> shape, float radius, PhysicsQueryFilter filter, PhysicsOverlapResultFn fn);

    void cast_ray(Vec2F origin, Vec2F translation, PhysicsQueryFilter filter, PhysicsCastResultFn fn);
    PhysicsCastResult cast_ray_closest(Vec2F origin, Vec2F translation, PhysicsQueryFilter filter);
    void cast_shape(Span<const Vec2F> shape, float radius, Vec2F translation, PhysicsQueryFilter filter, PhysicsCastResultFn fn);

    bool is_sleep_enabled() const;
    void enable_sleep(bool flag);

    bool is_continuous_enabled() const;
    void enable_continuous(bool flag);

    Vec2F gravity() const;
    void set_gravity(Vec2F gravity);

    void explode(const PhysicsExplosion& explosion);

    void set_user_data(void* data);
    void* user_data();

    template<typename T>
    T* user_data_as()
    {
      return static_cast<T*>(user_data());
    }

    void update(Time time) override;

  private:
    constexpr PhysicsWorld(details::PhysicsOwner owner, b2WorldId id)
    : m_owner(owner)
    , m_id(id)
    {
    }

    details::PhysicsOwner m_owner = details::PhysicsOwner::None;
    b2WorldId m_id = {};
  };

}

#endif // GF_PHYSICS_WORLD_H
