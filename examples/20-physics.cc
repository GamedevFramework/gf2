// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Random.h>

#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/Shape.h>
#include <gf2/physics/PhysicsBody.h>
#include <gf2/physics/PhysicsShape.h>
#include <gf2/physics/PhysicsTaskManager.h>
#include <gf2/physics/PhysicsWorld.h>

namespace {
  constexpr int ImageWidth = 188;
  constexpr int ImageHeight = 35;
  constexpr int ImageRowLength = 24;

  constexpr char ImageBitmap[] = {
    15, -16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, -64, 15, 63, -32, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, -64, 15, 127, -125, -1, -128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127, -64, 15, 127, 15, -1, -64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, -64, 15, -2, 31, -1, -64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, -64, 0, -4, 63, -1, -32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, -64, 15, -8, 127, -1, -32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, -64, 0, -8, -15, -1, -32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -31, -1, -64, 15, -8, -32, -1, -32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, -15, -1, -64, 9, -15, -32, -1, -32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, -15, -1, -64, 0, -15, -32, -1, -32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, -7, -1, -64, 9, -29, -32, 127, -61, -16, 63, 15, -61, -1, -8, 31, -16, 15, -8, 126, 7, -31, -8, 31, -65, -7, -1, -64, 9, -29, -32, 0, 7, -8, 127, -97, -25, -1, -2, 63, -8, 31, -4, -1, 15, -13, -4, 63, -1, -3, -1, -64, 9, -29, -32, 0, 7, -8, 127, -97, -25, -1, -2, 63, -8, 31, -4, -1, 15, -13, -2, 63, -1, -3, -1, -64, 9, -29, -32, 0, 7, -8, 127, -97, -25, -1, -1, 63, -4, 63, -4, -1, 15, -13, -2, 63, -33, -1, -1, -32, 9, -25, -32, 0, 7, -8, 127, -97, -25, -1, -1, 63, -4, 63, -4, -1, 15, -13, -1, 63, -33, -1, -1, -16, 9, -25, -32, 0, 7, -8, 127, -97, -25, -1, -1, 63, -4, 63, -4, -1, 15, -13, -1, 63, -49, -1, -1, -8, 9, -57, -32, 0, 7, -8, 127, -97, -25, -8, -1, 63, -2, 127, -4, -1, 15, -13, -1, -65, -49, -1, -1, -4, 9, -57, -32, 0, 7, -8, 127, -97, -25, -8, -1, 63, -2, 127, -4, -1, 15, -13, -1, -65, -57, -1, -1, -2, 9, -57, -32, 0, 7, -8, 127, -97, -25, -8, -1, 63, -2, 127, -4, -1, 15, -13, -1, -1, -57, -1, -1, -1, 9, -57, -32, 0, 7, -1, -1, -97, -25, -8, -1, 63, -1, -1, -4, -1, 15, -13, -1, -1, -61, -1, -1, -1, -119, -57, -32, 0, 7, -1, -1, -97, -25, -8, -1, 63, -1, -1, -4, -1, 15, -13, -1, -1, -61, -1, -1, -1, -55, -49, -32, 0, 7, -1, -1, -97, -25, -8, -1, 63, -1, -1, -4, -1, 15, -13, -1, -1, -63, -1, -1, -1, -23, -49, -32, 127, -57, -1, -1, -97, -25, -1, -1, 63, -1, -1, -4, -1, 15, -13, -1, -1, -63, -1, -1, -1, -16, -49, -32, -1, -25, -1, -1, -97, -25, -1, -1, 63, -33, -5, -4, -1, 15, -13, -1, -1, -64, -1, -9, -1, -7, -49, -32, -1, -25, -8, 127, -97, -25, -1, -1, 63, -33, -5, -4, -1, 15, -13, -1, -1, -64, -1, -13, -1, -32, -49, -32, -1, -25, -8, 127, -97, -25, -1, -2, 63, -49, -13, -4, -1, 15, -13, -1, -1, -64, 127, -7, -1, -119, -17, -15, -1, -25, -8, 127, -97, -25, -1, -2, 63, -49, -13, -4, -1, 15, -13, -3, -1, -64, 127, -8, -2, 15, -17, -1, -1, -25, -8, 127, -97, -25, -1, -8, 63, -49, -13, -4, -1, 15, -13, -3, -1, -64, 63, -4, 120, 0, -17, -1, -1, -25, -8, 127, -97, -25, -8, 0, 63, -57, -29, -4, -1, 15, -13, -4, -1, -64, 63, -4, 0, 15, -17, -1, -1, -25, -8, 127, -97, -25, -8, 0, 63, -57, -29, -4, -1, -1, -13, -4, -1, -64, 31, -2, 0, 0, 103, -1, -1, -57, -8, 127, -97, -25, -8, 0, 63, -57, -29, -4, -1, -1, -13, -4, 127, -64, 31, -2, 0, 15, 103, -1, -1, -57, -8, 127, -97, -25, -8, 0, 63, -61, -61, -4, 127, -1, -29, -4, 127, -64, 15, -8, 0, 0, 55, -1, -1, -121, -8, 127, -97, -25, -8, 0, 63, -61, -61, -4, 127, -1, -29, -4, 63, -64, 15, -32, 0, 0, 23, -1, -2, 3, -16, 63, 15, -61, -16, 0, 31, -127, -127, -8, 31, -1, -127, -8, 31, -128, 7, -128, 0, 0
  };

  int pixel(int x, int y)
  {
    return (ImageBitmap[(x >> 3) + (y * ImageRowLength)] >> (~x & 0x07)) & 1; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
  }

  class PhysicsEntity : public gf::Entity {
  public:
    PhysicsEntity(gf::RenderManager* render_manager, gf::Random& random)
    : m_render_manager(render_manager)
    , m_task_manager(4)
    , m_shape_group({}, render_manager)
    {
      setup_world(random);
    }

    void update(gf::Time time) override
    {
      static constexpr gf::Time StepTime = gf::seconds(1.0f / 60.0f);

      m_time += time;

      while (m_time > StepTime) {
        m_world.update(StepTime);
        m_time -= StepTime;
      }

      gf::ShapeGroupBuffer buffer;

      for (const gf::PhysicsBody& body : m_bodies) {
        gf::ShapeBuffer ball = gf::ShapeBuffer::make_circle(gf::CircF::from_center_radius(body.location(), 1.5f), 8);
        ball.color = gf::Black;
        buffer.shapes.push_back(std::move(ball));
      }

      m_shape_group.update(buffer, m_render_manager);
    }

    void render(gf::RenderRecorder& recorder) override
    {
      gf::RenderObject object = {};
      object.geometry = m_shape_group.geometry();
      recorder.record(object);
    }

    void reset_world(gf::Random& random)
    {
      m_shapes.clear();
      m_bodies.clear();
      setup_world(random);
    }

  private:
    void setup_world(gf::Random& random)
    {
      gf::PhysicsWorldData world_data;
      world_data.gravity = { 0.0f, 0.0f };
      // world_data.enable_continuous = true;

      m_world = { world_data }; //, &m_task_manager };

      const gf::Vec2I center = gf::vec(ImageWidth, ImageHeight) / 2;

      for (int y = 0; y < ImageHeight; ++y) {
        for (int x = 0; x < ImageWidth; ++x) {
          if (pixel(x, y) == 0) {
            continue;
          }

          const gf::Vec2I position = gf::vec(x, y);

          const float x_jitter = random.compute_uniform_float(0.0f, 0.05f);
          const float y_jitter = random.compute_uniform_float(0.0f, 0.05f);
          const gf::Vec2F jitter = gf::vec(x_jitter, y_jitter);

          gf::PhysicsBodyData body_data;
          body_data.type = gf::PhysicsBodyType::Dynamic;
          body_data.location = 2 * (position - center + jitter);

          gf::PhysicsBody body(&m_world, body_data);

          gf::PhysicsShapeData shape_data;
          shape_data.density = 1.0f;

          gf::PhysicsShape shape = gf::PhysicsShape::create_circle(&body, shape_data, gf::CircF::from_radius(0.95f));

          m_bodies.push_back(std::move(body));
          m_shapes.push_back(std::move(shape));
        }
      }

      gf::PhysicsBodyData body_data;
      body_data.type = gf::PhysicsBodyType::Dynamic;
      body_data.location = { -1000.0f, -10.0f };
      body_data.linear_velocity = { 400.0f, 0.0f };

      gf::PhysicsBody body(&m_world, body_data);

      gf::PhysicsShapeData shape_data;
      shape_data.density = 1000.0f;

      gf::PhysicsShape shape = gf::PhysicsShape::create_circle(&body, shape_data, gf::CircF::from_radius(10.0f));

      m_bodies.push_back(std::move(body));
      m_shapes.push_back(std::move(shape));
    }

    gf::RenderManager* m_render_manager = nullptr;
    gf::Time m_time;
    gf::PhysicsTaskManager m_task_manager;
    gf::PhysicsWorld m_world;
    std::vector<gf::PhysicsBody> m_bodies;
    std::vector<gf::PhysicsShape> m_shapes;
    gf::ShapeGroup m_shape_group;
  };

  class PhysicsScene : public gf::Scene {
  public:
    PhysicsScene(gf::BasicSceneManager* scene_manager)
    : m_entity(scene_manager->render_manager(), m_random)
    {
      set_world_size({ 1024.0f, 768.0f });
      set_world_center({ 0.0f, 0.0f });
      set_clear_color(gf::White);
      add_world_entity(&m_entity);
    }

  private:

    void do_process_event(const gf::Event& event) override
    {
      if (event.type() == gf::EventType::KeyPressed) {
        if (event.from<gf::EventType::KeyPressed>().scancode == gf::Scancode::Space) {
          m_entity.reset_world(m_random);
        }
      }
    }

    gf::Random m_random;
    PhysicsEntity m_entity;
  };

}

int main()
{
  gf::SingleSceneManager scene_manager("20-physics | gf2", gf::vec(1600, 900));
  PhysicsScene scene(&scene_manager);
  return scene_manager.run(&scene);
}
