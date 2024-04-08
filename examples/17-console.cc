// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cstdlib>

#include <gf2/core/ConsoleChar.h>

#include <gf2/graphics/ConsoleEntity.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>

#include "config.h"

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path font_file = assets_directory / "terminal.png";

  gf::SingleSceneManager scene_manager("17-console | gf2", gf::vec(1600, 900));

  gf::ConsoleFontContext font_context;
  font_context.format = { gf::ConsoleFontFormat::Grayscale, gf::ConsoleFontFormat::InRow, gf::ConsoleFontFormat::ModifiedCodePage437 };
  font_context.render_manager = scene_manager.render_manager();

  const gf::ConsoleFont console_font(font_file, font_context);

  gf::ConsoleStyle style;
  style.background = gf::gray(0.25f);

  gf::ConsoleData console_data({ 80, 50 });

  console_data.put_character({ 40, 25 }, '@', style);
  console_data.put_character({ 42, 25 }, gf::ConsoleChar::WhiteSmilingFace, style);

  console_data.print({ 1, 1 }, style, "String with a red word.");

  style.alignment = gf::ConsoleAlignment::Right;
  console_data.print(console_data.screen.size() - 2, style, "Made with gf!");

  const std::string_view text = "This is a simple but long text with multiple lines.";

  style.alignment = gf::ConsoleAlignment::Left;
  console_data.print_area(gf::RectI::from_position_size({ 2, 5 }, { 16, 5 }), style, text);
  style.alignment = gf::ConsoleAlignment::Center;
  console_data.print_area(gf::RectI::from_position_size({ 2, 15 }, { 16, 5 }), style, text);
  style.alignment = gf::ConsoleAlignment::Right;
  console_data.print_area(gf::RectI::from_position_size({ 2, 25 }, { 16, 5 }), style, text);

  console_data.clear(gf::RectI::from_position_size({ 30, 5 }, { 16, 5 }), style);
  console_data.draw_frame(gf::RectI::from_position_size({ 30, 5 }, { 16, 5 }), style, "");
  console_data.draw_frame(gf::RectI::from_position_size({ 30, 15 }, { 16, 5 }), style, "Frame title");

  gf::ConsoleEntity console_entity(&console_font, console_data, scene_manager.render_manager());
  console_entity.set_location({ 1000.0f, 1000.0f });
  console_entity.set_origin({ 0.5f, 0.5f });

  gf::StandardScene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size(console_data.screen.size() * console_font.character_size());

  scene.add_world_entity(&console_entity);

  return scene_manager.run(&scene);
}
