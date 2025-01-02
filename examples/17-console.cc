// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/core/ConsoleChar.h>
#include <gf2/core/ConsoleStyle.h>

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
  const std::filesystem::path font_file = assets_directory / "terminal.png";

  gf::SingleSceneManager scene_manager("17-console | gf2", gf::vec(1600, 900));

  gf::ConsoleFontContext font_context;
  font_context.format = { gf::ConsoleFontFormat::Grayscale, gf::ConsoleFontFormat::InRow, gf::ConsoleFontFormat::ModifiedCodePage437 };
  font_context.render_manager = scene_manager.render_manager();

  const gf::ConsoleFont console_font(font_file, font_context);

  gf::ConsoleStyle style;
  style.color.background = gf::gray(0.25f);

  gf::ConsoleRichStyle rich_style(style);
  rich_style.set_style("red", { gf::Red, gf::Black });
  rich_style.set_style("gf", { gf::Orange, gf::Azure });
  rich_style.set_style("warn", { gf::Black, gf::Yellow });

  gf::Console console({ 80, 50 });

  console.put_character({ 40, 25 }, '@', style);
  console.put_character({ 42, 25 }, gf::ConsoleChar::WhiteSmilingFace, style);

  console.print({ 1, 1 }, gf::ConsoleAlignment::Left, rich_style, "String with a <style=red>red</> word.");

  console.print(console.size() - 2, gf::ConsoleAlignment::Right, rich_style, "Made with <style=gf>gf</>!");

  const std::string_view text = "This is a simple but long text with <style=warn>multiple</> lines.";

  console.print_area(rect({ 2, 5 }, { 16, 5 }), gf::ConsoleAlignment::Left, rich_style, text);
  console.print_area(rect({ 2, 15 }, { 16, 5 }), gf::ConsoleAlignment::Center, rich_style, text);
  console.print_area(rect({ 2, 25 }, { 16, 5 }), gf::ConsoleAlignment::Right, rich_style, text);

  console.clear(rect({ 30, 5 }, { 16, 5 }), style);
  console.draw_frame(rect({ 30, 5 }, { 16, 5 }), style);
  console.draw_frame(rect({ 30, 15 }, { 16, 5 }), style, "Frame title");

  gf::ConsoleEntity console_entity(&console_font);
  console_entity.graphics().update(console, scene_manager.render_manager());
  console_entity.set_location({ 1000.0f, 1000.0f });
  console_entity.set_origin({ 0.5f, 0.5f });

  gf::Scene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size(console.size() * console_font.character_size());

  scene.add_world_entity(&console_entity);

  return scene_manager.run(&scene);
}
