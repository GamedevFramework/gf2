// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cstdlib>

#include <gf2/core/Activities.h>
#include <gf2/core/Easing.h>
#include <gf2/core/Log.h>
#include <gf2/core/Transform.h>

#include <gf2/graphics/Entity.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/Shape.h>

namespace {
  namespace ga = gf::activity;

  auto create_activity(gf::Vec2F* location, float* rotation, gf::Color* color, float* faces)
  {
    return ga::repeat(
        ga::sequence(
            ga::parallel_any(
                ga::sequence(
                    ga::motion({ 100.0f, 100.0f }, { 540.0f, 100.0f }, location, gf::seconds(2.7f), gf::ease_in_out_quad),
                    ga::motion({ 540.0f, 100.0f }, { 540.0f, 380.0f }, location, gf::seconds(1.9f), gf::ease_out_back),
                    ga::motion({ 540.0f, 380.0f }, { 100.0f, 380.0f }, location, gf::seconds(2.7f), gf::ease_out_elastic),
                    ga::motion({ 100.0f, 380.0f }, { 100.0f, 100.0f }, location, gf::seconds(1.9f))),
                ga::sequence(
                    ga::value(4, 8, faces, gf::seconds(4.6f), gf::ease_in_out_circ),
                    ga::value(8, 4, faces, gf::seconds(4.6f), gf::ease_in_out_circ)),
                ga::sequence(
                    ga::color(gf::Azure, gf::Orange, color, gf::seconds(4.6f)),
                    ga::color(gf::Orange, gf::Azure, color, gf::seconds(4.6f))),
                ga::value(0, 10 * gf::Pi, rotation, gf::seconds(9.2f), gf::ease_in_out_bounce)),
            ga::delay(gf::seconds(0.5f))));
  }

  class BlobEntity : public gf::Entity {
  public:
    BlobEntity(gf::RenderManager* render_manager)
    : m_render_manager(render_manager)
    , m_activity(create_activity(&m_location, &m_rotation, &m_color, &m_faces))
    {
    }

    void update(gf::Time time) override
    {
      m_activity.run(time);

      gf::ShapeBuffer shape = gf::ShapeBuffer::make_circle(30.0f, static_cast<uint32_t>(m_faces));
      shape.color = m_color;

      gf::ShapeGroupBuffer shape_group;
      shape_group.shapes.push_back(std::move(shape));

      m_shape.update(shape_group, m_render_manager);
    }

    void render(gf::RenderRecorder& recorder) override
    {
      gf::Transform transform;
      transform.location = m_location;
      transform.rotation = m_rotation;
      transform.origin = { 0.5f, 0.5f };

      gf::RenderObject object;
      object.geometry = m_shape.geometry();
      object.transform = transform.compute_matrix(m_shape.bounds());

      recorder.record(object);
    }

  private:
    gf::RenderManager* m_render_manager = nullptr;
    gf::Vec2F m_location = { 0.0f, 0.0f };
    float m_rotation = 0.0f;
    gf::Color m_color = gf::White;
    float m_faces = 0;
    ga::AnyActivity m_activity;
    gf::ShapeGroup m_shape;
  };

}

int main()
{
  gf::SingleSceneManager scene_manager("18-activities | gf2", gf::vec(1600, 900));

  gf::Scene scene;
  scene.set_clear_color(gf::White);
  scene.set_world_center({ 320.0f, 240.0f });
  scene.set_world_size({ 640.0f, 480.0f });

  BlobEntity blob_entity(scene_manager.render_manager());
  scene.add_world_entity(&blob_entity);

  return scene_manager.run(&scene);
}
