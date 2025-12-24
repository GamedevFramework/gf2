// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsWorld.h>

#include <utility>

#include <gf2/core/Log.h>

#include <gf2/physics/PhysicsEvents.h>

namespace gf {

  namespace {

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    int g_world_loaded = 0;

    PhysicsSensorBeginTouchEvent to_event(const b2SensorBeginTouchEvent& raw)
    {
      return { PhysicsShape::from_id(details::to_id(raw.sensorShapeId)), PhysicsShape::from_id(details::to_id(raw.visitorShapeId)) };
    }

    PhysicsSensorEndTouchEvent to_event(const b2SensorEndTouchEvent& raw)
    {
      return { PhysicsShape::from_id(details::to_id(raw.sensorShapeId)), PhysicsShape::from_id(details::to_id(raw.visitorShapeId)) };
    }

    PhysicsContactBeginTouchEvent to_event(const b2ContactBeginTouchEvent& raw)
    {
      return {
        PhysicsShape::from_id(details::to_id(raw.shapeIdA)),
        PhysicsShape::from_id(details::to_id(raw.shapeIdB)),
        PhysicsContact::from_id(details::to_id(raw.contactId))
      };
    }

    PhysicsContactEndTouchEvent to_event(const b2ContactEndTouchEvent& raw)
    {
      return {
        PhysicsShape::from_id(details::to_id(raw.shapeIdA)),
        PhysicsShape::from_id(details::to_id(raw.shapeIdB)),
        PhysicsContact::from_id(details::to_id(raw.contactId))
      };
    }

    PhysicsContactHitEvent to_event(const b2ContactHitEvent& raw)
    {
      return {
        PhysicsShape::from_id(details::to_id(raw.shapeIdA)),
        PhysicsShape::from_id(details::to_id(raw.shapeIdB)),
        PhysicsContact::from_id(details::to_id(raw.contactId)),
        { raw.point.x, raw.point.y },
        { raw.normal.x, raw.normal.y },
        raw.approachSpeed
      };
    }

    PhysicsBodyMoveEvent to_event(const b2BodyMoveEvent& raw)
    {
      return {
        details::to_transform(raw.transform),
        PhysicsBody::from_id(details::to_id(raw.bodyId)),
        raw.fellAsleep,
        raw.userData
      };
    }

    PhysicsJointEvent to_event(const b2JointEvent& raw)
    {
      return { PhysicsJoint::from_id(details::to_id(raw.jointId)), raw.userData };
    }

    b2ShapeProxy to_raw(Span<const Vec2F> shape, float radius)
    {
      assert(shape.size() < B2_MAX_POLYGON_VERTICES);
      b2ShapeProxy proxy = {};

      for (std::size_t i = 0; i < shape.size(); ++i) {
        const Vec2F point = shape[i];
        proxy.points[i] = { point.x, point.y };
      }

      proxy.radius = radius;

      return proxy;
    }

    b2AABB to_aabb(const RectF& bounds)
    {
      const Vec2F min = bounds.min();
      const Vec2F max = bounds.max();
      return { { min.x, min.y }, { max.x, max. y } };
    }

    bool overlap_result_fn(b2ShapeId shape_id, void* context)
    {
      auto* fn = static_cast<PhysicsOverlapResultFn*>(context);
      return (*fn)(PhysicsShape::from_id(details::to_id(shape_id)));
    }

    float cast_result_fn(b2ShapeId shape_id, b2Vec2 point, b2Vec2 normal, float fraction, void* context)
    {
      auto* fn = static_cast<PhysicsCastResultFn*>(context);
      return (*fn)(PhysicsShape::from_id(details::to_id(shape_id)), { point.x, point.y }, { normal.x, normal.y }, fraction);
    }

    constexpr uint8_t PhysicsDebugAlpha = 0x80;

    void draw_polygon_fn(const b2Vec2* raw_vertices, int raw_vertex_count, b2HexColor raw_color, void* context)
    {
      assert(raw_vertex_count < B2_MAX_POLYGON_VERTICES);
      std::array<Vec2F, B2_MAX_POLYGON_VERTICES> vertices = {};

      for (int i = 0; i < raw_vertex_count; ++i) {
        const b2Vec2 raw = raw_vertices[i];
        vertices[i] = { raw.x, raw.y };
      }

      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_polygon(span(vertices.data(), raw_vertex_count), { raw_color, PhysicsDebugAlpha });
    }

    void draw_solid_polygon_fn(b2Transform raw_transform, const b2Vec2* raw_vertices, int raw_vertex_count, float radius, b2HexColor raw_color, void* context)
    {
      assert(raw_vertex_count < B2_MAX_POLYGON_VERTICES);
      std::array<Vec2F, B2_MAX_POLYGON_VERTICES> vertices = {};

      for (int i = 0; i < raw_vertex_count; ++i) {
        const b2Vec2 raw = b2TransformPoint(raw_transform, raw_vertices[i]);
        vertices[i] = { raw.x, raw.y };
      }

      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_solid_polygon(span(vertices.data(), raw_vertex_count), radius, { raw_color, PhysicsDebugAlpha });
    }

    void draw_circle_fn(b2Vec2 center, float radius, b2HexColor raw_color, void* context)
    {
      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_circle({ { center.x, center.y }, radius }, { raw_color, PhysicsDebugAlpha });
    }

    void draw_solid_circle_fn(b2Transform raw_transform, float radius, b2HexColor raw_color, void* context)
    {
      const b2Vec2 center = raw_transform.p;
      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_solid_circle({ { center.x, center.y }, radius }, b2Rot_GetAngle(raw_transform.q), { raw_color, PhysicsDebugAlpha });
    }

    void draw_solid_capsule_fn(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor raw_color, void* context)
    {
      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_solid_capsule({ { p1.x, p1.y }, { p2.x, p2.y }, radius }, { raw_color, PhysicsDebugAlpha });
    }

    void draw_line_fn(b2Vec2 p1, b2Vec2 p2, b2HexColor raw_color, void* context)
    {
      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_line({ { p1.x, p1.y }, { p2.x, p2.y } }, { raw_color, PhysicsDebugAlpha });
    }

    void draw_transform_fn(b2Transform raw_transform, void* context)
    {
      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_transform(details::to_transform(raw_transform));
    }

    void draw_point_fn(b2Vec2 p, float size, b2HexColor raw_color, void* context)
    {
      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_point({ { p.x, p.y }, size}, { raw_color, PhysicsDebugAlpha });
    }

    void draw_string_fn(b2Vec2 p, const char* s, b2HexColor raw_color, void* context)
    {
      auto* debug = static_cast<PhysicsDebug*>(context);
      debug->draw_string({ p.x, p.y }, s, { raw_color, PhysicsDebugAlpha });
    }

  }

  void set_physics_length_units_per_meter(float length_units)
  {
    if (g_world_loaded > 0) {
      Log::warning("gf::set_length_units_per_meter() has been called but a physics world has already been created.");
    }

    b2SetLengthUnitsPerMeter(length_units);
  }

  PhysicsWorld::PhysicsWorld(const PhysicsWorldData& data)
  : m_owner(details::PhysicsOwner::Object)
  {
    ++g_world_loaded;

    b2WorldDef def = b2DefaultWorldDef();
    def.gravity = { data.gravity.x, data.gravity.y };
    def.enableSleep = data.enable_sleep;
    def.enableContinuous = data.enable_continuous;
    def.userData = data.user_data;

    m_id = b2CreateWorld(&def);
  }

  PhysicsWorld::PhysicsWorld(PhysicsWorld&& other) noexcept
  : m_owner(std::exchange(other.m_owner, details::PhysicsOwner::None))
  , m_id(std::exchange(other.m_id, b2_nullWorldId))
  {
  }

  PhysicsWorld::~PhysicsWorld()
  {
    if (m_owner == details::PhysicsOwner::Object) {
      b2DestroyWorld(m_id);
    }

    --g_world_loaded;
  }

  PhysicsWorld& PhysicsWorld::operator=(PhysicsWorld&& other) noexcept
  {
    std::swap(other.m_owner, m_owner);
    std::swap(other.m_id, m_id);
    return *this;
  }

  PhysicsWorldId PhysicsWorld::id() const
  {
    return details::to_id(m_id);
  }

  PhysicsWorld PhysicsWorld::from_id(PhysicsWorldId id)
  {
    return { details::PhysicsOwner::None, details::to_raw(id) };
  }

  bool PhysicsWorld::valid() const
  {
    return b2World_IsValid(m_id);
  }

  void PhysicsWorld::step(Time time, int sub_step_count)
  {
    b2World_Step(m_id, time.as_seconds(), sub_step_count);
  }

  void PhysicsWorld::debug_draw(PhysicsDebug* debug, const PhysicsDebugOptions& options)
  {
    b2DebugDraw raw = b2DefaultDebugDraw();
    raw.DrawPolygonFcn = draw_polygon_fn;
    raw.DrawSolidPolygonFcn = draw_solid_polygon_fn;
    raw.DrawCircleFcn = draw_circle_fn;
    raw.DrawSolidCircleFcn = draw_solid_circle_fn;
    raw.DrawSolidCapsuleFcn = draw_solid_capsule_fn;
    raw.DrawLineFcn = draw_line_fn;
    raw.DrawTransformFcn = draw_transform_fn;
    raw.DrawPointFcn = draw_point_fn;
    raw.DrawStringFcn = draw_string_fn;
    raw.drawingBounds = to_aabb(options.bounds);
    raw.forceScale = options.force_scale;
    raw.jointScale = options.joint_scale;
    raw.drawShapes = options.features.test(PhysicsDebugFeature::DrawShapes);
    raw.drawJoints = options.features.test(PhysicsDebugFeature::DrawJoints);
    raw.drawJointExtras = options.features.test(PhysicsDebugFeature::DrawJointsExtra);
    raw.drawBounds = options.features.test(PhysicsDebugFeature::DrawBounds);
    raw.drawMass = options.features.test(PhysicsDebugFeature::DrawMass);
    raw.drawBodyNames = options.features.test(PhysicsDebugFeature::DrawBodyNames);
    raw.drawContactPoints = options.features.test(PhysicsDebugFeature::DrawContactPoints);
    raw.drawGraphColors = options.features.test(PhysicsDebugFeature::DrawGraphColors);
    raw.drawContactFeatures = options.features.test(PhysicsDebugFeature::DrawContactFeatures);
    raw.drawContactNormals = options.features.test(PhysicsDebugFeature::DrawContactNormals);
    raw.drawContactForces = options.features.test(PhysicsDebugFeature::DrawContactForces);
    raw.drawFrictionForces = options.features.test(PhysicsDebugFeature::DrawFrictionForces);
    raw.drawIslands = options.features.test(PhysicsDebugFeature::DrawIslands);
    raw.context = debug;
    b2World_Draw(m_id, &raw);
  }

  PhysicsBodyEvents PhysicsWorld::body_events() const
  {
    const b2BodyEvents raw = b2World_GetBodyEvents(m_id);
    PhysicsBodyEvents events;

    events.move_events.reserve(raw.moveCount);

    for (int i = 0; i < raw.moveCount; ++i) {
      events.move_events.push_back(to_event(raw.moveEvents[i]));
    }

    return events;
  }

  PhysicsSensorEvents PhysicsWorld::sensor_events() const
  {
    const b2SensorEvents raw = b2World_GetSensorEvents(m_id);
    PhysicsSensorEvents events;

    events.begin_events.reserve(raw.beginCount);

    for (int i = 0; i < raw.beginCount; ++i) {
      events.begin_events.push_back(to_event(raw.beginEvents[i]));
    }

    events.end_events.reserve(raw.endCount);

    for (int i = 0; i < raw.endCount; ++i) {
      events.end_events.push_back(to_event(raw.endEvents[i]));
    }

    return events;
  }

  PhysicsContactEvents PhysicsWorld::contact_events() const
  {
    const b2ContactEvents raw = b2World_GetContactEvents(m_id);
    PhysicsContactEvents events;

    events.begin_events.reserve(raw.beginCount);

    for (int i = 0; i < raw.beginCount; ++i) {
      events.begin_events.push_back(to_event(raw.beginEvents[i]));
    }

    events.end_events.reserve(raw.endCount);

    for (int i = 0; i < raw.endCount; ++i) {
      events.end_events.push_back(to_event(raw.endEvents[i]));
    }

    events.hit_events.reserve(raw.hitCount);

    for (int i = 0; i < raw.hitCount; ++i) {
      events.hit_events.push_back(to_event(raw.hitEvents[i]));
    }

    return events;
  }

  PhysicsJointEvents PhysicsWorld::joint_events() const
  {
    const b2JointEvents raw = b2World_GetJointEvents(m_id);
    PhysicsJointEvents events;

    events.joint_events.reserve(raw.count);

    for (int i = 0; i < raw.count; ++i) {
      events.joint_events.push_back(to_event(raw.jointEvents[i]));
    }

    return events;
  }

  void PhysicsWorld::overlap_bounds(RectF bounds, PhysicsQueryFilter filter, PhysicsOverlapResultFn fn)
  {
    b2World_OverlapAABB(m_id, to_aabb(bounds), details::to_raw(filter), overlap_result_fn, &fn);
  }

  void PhysicsWorld::overlap_shape(Span<const Vec2F> shape, float radius, PhysicsQueryFilter filter, PhysicsOverlapResultFn fn)
  {
    const b2ShapeProxy proxy = to_raw(shape, radius);
    b2World_OverlapShape(m_id, &proxy, details::to_raw(filter), overlap_result_fn, &fn);
  }

  void PhysicsWorld::cast_ray(Vec2F origin, Vec2F translation, PhysicsQueryFilter filter, PhysicsCastResultFn fn)
  {
    b2World_CastRay(m_id, { origin.x, origin.y }, { translation.x, translation.y }, details::to_raw(filter), cast_result_fn, &fn);
  }

  PhysicsCastResult PhysicsWorld::cast_ray_closest(Vec2F origin, Vec2F translation, PhysicsQueryFilter filter)
  {
    const b2RayResult raw = b2World_CastRayClosest(m_id, { origin.x, origin.y }, { translation.x, translation.y }, details::to_raw(filter));
    return { PhysicsShape::from_id(details::to_id(raw.shapeId)), { raw.point.x, raw.point.y }, { raw.normal.x, raw.normal.y }, raw.fraction, raw.hit };
  }

  void PhysicsWorld::cast_shape(Span<const Vec2F> shape, float radius, Vec2F translation, PhysicsQueryFilter filter, PhysicsCastResultFn fn)
  {
    const b2ShapeProxy proxy = to_raw(shape, radius);
    b2World_CastShape(m_id, &proxy, { translation.x, translation.y }, details::to_raw(filter), cast_result_fn, &fn);
  }

  bool PhysicsWorld::is_sleep_enabled() const
  {
    return b2World_IsSleepingEnabled(m_id);
  }

  void PhysicsWorld::enable_sleep(bool flag)
  {
    b2World_EnableSleeping(m_id, flag);
  }

  bool PhysicsWorld::is_continuous_enabled() const
  {
    return b2World_IsContinuousEnabled(m_id);
  }

  void PhysicsWorld::enable_continuous(bool flag)
  {
    b2World_EnableContinuous(m_id, flag);
  }

  Vec2F PhysicsWorld::gravity() const
  {
    const b2Vec2 raw = b2World_GetGravity(m_id);
    return { raw.x, raw.y };
  }

  void PhysicsWorld::set_gravity(Vec2F gravity)
  {
    b2World_SetGravity(m_id, { gravity.x, gravity.y });
  }

  void PhysicsWorld::explode(const PhysicsExplosion& explosion)
  {
    b2ExplosionDef def = {};
    def.maskBits = explosion.mask_bits;
    def.position = { explosion.position.x, explosion.position.y };
    def.radius = explosion.radius;
    def.falloff = explosion.falloff;
    def.impulsePerLength = explosion.impulse_per_length;

    b2World_Explode(m_id, &def);
  }

  void PhysicsWorld::set_user_data(void* data)
  {
    b2World_SetUserData(m_id, data);
  }

  void* PhysicsWorld::user_data()
  {
    return b2World_GetUserData(m_id);
  }

  void PhysicsWorld::update(Time time)
  {
    step(time);
  }

}
