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
  constexpr int ImageWidth = 512;
  constexpr int ImageHeight = 40;
  constexpr int ImageRowLength = ImageWidth / 8;

  constexpr uint32_t ImageBitmap[ImageHeight][ImageRowLength] = {
    { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x00000000, },
    { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x00000000, },
    { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x00000000, },
    { 0x01FFFFE0, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x7FFFFE00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x007FFF80, },
    { 0x03FFFFF0, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x7FFFFE00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x01FFFFC0, },
    { 0x07FFFFF0, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x7FFFFE00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x01FFFFE0, },
    { 0x0FFFFFF0, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x7FFFFE00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x01FFFFF0, },
    { 0x0F800000, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x7C000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x000001F0, },
    { 0x0F000000, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x7C000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x000000F0, },
    { 0x0F000000, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x7C000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x000000F0, },
    { 0x0F000000, 0x00000000, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x00000000, 0x7C000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C000000, 0x000000F0, },
    { 0x0F000000, 0x03FFE003, 0xFFFFFFFC, 0x0007FFC0, 0x00FFFF80, 0x0FFF803C, 0x00070000, 0x7C000001, 0xFF00FFF0, 0x00FFFFFF, 0xFE0003FF, 0xE00F001F, 0x000E00FF, 0xF8001FF0, 0x3C007C00, 0x000000F0, },
    { 0x0F000000, 0x07FFF803, 0xFFFFFFFF, 0x001FFFE0, 0x01FFFF80, 0x1FFFE03C, 0x000F8000, 0x7C000007, 0xFF81FFFC, 0x01FFFFFF, 0xFF8007FF, 0xF80F001F, 0x801E03FF, 0xFE007FF8, 0x3C00FC00, 0x000000F0, },
    { 0x0F000000, 0x07FFF803, 0xFFFFFFFF, 0x803FFFF0, 0x03FFFF80, 0x3FFFF03C, 0x000F0000, 0x7C00000F, 0xFF81FFFE, 0x01FFFFFF, 0xFFC00FFF, 0xFC0F003F, 0x801E07FF, 0xFF007FF8, 0x3C00F800, 0x000000F0, },
    { 0x0F000000, 0x07FFFC03, 0xFFFFFFFF, 0x807FFFF8, 0x07FFFF80, 0x7FFFF03E, 0x000F0000, 0x7C00000F, 0xFF81FFFF, 0x01FFFFFF, 0xFFE01FFF, 0xFE0F003F, 0x801E07FF, 0xFF80FFF8, 0x3C01F000, 0x000000F0, },
    { 0x0F000000, 0x00003C03, 0xC003C007, 0xC07C0078, 0x07800780, 0x7800F81E, 0x001E0000, 0x7C00000F, 0x0000001F, 0x01F001F0, 0x03E01E00, 0x3E0F803F, 0x801E0F80, 0x0F80F000, 0x3C03E000, 0x000001F0, },
    { 0x0F000000, 0x00003E03, 0xC003C003, 0xC078007C, 0x0F800780, 0xF800781E, 0x001E0000, 0x7C00000F, 0x0000000F, 0x01F001F0, 0x01E03E00, 0x1E07803B, 0xC03E0F00, 0x0780F000, 0x3C07C000, 0x000001F0, },
    { 0x0F007FF8, 0x00001E03, 0xC003C003, 0xC078003C, 0x0F800780, 0xF000781F, 0x001E0000, 0x7C00000F, 0x0000000F, 0x01F001F0, 0x01E03E00, 0x1E07807B, 0xC03C0F00, 0x0780F000, 0x3C0F8000, 0x00001FE0, },
    { 0x0F00FFF8, 0x00001E03, 0xC003C003, 0xC078003C, 0x0F800780, 0xF000780F, 0x003C0000, 0x7FFFF80F, 0x0000000F, 0x01F001F0, 0x01E03C00, 0x1E07807B, 0xC03C0F00, 0x0780F000, 0x3C1F0000, 0x0003FFE0, },
    { 0x0F00FFF8, 0x00001E03, 0xC003C003, 0xC078003C, 0x0F800780, 0xF000780F, 0x003C0000, 0x7FFFFC0F, 0x0000000F, 0x01F001F0, 0x01E03C00, 0x1E07C079, 0xC03C0F00, 0x0780F000, 0x3C3E0000, 0x003FFFC0, },
    { 0x0F007FF8, 0x00001E03, 0xC003C003, 0xC078003C, 0x0F800780, 0xF000780F, 0x803C0000, 0x7FFFFC0F, 0x0000000F, 0x01F001F0, 0x01E03C00, 0x1E03C0F1, 0xE07C0F00, 0x0780F000, 0x3C7C0000, 0x00FFFF00, },
    { 0x0F000078, 0x00003E03, 0xC003C003, 0xC078007C, 0x0F800780, 0xF8007807, 0x80780000, 0x7FFFFC0F, 0x0000000F, 0x01F001F0, 0x01E03E00, 0x1E03C0F1, 0xE0780F00, 0x0780F000, 0x3CFC0000, 0x00FFF000, },
    { 0x0F000078, 0x03FFFE03, 0xC003C003, 0xC07FFFFC, 0x0F800780, 0xFFFFF807, 0x80780000, 0x7C00000F, 0x0001FFFF, 0x01F001F0, 0x01E03FFF, 0xFE03C0F1, 0xE0780F00, 0x0780F000, 0x3DFC0000, 0x01FE0000, },
    { 0x0F000078, 0x07FFFE03, 0xC003C003, 0xC07FFFFC, 0x0F800780, 0xFFFFF807, 0xC0780000, 0x7C00000F, 0x0003FFFF, 0x01F001F0, 0x01E03FFF, 0xFE03C0E0, 0xF0780F00, 0x0780F000, 0x3FFC0000, 0x01F00000, },
    { 0x0F000078, 0x0FFFFE03, 0xC003C003, 0xC07FFFFC, 0x0F800780, 0xFFFFF803, 0xC0F00000, 0x7C00000F, 0x0007FFFF, 0x01F001F0, 0x01E03FFF, 0xFE01E1E0, 0xF0780F00, 0x0780F000, 0x3FFE0000, 0x03E00000, },
    { 0x0F000078, 0x1F803E03, 0xC003C003, 0xC07FFFF8, 0x0F800780, 0xFFFFF803, 0xC0F00000, 0x7C00000F, 0x0007E00F, 0x01F001F0, 0x01E03FFF, 0xFE01E1E0, 0xF0F00F00, 0x0780F000, 0x3FDF0000, 0x03E00000, },
    { 0x0F000078, 0x1E001E03, 0xC003C003, 0xC0780000, 0x0F800780, 0xF0000003, 0xE0F00000, 0x7C00000F, 0x000F800F, 0x01F001F0, 0x01E03C00, 0x0001E1E0, 0x70F00F00, 0x0780F000, 0x3F9F0000, 0x03E00000, },
    { 0x0F000078, 0x1E001E03, 0xC003C003, 0xC0780000, 0x0F800780, 0xF0000001, 0xE1E00000, 0x7C00000F, 0x000F000F, 0x01F001F0, 0x01E03C00, 0x0001E1C0, 0x78F00F00, 0x0780F000, 0x3F0F8000, 0x03E00000, },
    { 0x0F000078, 0x1E001E03, 0xC003C003, 0xC0780000, 0x0F800780, 0xF0000001, 0xE1E00000, 0x7C00000F, 0x000F000F, 0x01F001F0, 0x01E03C00, 0x0001F3C0, 0x78F00F00, 0x0780F000, 0x3E07C000, 0x03E00000, },
    { 0x0F000078, 0x1E001E03, 0xC003C003, 0xC0780000, 0x0F800780, 0xF0000001, 0xF1E00000, 0x7C00000F, 0x000F000F, 0x01F001F0, 0x01E03C00, 0x0000F3C0, 0x79E00F00, 0x0780F000, 0x3C03C000, 0x03E00000, },
    { 0x0F000078, 0x1E001E03, 0xC003C003, 0xC0780000, 0x0F800780, 0xF8000000, 0xF3C00000, 0x7C00000F, 0x000F000F, 0x01F001F0, 0x01E03E00, 0x0000F3C0, 0x39E00F00, 0x0780F000, 0x3C03E000, 0x03E00000, },
    { 0x0F800078, 0x1E001E03, 0xC003C003, 0xC0780000, 0x0F800780, 0xF8000000, 0xF3C00000, 0x7C00000F, 0x000F800F, 0x01F001F0, 0x01E03E00, 0x0000F780, 0x3DE00F00, 0x0780F000, 0x3C01F000, 0x03E00000, },
    { 0x0FC000F8, 0x1F001E03, 0xC003C003, 0xC07C0000, 0x07C00780, 0x7C000000, 0xFBC00000, 0x7C00000F, 0x0007C00F, 0x01F001F0, 0x01E01F00, 0x0000FF80, 0x3DE00F80, 0x0F80F000, 0x3C00F800, 0x03E00000, },
    { 0x07FFFFF8, 0x0FFFFE03, 0xC003C003, 0xC03FFFF8, 0x07FFFF80, 0x7FFFF000, 0x7F800000, 0x7C00000F, 0x0007FFFF, 0x01F001F0, 0x01E01FFF, 0xFC007F80, 0x3FE007FF, 0xFF00F000, 0x3C00F800, 0x03FFFFF0, },
    { 0x07FFFFF8, 0x0FFFFE03, 0xC003C003, 0xC03FFFF8, 0x03FFFF80, 0x3FFFF000, 0x7F800000, 0x7C00000F, 0x0003FFFF, 0x01F001F0, 0x01E00FFF, 0xFC007F00, 0x1FC007FF, 0xFF00F000, 0x3C007C00, 0x03FFFFF0, },
    { 0x01FFFFF8, 0x07FFFE03, 0xC003C003, 0xC01FFFF0, 0x01FFFF80, 0x1FFFF000, 0x7F800000, 0x7C00000F, 0x0001FFFF, 0x01F001F0, 0x01E007FF, 0xFC007F00, 0x1FC001FF, 0xFE00F000, 0x3C003E00, 0x03FFFFF0, },
    { 0x007FFFF8, 0x01FFFE03, 0xC003C003, 0xC003FFE0, 0x007FFF80, 0x07FFE000, 0x3F000000, 0x7800000F, 0x00007FFF, 0x00F000E0, 0x01E001FF, 0xF8007F00, 0x1FC0007F, 0xF800F000, 0x3C003C00, 0x03FFFFF0, },
    { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, },
    { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, },
    { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, },
  };


  int pixel(int x, int y)
  {
    const uint32_t shift = x % 32;
    return (ImageBitmap[y][x / 32] >> (UINT32_C(31) - shift)) & UINT32_C(1); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
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
        static constexpr uint32_t PointCount = 8;
        gf::ShapeBuffer ball = gf::ShapeBuffer::make_circle(gf::CircF::from_center_radius(body.location(), 1.5f), PointCount);
        assert(ball.points.size() == PointCount);
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

      const gf::Vec2F center = gf::vec(ImageWidth, ImageHeight) / 2.0f;

      for (int y = 0; y < ImageHeight; ++y) {
        for (int x = 0; x < ImageWidth; ++x) {
          if (pixel(x, y) == 0) {
            continue;
          }

          const gf::Vec2F position = gf::vec(x, y);

          const float x_jitter = random.compute_uniform_float(-0.025f, 0.025f);
          const float y_jitter = random.compute_uniform_float(-0.025f, 0.025f);
          const gf::Vec2F jitter = gf::vec(x_jitter, y_jitter);

          gf::PhysicsBodyData body_data;
          body_data.type = gf::PhysicsBodyType::Dynamic;
          body_data.location = 2.0f * (position - center + jitter);

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
      body_data.location = { -1000.0f, 0.0f };
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
