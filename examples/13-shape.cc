// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cstdlib>

#include <gf2/core/Color.h>

#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>

#include "gf2/graphics/ShapeEntity.h"

int main()
{

  gf::SingleSceneManager scene_manager("13-shape | gf2", gf::vec(1600, 900));

  gf::ShapeData rectangle_data = gf::ShapeData::make_rectangle({ 160.0f, 90.0f });
  rectangle_data.color = gf::Azure;
  rectangle_data.outline_thickness = 2.0f;
  rectangle_data.outline_color = gf::darker(gf::Azure);

  gf::ShapeEntity rectangle_entity(nullptr, rectangle_data, scene_manager.renderer());
  rectangle_entity.transform().location = gf::vec(1000.0f, 1000.0f);
  rectangle_entity.transform().origin = gf::vec(0.5f, 0.5f);

  gf::StandardScene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size({ 200.0f, 200.0f });

  scene.add_world_entity(&rectangle_entity);

  scene_manager.run(&scene);

  return EXIT_SUCCESS;
}
