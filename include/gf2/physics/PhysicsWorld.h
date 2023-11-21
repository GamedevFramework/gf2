// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_PHYSICS_WORLD_H
#define GF_PHYSICS_WORLD_H

#include <functional>

#include <chipmunk/chipmunk.h>

#include <gf2/core/Time.h>
#include <gf2/core/Vec2.h>

#include "PhysicsApi.h"
#include "PhysicsDebug.h"
#include "PhysicsHandle.h"

namespace gf {
  class PhysicsBody;
  class PhysicsCollisionHandler;
  class PhysicsConstraint;
  class PhysicsShape;

  class GF_PHYSICS_API PhysicsWorld {
  public:
    PhysicsWorld();
    PhysicsWorld(const PhysicsWorld&) = default;
    PhysicsWorld(PhysicsWorld&&) noexcept = default;
    ~PhysicsWorld();

    PhysicsWorld& operator=(const PhysicsWorld&) = default;
    PhysicsWorld& operator=(PhysicsWorld&&) noexcept = default;

    int iterations() const;
    void set_iterations(int iterations);

    Vec2F gravity() const;
    void set_gravity(Vec2F gravity);

    float damping() const;
    void set_damping(float damping);

    float idle_speed_threshold() const;
    void set_idle_speed_threshold(float threshold);

    float sleep_time_threshold() const;
    void set_sleep_time_threshold(float threshold);

    float collision_slop() const;
    void set_collision_slop(float slop);

    float collision_bias() const;
    void set_collision_bias(float bias);

    unsigned collision_persistence() const;
    void set_collision_persistence(unsigned persistence);

    float current_time_step() const;

    bool locked();

    void add_default_collision_handler(PhysicsCollisionHandler* handler);
    void add_collision_handler(PhysicsCollisionHandler* handler, uintptr_t a, uintptr_t b);
    void add_wildcard_handler(PhysicsCollisionHandler* handler, uintptr_t type);

    void add_shape(PhysicsShape shape);
    void add_body(PhysicsBody body);
    void add_constraint(PhysicsConstraint constraint);

    void remove_shape(PhysicsShape shape);
    void remove_body(PhysicsBody body);
    void remove_constraint(PhysicsConstraint constraint);

    bool contains_shape(PhysicsShape shape);
    bool contains_body(PhysicsBody body);
    bool contains_constraint(PhysicsConstraint constraint);

    // TODO: post-step callback

    // TODO: queries

    void each_body(std::function<void(PhysicsBody)> func);
    void each_shape(std::function<void(PhysicsShape)> func);
    void each_constraint(std::function<void(PhysicsConstraint)> func);

    void reindex_static();
    void reindex_shape(PhysicsShape shape);
    void reindex_shapes_for_body(PhysicsBody& body);

    void use_spatial_hash(float dim, int count);

    void debug_draw(PhysicsDebug* debug, Flags<PhysicsDebugFeature> features = All);

    void update(Time time);

  private:
    friend class PhysicsArbiter;
    friend class PhysicsBody;
    friend class PhysicsConstraint;
    friend class PhysicsShape;

    static cpBool collision_begin(cpArbiter* arbiter, cpSpace* space, cpDataPointer user_data);
    static cpBool collision_pre_solve(cpArbiter* arbiter, cpSpace* space, cpDataPointer user_data);
    static void collision_post_solve(cpArbiter* arbiter, cpSpace* space, cpDataPointer user_data);
    static void collision_separate(cpArbiter* arbiter, cpSpace* space, cpDataPointer user_data);

    static void body_iterator(cpBody* body, void* data);
    static void shape_iterator(cpShape* shape, void* data);
    static void constraint_iterator(cpConstraint* constraint, void* data);

    static void draw_circle(cpVect center, cpFloat angle, cpFloat radius, cpSpaceDebugColor outline_color, cpSpaceDebugColor fill_color, cpDataPointer data);
    static void draw_segment(cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data);
    static void draw_fat_segment(cpVect a, cpVect b, cpFloat radius, cpSpaceDebugColor outline_color, cpSpaceDebugColor fill_color, cpDataPointer data);
    static void draw_polygon(int count, const cpVect* vertices, cpFloat radius, cpSpaceDebugColor outline_color, cpSpaceDebugColor fill_color, cpDataPointer data);
    static void draw_dot(cpFloat size, cpVect location, cpSpaceDebugColor color, cpDataPointer data);
    static cpSpaceDebugColor draw_color_for_shape(cpShape* shape, cpDataPointer data);

    PhysicsWorld(details::PhysicsExistingType existing, cpSpace* space);

    details::PhysicsHandle<cpSpace, cpSpaceGetUserData, cpSpaceSetUserData, cpSpaceFree> m_space;
  };

}

#endif // GF_PHYSICS_WORLD_H
