// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ShapeBuffer.h>

#include <gf2/core/Log.h>

#include <gf2/graphics/Entity.h>
#include <gf2/graphics/RenderManager.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/ShapeEntity.h>

namespace {

  class HUD : public gf::HudEntity {
  public:
    HUD(gf::RenderManager* render_manager)
    : m_render_manager(render_manager)
    , m_entity({}, render_manager)
    {
      do_resize(render_manager->surface_size());
    }

    void resize(gf::Vec2I surface_size) override
    {
      do_resize(surface_size);
    }

    void render(gf::RenderRecorder& recorder) override
    {
      m_entity.render(recorder);
    }

  private:
    void do_resize(gf::Vec2I surface_size)
    {
      const gf::RectF surface = gf::RectF::from_size(surface_size);

      auto shape_at = [surface](gf::Orientation orientation) {
        constexpr gf::Vec2F Size = { 50.0f, 50.0f };
        const gf::Vec2F position = surface.position_at(orientation);
        const gf::RectF rectangle = gf::RectF::from_center_size(position, Size);
        gf::ShapeBuffer shape = gf::ShapeBuffer::make_rectangle(rectangle);
        shape.color = gf::Red;
        return shape;
      };

      gf::ShapeGroupBuffer buffer;

      buffer.shapes.push_back(shape_at(gf::Orientation::NorthWest));
      buffer.shapes.push_back(shape_at(gf::Orientation::North));
      buffer.shapes.push_back(shape_at(gf::Orientation::NorthEast));
      buffer.shapes.push_back(shape_at(gf::Orientation::East));
      buffer.shapes.push_back(shape_at(gf::Orientation::SouthEast));
      buffer.shapes.push_back(shape_at(gf::Orientation::South));
      buffer.shapes.push_back(shape_at(gf::Orientation::SouthWest));
      buffer.shapes.push_back(shape_at(gf::Orientation::West));
      buffer.shapes.push_back(shape_at(gf::Orientation::Center));

      m_entity.shape_group().update(buffer, m_render_manager);
      m_entity.set_origin({ 0.5f, 0.5f });
      m_entity.set_location(surface_size / 2.0f);
    }

    gf::RenderManager* m_render_manager;
    gf::ShapeGroupEntity m_entity;
  };

}


int main()
{
  gf::SingleSceneManager scene_manager("events | gf2", gf::vec(1600, 900));
  gf::Scene scene;
  scene.set_world_center({ 0.0f, 0.0f });
  scene.set_world_size({ 1000.0f, 1000.0f });

  HUD hud(scene_manager.render_manager());
  scene.add_hud_entity(&hud);

  return scene_manager.run(&scene);
}
