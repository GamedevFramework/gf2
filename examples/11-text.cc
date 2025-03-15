// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/core/Color.h>
#include <gf2/core/FontFace.h>
#include <gf2/core/FontManager.h>

#include <gf2/graphics/CurveEntity.h>
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
  gf::FontFace face(font_file, &font_manager);
  gf::FontAtlas atlas({ 1024, 1024 }, scene_manager.render_manager());

  gf::TextData text_data;
  text_data.content = "Gamedev Framework 2";
  text_data.character_size = 64;
  text_data.color = gf::darker(gf::Azure, 0.2f);
  // text_data.outline_color = gf::Gray;
  // text_data.outline_thickness = 0.2f;

  gf::TextEntity text_entity(&atlas, &face, text_data, scene_manager.render_manager());
  text_entity.set_location({ 1000.0f, 1000.0f });
  text_entity.set_origin({ 0.5f, 0.5f });

  const float width = text_entity.text().bounds().extent.w;

  gf::CurveBuffer curve_buffer = gf::CurveBuffer::make_line({ 0.0f, 0.0f }, { 1.02f * width, 0.0f });
  curve_buffer.color = gf::Orange;
  curve_buffer.thickness = 4.0f;

  gf::CurveEntity curve_entity(curve_buffer, scene_manager.render_manager());
  curve_entity.set_location({ 1000.0f, 1026.0f });
  curve_entity.set_origin({ 0.5f, 0.5f });

  gf::Scene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size({ 500.0f, 500.0f });
  scene.set_clear_color(gf::White);

  scene.add_world_entity(&text_entity);
  scene.add_world_entity(&curve_entity);

  return scene_manager.run(&scene);
}
