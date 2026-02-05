// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/core/ConsoleChar.h>
#include <gf2/core/ConsoleStyle.h>
#include <gf2/core/ConsoleOperations.h>

#include <gf2/graphics/ConsoleEntity.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>

#include "config.h"

namespace {

  gf::RectI rect(gf::Vec2I position, gf::Vec2I size)
  {
    return gf::RectI::from_position_size(position, size);
  }

}

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path picture_font_file = assets_directory / "Hack_square_64x64.png";
  const std::filesystem::path text_font_file = assets_directory / "DejaVuSansMono.png";

  gf::SingleSceneManager scene_manager("17-console | gf2", gf::vec(1600, 900));

  gf::ConsoleFontContext font_context;
  font_context.format = gf::DwarfFortressFormat;
  font_context.render_manager = scene_manager.render_manager();

  gf::ConsoleFont console_picture_font(picture_font_file, font_context);
  gf::ConsoleFont console_text_font(text_font_file, font_context);

  gf::ConsoleStyle style;
  style.color.background = gf::gray(0.25f);

  gf::ConsoleRichStyle rich_style(style);
  rich_style.set_style("red", { gf::Red, gf::Black });
  rich_style.set_style("gf", { gf::Orange, gf::Azure });
  rich_style.set_style("warn", { gf::Black, gf::Yellow });

  gf::Console console({ 80, 50 });

  gf::console_write_picture(console, { 70, 25 }, '@', style);
  gf::console_write_picture(console, { 72, 25 }, gf::ConsoleChar::WhiteSmilingFace, style);

  gf::console_print_picture(console, { 1, 1 }, gf::ConsoleAlignment::Left, rich_style, "String with a <style=red>red</> word.");

  gf::console_print_picture(console, { 78, 48 }, gf::ConsoleAlignment::Right, rich_style, "Made with <style=gf>gf2</>!");
  gf::console_print_text(console, { 78, 47 }, gf::ConsoleAlignment::Right, rich_style, "Made with <style=gf>gf2</>!");

  const std::string_view text = "This is a simple but long text with <style=warn>multiple</> lines.";

  gf::console_print_picture(console, rect({ 2, 5 }, { 16, 5 }), gf::ConsoleAlignment::Left, rich_style, "{}", text);
  gf::console_print_picture(console, rect({ 2, 13 }, { 16, 5 }), gf::ConsoleAlignment::Center, rich_style, "{}", text);
  gf::console_print_picture(console, rect({ 2, 21 }, { 16, 5 }), gf::ConsoleAlignment::Right, rich_style, "{}", text);
  gf::console_print_text(console, rect({ 2, 29 }, { 8, 5 }), gf::ConsoleAlignment::Left, rich_style, "{}", text);
  gf::console_print_text(console, rect({ 2, 37 }, { 8, 5 }), gf::ConsoleAlignment::Center, rich_style, "{}", text);
  gf::console_print_text(console, rect({ 2, 45 }, { 8, 5 }), gf::ConsoleAlignment::Right, rich_style, "{}", text);


  gf::console_clear(console, rect({ 30, 5 }, { 16, 5 }), style);
  gf::console_draw_frame(console, rect({ 30, 5 }, { 16, 5 }), style);
  gf::console_draw_frame(console, rect({ 30, 15 }, { 16, 5 }), style, gf::ConsoleMode::Picture, "Frame title");
  gf::console_draw_frame(console, rect({ 30, 25 }, { 16, 5 }), style, gf::ConsoleMode::Text, "Frame title");

  gf::ConsoleEntity console_entity(&console_picture_font, &console_text_font);
  console_entity.graphics().update(console, scene_manager.render_manager());
  console_entity.set_location({ 0.0f, 0.0f });
  console_entity.set_origin({ 0.5f, 0.5f });

  gf::Scene scene;
  scene.set_world_center({ 0.0f, 0.0f });
  scene.set_world_size(console.size() * console_picture_font.character_size());

  scene.add_world_entity(&console_entity);

  return scene_manager.run(&scene);
}
