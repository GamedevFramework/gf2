// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/core/FontFace.h>
#include <gf2/core/FontManager.h>
#include <gf2/core/Log.h>
#include <gf2/core/SpriteSheet.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/Widgets.h>
#include <gf2/graphics/WidgetContainer.h>

#include "config.h"

namespace {

  constexpr gf::Vec2I SceneSize = { 1600, 9000 };

  struct WidgetResources {
    WidgetResources(const std::filesystem::path& assets_directory, gf::FontManager* font_manager, gf::RenderManager* render_manager)
    : face(assets_directory / "LinLibertine_R.otf", font_manager)
    , sprite_sheet(assets_directory / "ui.xml")
    , texture(assets_directory / sprite_sheet.texture_path(), render_manager)
    {
    }

    gf::FontFace face;
    gf::SpriteSheet sprite_sheet;
    gf::GpuTexture texture;
  };

  gf::TextWidgetData compute_text_widget_data(std::string_view content)
  {
    gf::TextData text = {};
    text.content = content;
    text.character_size = 32.0f;
    text.outline_thickness = 0.5f;

    gf::TextWidgetData data = {};

    data.disabled_text = text;
    data.disabled_text.color = gf::gray(0.8f);
    data.disabled_text.outline_color = gf::Gray;

    data.default_text = text;
    data.default_text.color = gf::Black;
    data.default_text.outline_color = gf::White;

    data.selected_text = text;
    data.selected_text.color = gf::Gray;
    data.selected_text.outline_color = gf::White;

    return data;
  }

  gf::TextButtonWidgetData compute_text_button_widget_data(std::string_view content)
  {
    gf::TextButtonWidgetData data = {};

    static_cast<gf::TextWidgetData&>(data) = compute_text_widget_data(content);

    data.disabled_button.color = gf::gray(0.95f);
    data.disabled_button.outline_thickness = 1.0f;
    data.disabled_button.outline_color = gf::Gray;

    data.default_button.color = gf::White;
    data.default_button.outline_thickness = 1.0f;
    data.default_button.outline_color = gf::Black;

    data.selected_button.color = gf::White;
    data.selected_button.outline_thickness = 1.0f;
    data.selected_button.outline_color = gf::Gray;

    return data;
  }

  gf::SpriteWidgetData compute_sprite_widget_data(const gf::SpriteSheet& sprite_sheet)
  {
    gf::SpriteWidgetData data = {};

    data.disabled_sprite.texture_region = sprite_sheet.texture_region("grey_button05.png");
    data.default_sprite.texture_region = sprite_sheet.texture_region("grey_button04.png");
    data.selected_sprite.texture_region = sprite_sheet.texture_region("grey_button02.png");

    return data;
  }

  class WidgetScene : public gf::Scene {
  public:
    WidgetScene(WidgetResources& resources, gf::RenderManager* render_manager)
    : m_atlas({ 1024, 1024 }, render_manager)
    , m_start_widget(&m_atlas, &resources.face, compute_text_widget_data("Start"), render_manager)
    , m_options_widget(&m_atlas, &resources.face, compute_text_widget_data("Options"), render_manager)
    , m_disabled_widget(&m_atlas, &resources.face, compute_text_widget_data("Disabled"), render_manager)
    , m_quit_widget(&m_atlas, &resources.face, compute_text_button_widget_data("Quit"), render_manager)
    , m_transformed_widget(&m_atlas, &resources.face, compute_text_button_widget_data("Transformed"), render_manager)
    , m_disabled_again_widget(&m_atlas, &resources.face, compute_text_button_widget_data("Disabled again"), render_manager)
    , m_sprite_widget(&resources.texture, &resources.texture, &resources.texture, compute_sprite_widget_data(resources.sprite_sheet), render_manager)
    {
      set_clear_color(gf::White);
      set_world_size({ 640, 480 });
      set_world_center({ 320, 240 });

      m_start_widget.set_location({ 50.0f, 50.0f });
      m_start_widget.set_callback([]() { gf::Log::info("Start!"); });
      add_world_entity(&m_start_widget);
      m_widgets.add_widget(&m_start_widget);

      m_options_widget.set_location({ 50.0f, 100.0f });
      m_options_widget.set_callback([]() { gf::Log::info("Options!"); });
      add_world_entity(&m_options_widget);
      m_widgets.add_widget(&m_options_widget);

      m_disabled_widget.set_location({ 50.0f, 150.0f });
      m_disabled_widget.set_callback([]() { gf::Log::info("Disabled?"); });
      m_disabled_widget.set_disabled();
      add_world_entity(&m_disabled_widget);
      m_widgets.add_widget(&m_disabled_widget);

      m_quit_widget.set_location({ 50.0f, 200.0f });
      m_quit_widget.set_callback([]() { gf::Log::info("Quit!"); });
      add_world_entity(&m_quit_widget);
      m_widgets.add_widget(&m_quit_widget);

      m_transformed_widget.set_location({ 50.0f, 250.0f });
      m_transformed_widget.set_rotation(gf::Pi4);
      m_transformed_widget.set_scale({ 1.0f, 0.8f });
      m_transformed_widget.set_callback([]() { gf::Log::info("Transformed!"); });
      add_world_entity(&m_transformed_widget);
      m_widgets.add_widget(&m_transformed_widget);

      m_disabled_again_widget.set_location({ 50.0f, 400.0f });
      m_disabled_again_widget.set_callback([]() { gf::Log::info("Disabled again?"); });
      m_disabled_again_widget.set_disabled();
      add_world_entity(&m_disabled_again_widget);
      m_widgets.add_widget(&m_disabled_again_widget);

      m_sprite_widget.set_location({ 300.0f, 50.0f });
      m_sprite_widget.set_callback([]() { gf::Log::info("Sprite!"); });
      add_world_entity(&m_sprite_widget);
      m_widgets.add_widget(&m_sprite_widget);
    }

  private:
    void do_process_event(const gf::Event& event) override
    {
      switch (event.type()) {
        case gf::EventType::MouseMoved:
        {
          const gf::Vec2F position = event.from<gf::EventType::MouseMoved>().position;
          const gf::Vec2F location = position_to_world_location(position);
          m_widgets.point_to(location);
          break;
        }
        case gf::EventType::MouseButtonPressed:
          m_widgets.trigger_selected();
          break;
        case gf::EventType::KeyPressed:
          switch (event.from<gf::EventType::KeyPressed>().scancode) {
            case gf::Scancode::Down:
              m_widgets.select_next_widget();
              break;
            case gf::Scancode::Up:
              m_widgets.select_prev_widget();
              break;
            case gf::Scancode::Return:
              m_widgets.trigger_selected();
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
    }

    gf::FontAtlas m_atlas;
    gf::TextWidget m_start_widget;
    gf::TextWidget m_options_widget;
    gf::TextWidget m_disabled_widget;
    gf::TextButtonWidget m_quit_widget;
    gf::TextButtonWidget m_transformed_widget;
    gf::TextButtonWidget m_disabled_again_widget;
    gf::SpriteWidget m_sprite_widget;

    gf::WidgetContainer m_widgets;
  };

}

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;

  gf::FontManager font_manager;
  gf::SingleSceneManager scene_manager("widgets | gf2", SceneSize);

  WidgetResources resources(assets_directory, &font_manager, scene_manager.render_manager());
  WidgetScene scene(resources, scene_manager.render_manager());
  return scene_manager.run(&scene);
}
