// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cstdlib>

#include <gf2/core/AnimationData.h>

#include <gf2/graphics/AnimationEntity.h>
#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>

#include "config.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path texture_file0 = assets_directory / "splash_screen_0.png";
  const std::filesystem::path texture_file1 = assets_directory / "splash_screen_1.png";
  const std::filesystem::path texture_file2 = assets_directory / "splash_screen_2.png";
  const std::filesystem::path texture_file3 = assets_directory / "splash_screen_3.png";
  const std::filesystem::path texture_file4 = assets_directory / "splash_screen_4.png";
  const std::filesystem::path texture_file5 = assets_directory / "splash_screen_5.png";
  const std::filesystem::path texture_file6 = assets_directory / "splash_screen_6.png";
  const std::filesystem::path texture_file7 = assets_directory / "splash_screen_7.png";

  gf::SingleSceneManager scene_manager("12-animation | gf2", gf::vec(1600, 900));

  const gf::Texture texture0(texture_file0, scene_manager.render_manager());
  const gf::Texture texture1(texture_file1, scene_manager.render_manager());
  const gf::Texture texture2(texture_file2, scene_manager.render_manager());
  const gf::Texture texture3(texture_file3, scene_manager.render_manager());
  const gf::Texture texture4(texture_file4, scene_manager.render_manager());
  const gf::Texture texture5(texture_file5, scene_manager.render_manager());
  const gf::Texture texture6(texture_file6, scene_manager.render_manager());
  const gf::Texture texture7(texture_file7, scene_manager.render_manager());

  const std::vector<const gf::Texture*> textures = { &texture0, &texture1, &texture2, &texture3, &texture4, &texture5, &texture6, &texture7 };

  gf::AnimationData data = {};
  data.properties = gf::AnimationProperties::Loop;

  for (uint32_t i = 0; i < 7; ++i) {
    data.add_tileset(i, { 6, 4 }, gf::seconds(1.0f / 25.0f), 24);
  }

  data.add_tileset(7, { 6, 4 }, gf::seconds(1.0f / 25.0f), 4);

  gf::AnimationEntity animation_entity(textures, data, scene_manager.render_manager());
  animation_entity.set_location({ 1000.0f, 1000.0f });
  animation_entity.set_origin({ 0.5f, 0.5f });
  animation_entity.set_scale(0.1f);

  gf::StandardScene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size({ 50.0f, 100.0f });

  scene.add_world_entity(&animation_entity);

  return scene_manager.run(&scene);
}
