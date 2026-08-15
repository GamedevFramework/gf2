// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/core/FontFace.h>
#include <gf2/core/FontManager.h>
#include <gf2/core/Log.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/Widgets.h>
#include <gf2/graphics/WidgetContainer.h>

#include "config.h"

namespace {

  constexpr gf::Vec2I SceneSize = { 1600, 9000 };

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

  class WidgetScene : public gf::Scene {
  public:
    WidgetScene(gf::FontFace* face, gf::RenderManager* render_manager)
    : m_atlas({ 1024, 1024 }, render_manager)
    , m_start_widget(&m_atlas, face, compute_text_widget_data("Start"), render_manager)
    , m_options_widget(&m_atlas, face, compute_text_widget_data("Options"), render_manager)
    , m_disabled_widget(&m_atlas, face, compute_text_widget_data("Disabled"), render_manager)
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
    gf::WidgetContainer m_widgets;
  };

}

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path font_file = assets_directory / "LinLibertine_R.otf";

  gf::FontManager font_manager;
  gf::FontFace face(font_file, &font_manager);

  gf::SingleSceneManager scene_manager("widgets | gf2", SceneSize);

  WidgetScene scene(&face, scene_manager.render_manager());
  return scene_manager.run(&scene);
}
