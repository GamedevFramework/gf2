// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cstdlib>

#include <gf2/core/FontManager.h>

#include <gf2/graphics/FontAtlas.h>
#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/TextEntity.h>

#include "config.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path font_file = assets_directory / "Oxanium-Regular.ttf";

  gf::SingleSceneManager scene_manager("11-text | gf2", gf::vec(1600, 900));

  gf::FontManager font_manager;
  gf::Font font(font_file, &font_manager);
  gf::FontAtlas font_atlas(&font, gf::vec(2048, 2048), scene_manager.renderer());

  gf::TextData text_data;
  text_data.content = "Gamedev Framework 2";
  text_data.character_size = 64;
  text_data.color = gf::darker(gf::Azure, 0.2f);
  // text_data.outline_color = gf::Gray;
  // text_data.outline_thickness = 2.0f;

  gf::TextEntity text_entity(&font_atlas, text_data, scene_manager.renderer());
  text_entity.transform().location = gf::vec(1000.0f, 1000.0f);
  text_entity.transform().origin = gf::vec(0.5f, 0.5f);

  gf::StandardScene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size({ 500.0f, 500.0f });
  scene.set_clear_color(gf::White);

  scene.add_world_entity(&text_entity);

  scene_manager.run(&scene);

  return EXIT_SUCCESS;
}
