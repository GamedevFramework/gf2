// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cstdlib>

#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/SpriteEntity.h>

#include "config.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path texture_file = assets_directory / "root_company_game_over.jpg";

  constexpr gf::Vec2I WindowSize = gf::vec(1600, 900);

  const gf::GraphicsInitializer graphics;

  gf::SingleSceneManager scene_manager("10-sprite | gf2", WindowSize);

  const gf::Texture texture(texture_file, scene_manager.renderer());
  gf::SpriteEntity sprite_entity(&texture, scene_manager.renderer());
  sprite_entity.transform().location = gf::vec(1000.0f, 1000.0f);
  sprite_entity.transform().origin = gf::vec(0.5f, 0.5f);
  sprite_entity.transform().scale = gf::vec(0.1f, 0.1f);

  gf::StandardScene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size({ 200.0f, 200.0f });

  scene.add_world_entity(&sprite_entity);

  scene_manager.run(&scene);

  return EXIT_SUCCESS;
}
