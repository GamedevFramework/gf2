#include "HeroEntity.h"

#include "GameHub.h"

#include "gf2/core/Orientation.h"
#include "gf2/core/Vec2.h"

namespace home {

  namespace {
    constexpr float HeroVelocity = 200.0f;

    gf::Orientation harvest_orientation(float angle)
    {
      assert(-gf::Pi <= angle && angle <= gf::Pi);

      if (angle < -2 * gf::Pi4) {
        return gf::Orientation::NorthWest;
      }

      if (angle < 0 * gf::Pi4) {
        return gf::Orientation::NorthEast;
      }

      if (angle < 2 * gf::Pi4) {
        return gf::Orientation::SouthEast;
      }

      return gf::Orientation::SouthWest;
    }
  }

  using namespace gf::literals;

  HeroEntity::HeroEntity(GameHub* hub, const WorldData& data)
  : m_hero_animations(data.hero_animations, hub->render_manager(), hub->resource_manager())
  , m_crosshair(data.crosshair, hub->render_manager(), hub->resource_manager())
  , m_jet_engine_sound(hub->resource_manager()->get<gf::Sound>(data.jet_engine_sound.filename))
  {
    set_location({ 0.0f, 0.0f });
    set_origin({ 0.5f, 0.5f });
    set_scale(0.75f);
    m_hero_animations.select("pause_south_east"_id);

    m_crosshair.set_scale({ 0.5f, 0.25f });
    m_crosshair.set_origin({ 0.5f, 0.5f });

    m_jet_engine_sound->set_looping(true);
  }

  void HeroEntity::set_target(gf::Vec2F target)
  {
    m_target = target;
    m_crosshair.set_location(m_target);
  }

  void HeroEntity::update(gf::Time time)
  {
    if (m_status == Status::Dying) {
      m_hero_animations.select("death"_id);
      m_target = location();
      m_velocity = { 0.0f, 0.0f };

      // TODO: sound
    } else {
      auto move = m_target - location();
      float length = gf::euclidean_length(move);

      if (length > time.as_seconds() * HeroVelocity) {
        if (m_activity == Activity::Mining) {
          m_orientation = harvest_orientation(gf::angle(move));
        } else {
          assert(m_activity == Activity::Walking);
          m_orientation = gf::orientation(gf::angle(move));
        }

        m_velocity = (move / length) * HeroVelocity;
        m_status = Status::Moving;
      } else {
        set_location(m_target);
        m_velocity = { 0.0f, 0.0f };
        m_status = Status::Waiting;
      }

      set_location(location() + m_velocity * time.as_seconds()); // TODO: remove when physics ready

      if (m_status == Status::Moving) {
        if (!m_jet_engine_sound->playing()) {
          m_jet_engine_sound->start();
        }
      } else {
        if (m_jet_engine_sound->playing()) {
          m_jet_engine_sound->stop();
        }
      }

      // set the animation

      if (m_activity == Activity::Mining && m_status == Status::Waiting) {
        switch (m_orientation) {
          case gf::Orientation::NorthWest:
            m_hero_animations.select("harvest_north_west"_id);
            break;
          case gf::Orientation::NorthEast:
            m_hero_animations.select("harvest_north_east"_id);
            break;
          case gf::Orientation::SouthWest:
            m_hero_animations.select("harvest_south_west"_id);
            break;
          case gf::Orientation::SouthEast:
            m_hero_animations.select("harvest_south_east"_id);
            break;
          default:
            break;
        }
      } else {
        switch (m_orientation) {
          case gf::Orientation::NorthWest:
            m_hero_animations.select(m_status == Status::Moving ? "move_north_west"_id : "pause_north_west"_id);
            break;
          case gf::Orientation::North:
            m_hero_animations.select(m_status == Status::Moving ? "move_north"_id : "pause_north"_id);
            break;
          case gf::Orientation::NorthEast:
            m_hero_animations.select(m_status == Status::Moving ? "move_north_east"_id : "pause_north_east"_id);
            break;
          case gf::Orientation::East:
            m_hero_animations.select(m_status == Status::Moving ? "move_east"_id : "pause_east"_id);
            break;
          case gf::Orientation::SouthEast:
            m_hero_animations.select(m_status == Status::Moving ? "move_south_east"_id : "pause_south_east"_id);
            break;
          case gf::Orientation::South:
            m_hero_animations.select("south"_id);
            break;
          case gf::Orientation::SouthWest:
            m_hero_animations.select(m_status == Status::Moving ? "move_south_west"_id : "pause_south_west"_id);
            break;
          case gf::Orientation::West:
            m_hero_animations.select(m_status == Status::Moving ? "move_west"_id : "pause_west"_id);
            break;
          default:
            assert(false);
            break;
        }
      }
    }

    m_hero_animations.update(time);

    update_position.emit(location());

    m_activity = Activity::Walking;
  }

  void HeroEntity::render(gf::RenderRecorder& recorder)
  {
    m_hero_animations.set_transform(transform());
    m_hero_animations.render(recorder);
    m_crosshair.render(recorder);
  }

}
