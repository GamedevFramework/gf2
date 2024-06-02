// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
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
  const std::filesystem::path default_font_file = assets_directory / "LinLibertine_R.otf";
  const std::filesystem::path bold_font_file = assets_directory / "LinLibertine_RB.otf";
  const std::filesystem::path italic_font_file = assets_directory / "LinLibertine_RI.otf";
  const std::filesystem::path bold_italic_font_file = assets_directory / "LinLibertine_RBI.otf";

  gf::SingleSceneManager scene_manager("16-richtext | gf2", gf::vec(1600, 900));

  gf::FontManager font_manager;
  gf::FontFace default_face(default_font_file, &font_manager);
  gf::FontFace bold_face(bold_font_file, &font_manager);
  gf::FontFace italic_face(italic_font_file, &font_manager);
  gf::FontFace bold_italic_face(bold_italic_font_file, &font_manager);

  gf::FontAtlas atlas({ 1024, 1024 }, scene_manager.render_manager());

  gf::TextData text_data;
  text_data.content = "Default <color=0080FF>colored</> <b>bold and <i>italic</></> and <i>only italic</>\nand <size=1.4>bigger</> or <size=0.6>smaller</>, it works!";
  text_data.character_size = 32;
  text_data.color = gf::Black;

  gf::TextData paragraph_data;
  paragraph_data.content = "<b>Line breaking</>, also known as <b>word wrapping</>, is breaking a section of text into lines so that it will fit into the available width of a page, window or other display area.\nIn text display, <b>line wrap</> is continuing on a new line when a line is full, so that each line fits into the viewable window, allowing text to be read from top to bottom without any horizontal <i>scrolling</>.\n<b>Word wrap</> is the additional feature of most text editors, word processors, and web browsers, of breaking lines between words rather than within words, where possible. Word wrap makes it unnecessary to hard-code newline delimiters within paragraphs, and allows the display of text to adapt flexibly and dynamically to displays of varying sizes.\n<size=0.8>Source: <color=0000FF>https://en.wikipedia.org/wiki/Line_wrap_and_word_wrap</></>";
  paragraph_data.character_size = 24;
  paragraph_data.color = gf::Black;
  paragraph_data.alignment = gf::Alignment::Justify;
  paragraph_data.paragraph_width = 750.0f;

  gf::RichTextEntity text_entity(&atlas, &default_face, &bold_face, &italic_face, &bold_italic_face, text_data, scene_manager.render_manager());
  text_entity.set_location({ 1000.0f, 800.0f });
  text_entity.set_origin({ 0.5f, 0.5f });

  gf::RichTextEntity paragraph_entity(&atlas, &default_face, &bold_face, &italic_face, &bold_italic_face, paragraph_data, scene_manager.render_manager());
  paragraph_entity.set_location({ 1000.0f, 875.0f });
  paragraph_entity.set_origin({ 0.5f, 0.0f });

  gf::Scene scene;
  scene.set_world_center({ 1000.0f, 1000.0f });
  scene.set_world_size({ 500.0f, 500.0f });
  scene.set_clear_color(gf::White);

  scene.add_world_entity(&text_entity);
  scene.add_world_entity(&paragraph_entity);

  return scene_manager.run(&scene);
}
