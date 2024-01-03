// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/physics/PhysicsWorld.h>
// clang-format on

#include <vector>

#include <gf2/physics/PhysicsArbiter.h>
#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsCollisionHandler.h>
#include <gf2/physics/PhysicsConstraint.h>
#include <gf2/physics/PhysicsDebug.h>
#include <gf2/physics/PhysicsShape.h>

namespace gf {

  PhysicsWorld::PhysicsWorld()
  : m_space(cpSpaceNew())
  {
    static_body().m_body.set_refcount(2); // this one + static body in world
  }

  namespace {
    // shape

    void dispose_shape_post_step(cpSpace* space, void* key, [[maybe_unused]] void* data)
    {
      auto* shape = static_cast<cpShape*>(key);
      cpSpaceRemoveShape(space, shape);

      PhysicsShape::Handle handle(details::PhysicsNoReference, shape);
      handle.unreference();
    }

    void dispose_shape_iterator(cpShape* shape, void* data)
    {
      auto* space = static_cast<cpSpace*>(data);
      cpSpaceAddPostStepCallback(space, dispose_shape_post_step, static_cast<void*>(shape), nullptr);
    }

    // constraint

    void dispose_constraint_post_step(cpSpace* space, void* key, [[maybe_unused]] void* data)
    {
      auto* constraint = static_cast<cpConstraint*>(key);
      cpSpaceRemoveConstraint(space, constraint);

      PhysicsConstraint::Handle handle(details::PhysicsNoReference, constraint);
      handle.unreference();
    }

    void dispose_constraint_iterator(cpConstraint* constraint, void* data)
    {
      auto* space = static_cast<cpSpace*>(data);
      cpSpaceAddPostStepCallback(space, dispose_constraint_post_step, static_cast<void*>(constraint), nullptr);
    }

    // body

    void dispose_body_post_step(cpSpace* space, void* key, [[maybe_unused]] void* data)
    {
      auto* body = static_cast<cpBody*>(key);
      cpSpaceRemoveBody(space, body);

      PhysicsBody::Handle handle(details::PhysicsNoReference, body);
      handle.unreference();
    }

    void dispose_body_iterator(cpBody* body, void* data)
    {
      auto* space = static_cast<cpSpace*>(data);
      cpSpaceAddPostStepCallback(space, dispose_body_post_step, static_cast<void*>(body), nullptr);
    }
  }

  PhysicsWorld::~PhysicsWorld()
  {
    cpSpace* space = m_space;

    if (m_space == nullptr) {
      return;
    }

    if (m_space.refcount() == 1) {
      cpSpaceEachShape(space, dispose_shape_iterator, static_cast<void*>(space));
      cpSpaceEachConstraint(space, dispose_constraint_iterator, static_cast<void*>(space));
      cpSpaceEachBody(space, dispose_body_iterator, static_cast<void*>(space));
    }
  }

  int PhysicsWorld::iterations() const
  {
    return cpSpaceGetIterations(m_space);
  }

  void PhysicsWorld::set_iterations(int iterations)
  {
    cpSpaceSetIterations(m_space, iterations);
  }

  Vec2F PhysicsWorld::gravity() const
  {
    auto gravity = cpSpaceGetGravity(m_space);
    return gf::vec(gravity.x, gravity.y);
  }

  void PhysicsWorld::set_gravity(Vec2F gravity)
  {
    cpSpaceSetGravity(m_space, cpv(gravity.x, gravity.y));
  }

  float PhysicsWorld::damping() const
  {
    return static_cast<float>(cpSpaceGetDamping(m_space));
  }

  void PhysicsWorld::set_damping(float damping)
  {
    cpSpaceSetDamping(m_space, damping);
  }

  float PhysicsWorld::idle_speed_threshold() const
  {
    return static_cast<float>(cpSpaceGetIdleSpeedThreshold(m_space));
  }

  void PhysicsWorld::set_idle_speed_threshold(float threshold)
  {
    cpSpaceSetIdleSpeedThreshold(m_space, threshold);
  }

  float PhysicsWorld::sleep_time_threshold() const
  {
    return static_cast<float>(cpSpaceGetSleepTimeThreshold(m_space));
  }

  void PhysicsWorld::set_sleep_time_threshold(float threshold)
  {
    cpSpaceSetSleepTimeThreshold(m_space, threshold);
  }

  float PhysicsWorld::collision_slop() const
  {
    return static_cast<float>(cpSpaceGetCollisionSlop(m_space));
  }

  void PhysicsWorld::set_collision_slop(float slop)
  {
    cpSpaceSetCollisionSlop(m_space, slop);
  }

  float PhysicsWorld::collision_bias() const
  {
    return static_cast<float>(cpSpaceGetCollisionBias(m_space));
  }

  void PhysicsWorld::set_collision_bias(float bias)
  {
    cpSpaceSetCollisionBias(m_space, bias);
  }

  unsigned PhysicsWorld::collision_persistence() const
  {
    return cpSpaceGetCollisionPersistence(m_space);
  }

  void PhysicsWorld::set_collision_persistence(unsigned persistence)
  {
    cpSpaceSetCollisionPersistence(m_space, persistence);
  }

  float PhysicsWorld::current_time_step() const
  {
    return static_cast<float>(cpSpaceGetCurrentTimeStep(m_space));
  }

  bool PhysicsWorld::locked()
  {
    return cpSpaceIsLocked(m_space) == cpTrue;
  }

  PhysicsBody PhysicsWorld::static_body()
  {
    return { details::PhysicsExisting, cpSpaceGetStaticBody(m_space) };
  }

  void PhysicsWorld::add_default_collision_handler(PhysicsCollisionHandler* handler)
  {
    auto* raw = cpSpaceAddDefaultCollisionHandler(m_space);
    raw->beginFunc = collision_begin;
    raw->preSolveFunc = collision_pre_solve;
    raw->postSolveFunc = collision_post_solve;
    raw->separateFunc = collision_separate;
    raw->userData = handler;
  }

  void PhysicsWorld::add_collision_handler(PhysicsCollisionHandler* handler, uintptr_t a, uintptr_t b) // TODO: check type for a and b
  {
    auto* raw = cpSpaceAddCollisionHandler(m_space, a, b);
    raw->beginFunc = collision_begin;
    raw->preSolveFunc = collision_pre_solve;
    raw->postSolveFunc = collision_post_solve;
    raw->separateFunc = collision_separate;
    raw->userData = handler;
  }

  void PhysicsWorld::add_wildcard_handler(PhysicsCollisionHandler* handler, uintptr_t type)
  {
    auto* raw = cpSpaceAddWildcardHandler(m_space, type);
    raw->beginFunc = collision_begin;
    raw->preSolveFunc = collision_pre_solve;
    raw->postSolveFunc = collision_post_solve;
    raw->separateFunc = collision_separate;
    raw->userData = handler;
  }

  void PhysicsWorld::add_shape(PhysicsShape shape)
  {
    shape.m_shape.reference();
    cpSpaceAddShape(m_space, shape.m_shape);
  }

  void PhysicsWorld::add_body(PhysicsBody body)
  {
    body.m_body.reference();
    cpSpaceAddBody(m_space, body.m_body);
  }

  void PhysicsWorld::add_constraint(PhysicsConstraint constraint)
  {
    constraint.m_constraint.reference();
    cpSpaceAddConstraint(m_space, constraint.m_constraint);
  }

  void PhysicsWorld::remove_shape(PhysicsShape shape)
  {
    assert(cpSpaceContainsShape(m_space, shape.m_shape) == cpTrue);
    shape.m_shape.unreference();
    cpSpaceRemoveShape(m_space, shape.m_shape);
  }

  void PhysicsWorld::remove_body(PhysicsBody body)
  {
    assert(cpSpaceContainsBody(m_space, body.m_body) == cpTrue);
    body.m_body.unreference();
    cpSpaceRemoveBody(m_space, body.m_body);
  }

  void PhysicsWorld::remove_constraint(PhysicsConstraint constraint)
  {
    assert(cpSpaceContainsConstraint(m_space, constraint.m_constraint) == cpTrue);
    constraint.m_constraint.unreference();
    cpSpaceRemoveConstraint(m_space, constraint.m_constraint);
  }

  bool PhysicsWorld::contains_shape(PhysicsShape* shape)
  {
    return cpSpaceContainsShape(m_space, shape->m_shape) == cpTrue;
  }

  bool PhysicsWorld::contains_body(PhysicsBody* body)
  {
    return cpSpaceContainsBody(m_space, body->m_body) == cpTrue;
  }

  bool PhysicsWorld::contains_constraint(PhysicsConstraint* constraint)
  {
    return cpSpaceContainsConstraint(m_space, constraint->m_constraint) == cpTrue;
  }

  // TODO: post-step callback

  // TODO: queries

  void PhysicsWorld::each_body(std::function<void(PhysicsBody)> func)
  {
    cpSpaceEachBody(m_space, body_iterator, &func);
  }

  void PhysicsWorld::each_shape(std::function<void(PhysicsShape)> func)
  {
    cpSpaceEachShape(m_space, shape_iterator, &func);
  }

  void PhysicsWorld::each_constraint(std::function<void(PhysicsConstraint)> func)
  {
    cpSpaceEachConstraint(m_space, constraint_iterator, &func);
  }

  void PhysicsWorld::reindex_static()
  {
    cpSpaceReindexStatic(m_space);
  }

  void PhysicsWorld::reindex_shape(PhysicsShape* shape)
  {
    cpSpaceReindexShape(m_space, shape->m_shape);
  }

  void PhysicsWorld::reindex_shapes_for_body(PhysicsBody* body)
  {
    cpSpaceReindexShapesForBody(m_space, body->m_body);
  }

  void PhysicsWorld::use_spatial_hash(float dim, int count)
  {
    cpSpaceUseSpatialHash(m_space, dim, count);
  }

  void PhysicsWorld::debug_draw(PhysicsDebug* debug, Flags<PhysicsDebugFeature> features)
  {
    cpSpaceDebugDrawOptions options;
    options.drawCircle = draw_circle;
    options.drawSegment = draw_segment;
    options.drawFatSegment = draw_fat_segment;
    options.drawPolygon = draw_polygon;
    options.drawDot = draw_dot;
    options.flags = static_cast<cpSpaceDebugDrawFlags>(features.value());
    options.shapeOutlineColor = { 1.0f, 0.0f, 0.0f, 0.5f };
    options.colorForShape = draw_color_for_shape;
    options.constraintColor = { 0.0f, 1.0f, 0.0f, 0.5f };
    options.collisionPointColor = { 0.0f, 0.0f, 1.0f, 0.5f };
    options.data = debug;
    cpSpaceDebugDraw(m_space, &options);
  }

  void PhysicsWorld::update(Time time)
  {
    cpSpaceStep(m_space, time.as_seconds());
  }

  cpBool PhysicsWorld::collision_begin(cpArbiter* arbiter, cpSpace* space, cpDataPointer user_data)
  {
    auto* handler = static_cast<PhysicsCollisionHandler*>(user_data);
    return handler->begin(PhysicsArbiter(arbiter), PhysicsWorld(details::PhysicsExisting, space)) ? cpTrue : cpFalse;
  }

  cpBool PhysicsWorld::collision_pre_solve(cpArbiter* arbiter, cpSpace* space, cpDataPointer user_data)
  {
    auto* handler = static_cast<PhysicsCollisionHandler*>(user_data);
    return handler->pre_solve(PhysicsArbiter(arbiter), PhysicsWorld(details::PhysicsExisting, space)) ? cpTrue : cpFalse;
  }

  void PhysicsWorld::collision_post_solve(cpArbiter* arbiter, cpSpace* space, cpDataPointer user_data)
  {
    auto* handler = static_cast<PhysicsCollisionHandler*>(user_data);
    handler->post_solve(PhysicsArbiter(arbiter), PhysicsWorld(details::PhysicsExisting, space));
  }

  void PhysicsWorld::collision_separate(cpArbiter* arbiter, cpSpace* space, cpDataPointer user_data)
  {
    auto* handler = static_cast<PhysicsCollisionHandler*>(user_data);
    handler->separate(PhysicsArbiter(arbiter), PhysicsWorld(details::PhysicsExisting, space));
  }

  void PhysicsWorld::body_iterator(cpBody* body, void* data)
  {
    auto* func = static_cast<std::function<void(PhysicsBody)>*>(data);
    (*func)(PhysicsBody(details::PhysicsExisting, body));
  }

  void PhysicsWorld::shape_iterator(cpShape* shape, void* data)
  {
    auto* func = static_cast<std::function<void(PhysicsShape)>*>(data);
    (*func)(PhysicsShape(details::PhysicsExisting, shape));
  }

  void PhysicsWorld::constraint_iterator(cpConstraint* constraint, void* data)
  {
    auto* func = static_cast<std::function<void(PhysicsConstraint)>*>(data);
    (*func)(PhysicsConstraint(details::PhysicsExisting, constraint));
  }

  namespace {

    Color to_color(cpSpaceDebugColor color)
    {
      return { color.r, color.g, color.b, color.a };
    }

  }

  void PhysicsWorld::draw_circle(cpVect center, cpFloat angle, cpFloat radius, cpSpaceDebugColor outline_color, cpSpaceDebugColor fill_color, cpDataPointer data)
  {
    auto* debug = static_cast<PhysicsDebug*>(data);
    debug->draw_circle(gf::vec(center.x, center.y), static_cast<float>(angle), static_cast<float>(radius), to_color(outline_color), to_color(fill_color));
  }

  void PhysicsWorld::draw_segment(cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data)
  {
    auto* debug = static_cast<PhysicsDebug*>(data);
    debug->draw_segment(gf::vec(a.x, a.y), gf::vec(b.x, b.y), to_color(color));
  }

  void PhysicsWorld::draw_fat_segment(cpVect a, cpVect b, cpFloat radius, cpSpaceDebugColor outline_color, cpSpaceDebugColor fill_color, cpDataPointer data)
  {
    auto* debug = static_cast<PhysicsDebug*>(data);
    debug->draw_fat_segment(gf::vec(a.x, a.y), gf::vec(b.x, b.y), static_cast<float>(radius), to_color(outline_color), to_color(fill_color));
  }

  void PhysicsWorld::draw_polygon(int count, const cpVect* vertices, cpFloat radius, cpSpaceDebugColor outline_color, cpSpaceDebugColor fill_color, cpDataPointer data)
  {
    std::vector<gf::Vec2F> new_vertices;
    new_vertices.reserve(count);

    for (int i = 0; i < count; ++i) {
      new_vertices.emplace_back(static_cast<float>(vertices[i].x), static_cast<float>(vertices[i].y));
    }

    auto* debug = static_cast<PhysicsDebug*>(data);
    debug->draw_polygon(new_vertices, static_cast<float>(radius), to_color(outline_color), to_color(fill_color));
  }

  void PhysicsWorld::draw_dot(cpFloat size, cpVect location, cpSpaceDebugColor color, cpDataPointer data)
  {
    auto* debug = static_cast<PhysicsDebug*>(data);
    debug->draw_dot(static_cast<float>(size), gf::vec(location.x, location.y), to_color(color));
  }

  cpSpaceDebugColor PhysicsWorld::draw_color_for_shape(cpShape* shape, cpDataPointer data)
  {
    auto* debug = static_cast<PhysicsDebug*>(data);
    auto color = debug->color_for_shape(PhysicsShape(details::PhysicsExisting, shape));
    return { color.r, color.g, color.b, color.a };
  }

  PhysicsWorld::PhysicsWorld(details::PhysicsExistingType existing, cpSpace* space)
  : m_space(existing, space)
  {
  }

}
