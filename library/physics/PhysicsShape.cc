// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsShape.h>

#include <cassert>

#include <utility>

#include <gf2/core/Log.h>

#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsChain.h>
#include <gf2/physics/PhysicsContact.h>
#include <gf2/physics/PhysicsWorld.h>

namespace gf {

  namespace {

    b2Polygon to_raw(Span<const Vec2F> polygon, float radius)
    {
      std::vector<b2Vec2> raw_points;
      raw_points.reserve(polygon.size());

      for (const Vec2F point :polygon) {
        raw_points.push_back({ point.x, point.y });
      }

      const b2Hull raw_hull = b2ComputeHull(raw_points.data(), static_cast<int>(raw_points.size()));

      if (raw_hull.count == 0) {
        Log::fatal("Could not create hull.");
      }

      return b2MakePolygon(&raw_hull, radius);
    }

    constexpr b2Circle to_raw(const CircF& circle)
    {
      return { { circle.center.x, circle.center.y }, circle.radius };
    }

    constexpr b2Segment to_raw(const SegmentF& segment)
    {
      return {
        { segment.p0.x, segment.p0.y },
        { segment.p1.x, segment.p1.y }
      };
    }

    constexpr b2Capsule to_raw(const CapsuleF& capsule)
    {
      return {
        { capsule.center0.x , capsule.center0.y },
        { capsule.center1.x , capsule.center1.y },
        capsule.radius
      };
    }

    b2ShapeDef to_raw(const PhysicsShapeData& data)
    {
      b2ShapeDef raw = b2DefaultShapeDef();
      raw.material = details::to_raw(data.material);
      raw.density = data.density;
      raw.filter = details::to_raw(data.filter);
      raw.enableCustomFiltering = data.enable_custom_filtering;
      raw.isSensor = data.sensor;
      raw.enableSensorEvents = data.enable_sensor_events;
      raw.enableContactEvents = data.enable_contact_events;
      raw.enableHitEvents = data.enable_hit_events;
      raw.userData = data.user_data;
      return raw;
    }

  }

  PhysicsShape::PhysicsShape(PhysicsShape&& other) noexcept
  : m_owner(std::exchange(other.m_owner, details::PhysicsOwner::None))
  , m_id(std::exchange(other.m_id, b2_nullShapeId))
  {
  }

  PhysicsShape::~PhysicsShape()
  {
    if (m_owner == details::PhysicsOwner::Object) {
      b2DestroyShape(m_id, true);
    }
  }

  PhysicsShape& PhysicsShape::operator=(PhysicsShape&& other) noexcept
  {
    std::swap(other.m_owner, m_owner);
    std::swap(other.m_id, m_id);
    return *this;
  }

  PhysicsShapeId PhysicsShape::id() const
  {
    return details::to_id(m_id);
  }

  PhysicsShape PhysicsShape::from_id(PhysicsShapeId id)
  {
    return { details::PhysicsOwner::None, details::to_raw(id) };
  }

  bool PhysicsShape::valid() const
  {
    return b2Shape_IsValid(m_id);
  }

  PhysicsShapeType PhysicsShape::type() const
  {
    return static_cast<PhysicsShapeType>(b2Shape_GetType(m_id));
  }

  PhysicsBody PhysicsShape::body() const
  {
    return PhysicsBody::from_id(details::to_id(b2Shape_GetBody(m_id)));
  }

  PhysicsWorld PhysicsShape::world() const
  {
    return PhysicsWorld::from_id(details::to_id(b2Shape_GetWorld(m_id)));
  }

  bool PhysicsShape::sensor() const
  {
    return b2Shape_IsSensor(m_id);
  }

  float PhysicsShape::density() const
  {
    return b2Shape_GetDensity(m_id);
  }

  void PhysicsShape::set_density(float density)
  {
    b2Shape_SetDensity(m_id, density, true);
  }

  float PhysicsShape::friction() const
  {
    return b2Shape_GetFriction(m_id);
  }

  void PhysicsShape::set_friction(float friction)
  {
    b2Shape_SetFriction(m_id, friction);
  }

  float PhysicsShape::restitution() const
  {
    return b2Shape_GetRestitution(m_id);
  }

  void PhysicsShape::set_restitution(float restitution)
  {
    b2Shape_SetRestitution(m_id, restitution);
  }

  uint64_t PhysicsShape::user_material() const
  {
    return b2Shape_GetUserMaterial(m_id);
  }

  void PhysicsShape::set_user_material(uint64_t material)
  {
    b2Shape_SetUserMaterial(m_id, material);
  }

  PhysicsSurfaceMaterial PhysicsShape::surface_material() const
  {
    return details::to_material(b2Shape_GetSurfaceMaterial(m_id));
  }

  void PhysicsShape::set_surface_material(const PhysicsSurfaceMaterial& material)
  {
    const b2SurfaceMaterial raw = details::to_raw(material);
    b2Shape_SetSurfaceMaterial(m_id, &raw);
  }

  PhysicsFilter PhysicsShape::filter() const
  {
    return details::to_filter(b2Shape_GetFilter(m_id));
  }

  void PhysicsShape::set_filter(const PhysicsFilter& filter)
  {
    b2Shape_SetFilter(m_id, details::to_raw(filter));
  }

  bool PhysicsShape::sensor_events_enabled() const
  {
    return b2Shape_AreSensorEventsEnabled(m_id);
  }

  void PhysicsShape::enable_sensor_events(bool flag)
  {
    b2Shape_EnableSensorEvents(m_id, flag);
  }

  bool PhysicsShape::contact_events_enabled() const
  {
    return b2Shape_AreContactEventsEnabled(m_id);
  }

  void PhysicsShape::enable_contact_events(bool flag)
  {
    b2Shape_EnableContactEvents(m_id, flag);
  }

  bool PhysicsShape::hit_events_enabled() const
  {
    return b2Shape_AreHitEventsEnabled(m_id);
  }

  void PhysicsShape::enable_hit_events(bool flag)
  {
    b2Shape_EnableHitEvents(m_id, flag);
  }

  bool PhysicsShape::test_point(Vec2F point)
  {
    return b2Shape_TestPoint(m_id, { point.x, point.y });
  }

  PhysicsCastOutput PhysicsShape::ray_cast(const PhysicsRayCastInput& input)
  {
    b2RayCastInput raw_input = {};
    raw_input.origin = { input.origin.x, input.origin.y };
    raw_input.translation = { input.translation.x, input.translation.y };
    raw_input.maxFraction = input.max_fraction;

    const b2CastOutput raw = b2Shape_RayCast(m_id, &raw_input);
    return {
      { raw.normal.x, raw.normal.y },
      { raw.point.x, raw.point.y },
      raw.fraction,
      raw.iterations,
      raw.hit
    };
  }

  CircF PhysicsShape::circle() const
  {
    const b2Circle raw = b2Shape_GetCircle(m_id);
    return CircF::from_center_radius({ raw.center.x, raw.center.y }, raw.radius);
  }

  SegmentF PhysicsShape::segment() const
  {
    const b2Segment raw = b2Shape_GetSegment(m_id);
    return { { raw.point1.x, raw.point1.y }, { raw.point2.x, raw.point2.y }};
  }

  PhysicsChainSegment PhysicsShape::chain_segment() const
  {
    const b2ChainSegment raw = b2Shape_GetChainSegment(m_id);
    return {
      { raw.ghost1.x, raw.ghost1.y },
      { { raw.segment.point1.x, raw.segment.point1.y }, { raw.segment.point2.x, raw.segment.point2.y } },
      { raw.ghost2.x, raw.ghost2.y }
    };
  }

  CapsuleF PhysicsShape::capsule() const
  {
    const b2Capsule raw = b2Shape_GetCapsule(m_id);
    return { { raw.center1.x, raw.center1.y }, { raw.center2.x, raw.center2.y }, raw.radius };
  }

  std::vector<Vec2F> PhysicsShape::polygon() const
  {
    const b2Polygon raw = b2Shape_GetPolygon(m_id);
    std::vector<Vec2F> polygon;
    polygon.reserve(raw.count);

    for (int i = 0; i < raw.count; ++i) {
      polygon.emplace_back(raw.vertices[i].x, raw.vertices[i].y); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return polygon;
  }

  void PhysicsShape::set_circle(const CircF& circle)
  {
    const b2Circle raw = to_raw(circle);
    b2Shape_SetCircle(m_id, &raw);
  }

  void PhysicsShape::set_segment(const SegmentF& segment)
  {
    const b2Segment raw = to_raw(segment);
    b2Shape_SetSegment(m_id, &raw);
  }

  void PhysicsShape::set_capsule(const CapsuleF& capsule)
  {
    const b2Capsule raw = to_raw(capsule);
    b2Shape_SetCapsule(m_id, &raw);
  }

  void PhysicsShape::set_polygon(Span<const Vec2F> polygon, float radius)
  {
    const b2Polygon raw_polygon = to_raw(polygon, radius);
    b2Shape_SetPolygon(m_id, &raw_polygon);
  }

  void PhysicsShape::set_square(float half_size)
  {
    const b2Polygon raw_polygon = b2MakeSquare(half_size);
    b2Shape_SetPolygon(m_id, &raw_polygon);
  }

  void PhysicsShape::set_box(Vec2F half_size)
  {
    const b2Polygon raw_polygon = b2MakeBox(half_size.w, half_size.h);
    b2Shape_SetPolygon(m_id, &raw_polygon);
  }

  void PhysicsShape::set_box(Vec2F half_size, Vec2F center, PhysicsRotation rotation)
  {
    const b2Polygon raw_polygon = b2MakeOffsetBox(half_size.w, half_size.h, { center.x, center.y }, { rotation.cosine, rotation.sine });
    b2Shape_SetPolygon(m_id, &raw_polygon);
  }

  void PhysicsShape::set_rounded_box(Vec2F half_size, float radius)
  {
    const b2Polygon raw_polygon = b2MakeRoundedBox(half_size.w, half_size.h, radius);
    b2Shape_SetPolygon(m_id, &raw_polygon);
  }

  void PhysicsShape::set_rounded_box(Vec2F half_size, Vec2F center, PhysicsRotation rotation, float radius)
  {
    const b2Polygon raw_polygon = b2MakeOffsetRoundedBox(half_size.w, half_size.h, { center.x, center.y }, { rotation.cosine, rotation.sine }, radius);
    b2Shape_SetPolygon(m_id, &raw_polygon);
  }

  PhysicsChain PhysicsShape::parent_chain() const
  {
    return PhysicsChain::from_id(details::to_id(b2Shape_GetParentChain(m_id)));
  }

  std::vector<PhysicsContactFeatures> PhysicsShape::contact_features() const
  {
    const int capacity = b2Shape_GetContactCapacity(m_id);

    std::vector<b2ContactData> raw_contacts(capacity);
    const int count = b2Shape_GetContactData(m_id, raw_contacts.data(), capacity);
    assert(count <= capacity);
    raw_contacts.resize(count);

    std::vector<PhysicsContactFeatures> contacts;
    contacts.reserve(count);

    for (const b2ContactData& raw : raw_contacts) {
      contacts.push_back(details::to_contact_features(raw));
    }

    return contacts;
  }

  std::vector<PhysicsShape> PhysicsShape::sensors() const
  {
    const int capacity = b2Shape_GetSensorCapacity(m_id);

    if (capacity == 0) {
      return {};
    }

    std::vector<b2ShapeId> raw_sensors(capacity);
    const int count = b2Shape_GetSensorData(m_id, raw_sensors.data(), capacity);
    assert(count <= capacity);
    raw_sensors.resize(count);

    std::vector<PhysicsShape> sensors;
    sensors.reserve(count);

    for (const b2ShapeId raw : raw_sensors) {
      sensors.push_back(PhysicsShape::from_id(details::to_id(raw)));
    }

    return sensors;
  }

  RectF PhysicsShape::bouding_box() const
  {
    const b2AABB raw = b2Shape_GetAABB(m_id);
    return RectF::from_min_max({ raw.lowerBound.x, raw.lowerBound.y }, { raw.upperBound.x, raw.upperBound.y });
  }

  PhysicsMassData PhysicsShape::compute_mass_data() const
  {
    const b2MassData raw = b2Shape_ComputeMassData(m_id);
    return { raw.mass, { raw.center.x, raw.center.y }, raw.rotationalInertia };
  }

  Vec2F PhysicsShape::closest_point(Vec2F target)
  {
    const b2Vec2 raw = b2Shape_GetClosestPoint(m_id, { target.x, target.y });
    return { raw.x, raw.y };
  }

  void PhysicsShape::apply_wind(Vec2F wind, float drag, float lift)
  {
    b2Shape_ApplyWind(m_id, { wind.x, wind.y }, drag, lift, true);
  }

  void PhysicsShape::set_user_data(void* data)
  {
    b2Shape_SetUserData(m_id, data);
  }

  void* PhysicsShape::user_data()
  {
    return b2Shape_GetUserData(m_id);
  }

  PhysicsShape PhysicsShape::create_circle(PhysicsBody* body, const PhysicsShapeData& data, const CircF& circle)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Circle raw = to_raw(circle);
    return { details::PhysicsOwner::Object, b2CreateCircleShape(details::to_raw(body->id()), &def, &raw) };
  }

  PhysicsShape PhysicsShape::create_segment(PhysicsBody* body, const PhysicsShapeData& data, const SegmentF& segment)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Segment raw = to_raw(segment);
    return { details::PhysicsOwner::Object, b2CreateSegmentShape(details::to_raw(body->id()), &def, &raw) };
  }

  PhysicsShape PhysicsShape::create_capsule(PhysicsBody* body, const PhysicsShapeData& data, const CapsuleF& capsule)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Capsule raw = to_raw(capsule);
    return { details::PhysicsOwner::Object, b2CreateCapsuleShape(details::to_raw(body->id()), &def, &raw) };
  }

  PhysicsShape PhysicsShape::create_polygon(PhysicsBody* body, const PhysicsShapeData& data, Span<const Vec2F> polygon, float radius)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Polygon raw = to_raw(polygon, radius);
    return { details::PhysicsOwner::Object, b2CreatePolygonShape(details::to_raw(body->id()), &def, &raw) };
  }

  PhysicsShape PhysicsShape::create_square(PhysicsBody* body, const PhysicsShapeData& data, float half_size)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Polygon raw = b2MakeSquare(half_size);
    return { details::PhysicsOwner::Object, b2CreatePolygonShape(details::to_raw(body->id()), &def, &raw) };
  }

  PhysicsShape PhysicsShape::create_box(PhysicsBody* body, const PhysicsShapeData& data, Vec2F half_size)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Polygon raw = b2MakeBox(half_size.w, half_size.h);
    return { details::PhysicsOwner::Object, b2CreatePolygonShape(details::to_raw(body->id()), &def, &raw) };
  }

  PhysicsShape PhysicsShape::create_box(PhysicsBody* body, const PhysicsShapeData& data, Vec2F half_size, Vec2F center, PhysicsRotation rotation)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Polygon raw = b2MakeOffsetBox(half_size.w, half_size.h, { center.x, center.y }, { rotation.cosine, rotation.sine });
    return { details::PhysicsOwner::Object, b2CreatePolygonShape(details::to_raw(body->id()), &def, &raw) };
  }

  PhysicsShape PhysicsShape::create_rounded_box(PhysicsBody* body, const PhysicsShapeData& data, Vec2F half_size, float radius)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Polygon raw = b2MakeRoundedBox(half_size.w, half_size.h, radius);
    return { details::PhysicsOwner::Object, b2CreatePolygonShape(details::to_raw(body->id()), &def, &raw) };
  }

  PhysicsShape PhysicsShape::create_rounded_box(PhysicsBody* body, const PhysicsShapeData& data, Vec2F half_size, Vec2F center, PhysicsRotation rotation, float radius)
  {
    assert(body != nullptr);
    assert(body->valid());

    const b2ShapeDef def = to_raw(data);
    const b2Polygon raw = b2MakeOffsetRoundedBox(half_size.w, half_size.h, { center.x, center.y }, { rotation.cosine, rotation.sine }, radius);
    return { details::PhysicsOwner::Object, b2CreatePolygonShape(details::to_raw(body->id()), &def, &raw) };
  }


}

