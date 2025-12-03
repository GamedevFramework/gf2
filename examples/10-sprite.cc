// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/SpriteEntity.h>

#include "config.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path texture_file = assets_directory / "root_company_game_over.jpg";

  gf::SingleSceneManager scene_manager("10-sprite | gf2", gf::vec(1600, 900));

  const gf::GpuTexture texture(texture_file, scene_manager.render_manager());
  gf::SpriteEntity sprite_entity(&texture, scene_manager.render_manager());
  sprite_entity.set_location({ 1000.0f, 1000.0f });
  sprite_entity.set_origin({ 0.5f, 0.5f });
  sprite_entity.set_scale(0.1f);

  gf::Scene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size({ 200.0f, 200.0f });

  scene.add_world_entity(&sprite_entity);

  return scene_manager.run(&scene);
}
