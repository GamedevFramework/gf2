// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/core/Color.h>

#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/ShapeEntity.h>

int main()
{
  gf::SingleSceneManager scene_manager("13-shape | gf2", gf::vec(1600, 900));

  gf::ShapeBuffer rectangle_buffer = gf::ShapeBuffer::make_rectangle({ 160.0f, 90.0f });
  rectangle_buffer.color = gf::Azure;
  rectangle_buffer.outline_thickness = 2.0f;
  rectangle_buffer.outline_color = gf::darker(gf::Azure);

  gf::ShapeEntity rectangle_entity(nullptr, rectangle_buffer, scene_manager.render_manager());
  rectangle_entity.set_location({ 1000.0f, 1000.0f });
  rectangle_entity.set_origin({ 0.5f, 0.5f });

  gf::Scene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size({ 200.0f, 200.0f });
  scene.set_clear_color(gf::White);

  scene.add_world_entity(&rectangle_entity);

  return scene_manager.run(&scene);
}
