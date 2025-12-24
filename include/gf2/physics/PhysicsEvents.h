// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_EVENTS_H
#define GF_PHYSICS_EVENTS_H

#include <vector>

#include "PhysicsApi.h"
#include "PhysicsBody.h"
#include "PhysicsContact.h"
#include "PhysicsJoint.h"
#include "PhysicsShape.h"
#include "PhysicsTransform.h"

namespace gf {

  struct GF_PHYSICS_API PhysicsSensorBeginTouchEvent {
    PhysicsShape sensor;
    PhysicsShape visitor;
  };

  struct GF_PHYSICS_API PhysicsSensorEndTouchEvent {
    PhysicsShape sensor;
    PhysicsShape visitor;
  };

  struct GF_PHYSICS_API PhysicsSensorEvents {
    std::vector<PhysicsSensorBeginTouchEvent> begin_events;
    std::vector<PhysicsSensorEndTouchEvent> end_events;
  };

  struct GF_PHYSICS_API PhysicsContactBeginTouchEvent {
    PhysicsShape shape0;
    PhysicsShape shape1;
    PhysicsContact contact;
  };

  struct GF_PHYSICS_API PhysicsContactEndTouchEvent {
    PhysicsShape shape0;
    PhysicsShape shape1;
    PhysicsContact contact;
  };

  struct GF_PHYSICS_API PhysicsContactHitEvent {
    PhysicsShape shape0;
    PhysicsShape shape1;
    PhysicsContact contact;
    Vec2F point;
    Vec2F normal;
    float approach_speed;
  };

  struct GF_PHYSICS_API PhysicsContactEvents {
    std::vector<PhysicsContactBeginTouchEvent> begin_events;
    std::vector<PhysicsContactEndTouchEvent> end_events;
    std::vector<PhysicsContactHitEvent> hit_events;
  };

  struct GF_PHYSICS_API PhysicsBodyMoveEvent {
    PhysicsTransform transform;
    PhysicsBody body;
    bool fell_asleep;
    void* user_data;
  };

  struct GF_PHYSICS_API PhysicsBodyEvents {
    std::vector<PhysicsBodyMoveEvent> move_events;
  };

  struct GF_PHYSICS_API PhysicsJointEvent {
    PhysicsJoint joint;
    void* user_data;
  };

  struct GF_PHYSICS_API PhysicsJointEvents {
    std::vector<PhysicsJointEvent> joint_events;
  };

}

#endif // GF_PHYSICS_EVENTS_H
