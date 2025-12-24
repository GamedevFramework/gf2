// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_SHAPE_H
#define GF_PHYSICS_SHAPE_H

#include <cstdint>

#include <type_traits>
#include <vector>

#include <box2d/box2d.h>

#include <gf2/core/Circ.h>
#include <gf2/core/Geometry.h>
#include <gf2/core/Rect.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsId.h"
#include "PhysicsFilter.h"
#include "PhysicsMassData.h"
#include "PhysicsOwner.h"
#include "PhysicsSurfaceMaterial.h"
#include "PhysicsTransform.h"

namespace gf {
  struct PhysicsContactFeatures;

  class PhysicsBody;
  class PhysicsWorld;

  struct GF_PHYSICS_API PhysicsShapeData {
    PhysicsSurfaceMaterial material;
    float density = 1.0f;
    PhysicsFilter filter;
    bool enable_custom_filtering = false;
    bool sensor = false;
    bool enable_sensor_events = false;
    bool enable_contact_events = false;
    bool enable_hit_events = false;
    void* user_data = nullptr;
  };

  // NOLINTNEXTLINE(performance-enum-size)
  enum class PhysicsShapeType : std::underlying_type_t<b2ShapeType> {
    Circle = b2_circleShape,
    Capsule = b2_capsuleShape,
    Segment = b2_segmentShape,
    Polygon = b2_polygonShape,
    ChainSegment = b2_chainSegmentShape,
  };

  struct GF_PHYSICS_API PhysicsRayCastInput {
    Vec2F origin;
    Vec2F translation;
    float max_fraction;
  };

  struct GF_PHYSICS_API PhysicsCastOutput {
    Vec2F normal;
    Vec2F point;
    float fraction;
    int iterations;
    bool hit;
  };

  class GF_PHYSICS_API PhysicsShape {
  public:
    PhysicsShape() = default;
    PhysicsShape(const PhysicsShape&) = delete;
    PhysicsShape(PhysicsShape&& other) noexcept;
    ~PhysicsShape();

    PhysicsShape& operator=(const PhysicsShape&) = delete;
    PhysicsShape& operator=(PhysicsShape&& other) noexcept;

    PhysicsShapeId id() const;
    static PhysicsShape from_id(PhysicsShapeId id);

    bool valid() const;

    PhysicsShapeType type() const;

    PhysicsBody body() const;
    PhysicsWorld world() const;

    bool sensor() const;

    float density() const;
    void set_density(float density);

    float friction() const;
    void set_friction(float friction);

    float restitution() const;
    void set_restitution(float restitution);

    uint64_t user_material() const;
    void set_user_material(uint64_t material);

    PhysicsSurfaceMaterial surface_material() const;
    void set_surface_material(const PhysicsSurfaceMaterial& material);

    PhysicsFilter filter() const;
    void set_filter(const PhysicsFilter& filter);

    bool sensor_events_enabled() const;
    void enable_sensor_events(bool flag);

    bool contact_events_enabled() const;
    void enable_contact_events(bool flag);

    bool hit_events_enabled() const;
    void enable_hit_events(bool flag);

    bool test_point(Vec2F point);
    PhysicsCastOutput ray_cast(const PhysicsRayCastInput& input);

    void set_circle(const CircF& circle);
    void set_segment(const SegmentF& segment);
    void set_capsule(const CapsuleF& capsule);
    void set_polygon(Span<const Vec2F> polygon, float radius = 0.0f);
    void set_square(float half_size);
    void set_box(Vec2F half_size);
    void set_box(Vec2F half_size, Vec2F center, PhysicsRotation rotation);
    void set_rounded_box(Vec2F half_size, float radius);
    void set_rounded_box(Vec2F half_size, Vec2F center, PhysicsRotation rotation, float radius);

    // TODO: parent_chain();

    std::vector<PhysicsContactFeatures> contact_features() const;
    std::vector<PhysicsShape> sensors() const;

    RectF bouding_box() const;

    PhysicsMassData compute_mass_data() const;

    Vec2F closest_point(Vec2F target);

    void apply_wind(Vec2F wind, float drag, float lift);

    void set_user_data(void* data);
    void* user_data();

    template<typename T>
    T* user_data_as()
    {
      return static_cast<T*>(user_data());
    }

    static PhysicsShape create_circle(PhysicsBody* body, const PhysicsShapeData& data, const CircF& circle);
    static PhysicsShape create_segment(PhysicsBody* body, const PhysicsShapeData& data, const SegmentF& segment);
    static PhysicsShape create_capsule(PhysicsBody* body, const PhysicsShapeData& data, const CapsuleF& capsule);
    static PhysicsShape create_polygon(PhysicsBody* body, const PhysicsShapeData& data, Span<const Vec2F> polygon, float radius = 0.0f);
    static PhysicsShape create_square(PhysicsBody* body, const PhysicsShapeData& data, float half_size);
    static PhysicsShape create_box(PhysicsBody* body, const PhysicsShapeData& data, Vec2F half_size);
    static PhysicsShape create_box(PhysicsBody* body, const PhysicsShapeData& data, Vec2F half_size, Vec2F center, PhysicsRotation rotation);
    static PhysicsShape create_rounded_box(PhysicsBody* body, const PhysicsShapeData& data, Vec2F half_size, float radius);
    static PhysicsShape create_rounded_box(PhysicsBody* body, const PhysicsShapeData& data, Vec2F half_size, Vec2F center, PhysicsRotation rotation, float radius);

  private:
    constexpr PhysicsShape(details::PhysicsOwner owner, b2ShapeId id)
    : m_owner(owner)
    , m_id(id)
    {
    }

    details::PhysicsOwner m_owner = details::PhysicsOwner::None;
    b2ShapeId m_id = {};
  };


}

#endif // GF_PHYSICS_SHAPE_H
