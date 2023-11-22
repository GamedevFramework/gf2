#ifndef HOME_HERO_ENTITY_H
#define HOME_HERO_ENTITY_H

#include <gf2/core/Orientation.h>
#include <gf2/core/Signal.h>
#include <gf2/core/Vec2.h>

#include <gf2/audio/Sound.h>
#include <gf2/graphics/AnimationEntity.h>
#include <gf2/graphics/TransformableEntity.h>
#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsShape.h>

#include "WorldData.h"

#include "gf2/graphics/SpriteEntity.h"

namespace home {
  class GameHub;

  class HeroEntity : public gf::TransformableEntity {
  public:
    HeroEntity(GameHub* hub, const WorldData& data);

    void update(gf::Time time) override;
    void render(gf::RenderRecorder& recorder) override;

    void set_target(gf::Vec2F target);

    gf::Signal<void(gf::Vec2F)> update_location; // NOLINT

  private:
    void compute_animation();
    void compute_mining_animation();
    void compute_walking_animation();

    gf::Orientation m_orientation = gf::Orientation::SouthWest;

    enum class Status : uint8_t {
      Waiting,
      Moving,
      Dying,
    };

    Status m_status = Status::Waiting;

    enum class Activity : uint8_t {
      Walking,
      Mining,
    };

    Activity m_activity = Activity::Walking;

    gf::PhysicsBody m_body;
    gf::PhysicsShape m_shape;

    gf::Vec2F m_target = { 0.0f, 0.0f };
    gf::Vec2F m_velocity = { 0.0f, 0.0f };

    gf::AnimationGroupEntity m_hero_animations;
    gf::SpriteEntity m_crosshair;

    gf::Sound* m_jet_engine_sound = nullptr;
  };

}

#endif // HOME_HERO_ENTITY_H
