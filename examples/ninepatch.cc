// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/graphics/NinePatchEntity.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>

#include "config.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path texture_file = assets_directory / "panel_brown_corners_a.png";

  gf::SingleSceneManager scene_manager("nine patch | gf2", gf::vec(1600, 900));

  const gf::GpuTexture texture(texture_file, scene_manager.render_manager());

  gf::NinePatchData data;
  data.size = texture.size();

  gf::NinePatchEntity nine_patch_entity0(&texture, data, scene_manager.render_manager());
  nine_patch_entity0.set_location({ 100.0f, 100.0f });

  data.size = { 1000.0f, 100.0f };
  gf::NinePatchEntity nine_patch_entity1(&texture, data, scene_manager.render_manager());
  nine_patch_entity1.set_location({ 300.0f, 100.0f });

  data.size = { 300.0f, 500.0f };
  gf::NinePatchEntity nine_patch_entity2(&texture, data, scene_manager.render_manager());
  nine_patch_entity2.set_location({ 300.0f, 300.0f });

  gf::Scene scene;
  scene.set_world_center({ 800.0f, 450.0f });
  scene.set_world_size({ 1600.0f, 900.0f });

  scene.add_world_entity(&nine_patch_entity0);
  scene.add_world_entity(&nine_patch_entity1);
  scene.add_world_entity(&nine_patch_entity2);

  return scene_manager.run(&scene);
}
