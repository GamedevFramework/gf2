#include "HeroEntity.h"

#include <gf2/core/Log.h>

#include <gf2/graphics/RichMap.h>

#include "GameHub.h"

namespace home {

  namespace {

    constexpr float HeroMass = 1.0f;
    constexpr float HeroRadius = 20.0f;

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

    gf::Vec2F compute_initial_location(const WorldResources& data, gf::ResourceManager* resource_manager)
    {
      const gf::RichMap* map = resource_manager->get<gf::RichMap>(data.map.filename);

      for (const gf::MapObjectLayer& object_layer : map->tiled_map()->object_layers) {
        if (object_layer.layer.name == "Locations") {
          for (const gf::MapObject& object : object_layer.objects) {
            if (object.name == "start") {
              assert(object.type == gf::MapObjectType::Point);
              return object.location;
            }
          }
        }
      }

      assert(false);
      return { 0.0f, 0.0f };
    }

    gf::PhysicsShapeData compute_hero_shape_data()
    {
      gf::PhysicsShapeData data;
      data.density = HeroMass;
      return data;
    }

    gf::PhysicsBodyData compute_hero_body_data(gf::Vec2F initial_position)
    {
      gf::PhysicsBodyData data;
      data.location = initial_position;
      data.type = gf::PhysicsBodyType::Dynamic;
      data.motion_locks.angular_z = true;
      return data;
    }
  }

  using namespace gf::literals;

  HeroEntity::HeroEntity(GameHub* hub, const WorldResources& resources, gf::PhysicsWorld* physics_world)
  : m_body(physics_world, compute_hero_body_data(compute_initial_location(resources, hub->resource_manager())))
  , m_shape(gf::PhysicsShape::create_circle(&m_body, compute_hero_shape_data(), gf::CircF::from_radius(HeroRadius)))
  , m_target(m_body.location())
  , m_hero_animations(resources.hero_animations, hub->render_manager(), hub->resource_manager())
  , m_crosshair(resources.crosshair, hub->render_manager(), hub->resource_manager())
  , m_jet_engine_sound(hub->resource_manager()->get<gf::Sound>(resources.jet_engine_sound.filename))
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

  void HeroEntity::set_mining()
  {
    m_activity = Activity::Mining;
  }

  void HeroEntity::update(gf::Time time)
  {
    set_location(m_body.location());

    if (m_status == Status::Dying) {
      m_target = location();
      m_velocity = { 0.0f, 0.0f };

      // TODO: sound
    } else {
      auto move = m_target - location();
      const float length = gf::euclidean_length(move);

      if (length > HeroRadius) {
        if (m_activity == Activity::Mining) {
          m_orientation = harvest_orientation(gf::angle(move));
        } else {
          assert(m_activity == Activity::Walking);
          m_orientation = gf::orientation(gf::angle(move));
        }

        m_velocity = (move / length) * HeroVelocity;
        m_status = Status::Moving;
      } else {
        m_velocity = { 0.0f, 0.0f };
        m_status = Status::Waiting;
      }

      if (m_status == Status::Moving) {
        if (!m_jet_engine_sound->playing()) {
          m_jet_engine_sound->start();
        }
      } else {
        if (m_jet_engine_sound->playing()) {
          m_jet_engine_sound->stop();
        }
      }
    }

    compute_animation();
    m_hero_animations.update(time);

    update_location.emit(location());

    m_body.set_linear_velocity(m_velocity);

    m_activity = Activity::Walking;
  }

  void HeroEntity::render(gf::RenderRecorder& recorder)
  {
    m_crosshair.render(recorder);
    m_hero_animations.set_transform(transform());
    m_hero_animations.render(recorder);
  }

  void HeroEntity::compute_animation()
  {
    if (m_status == Status::Dying) {
      m_hero_animations.select("death"_id);
    } else {
      if (m_activity == Activity::Mining && m_status == Status::Waiting) {
        compute_mining_animation();
      } else {
        compute_walking_animation();
      }
    }
  }

  void HeroEntity::compute_mining_animation()
  {
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
  }

  void HeroEntity::compute_walking_animation()
  {
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
