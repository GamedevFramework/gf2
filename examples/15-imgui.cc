// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cstdlib>

#include <imgui.h>

#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/imgui/ImguiEntity.h>
#include <gf2/imgui/ImguiManager.h>

#include "config.h"

namespace {

  class ImguiScene : public gf::StandardScene {
  public:
    ImguiScene(gf::BasicSceneManager* scene_manager)
    : m_manager(scene_manager->window(), scene_manager->renderer())
    {
      add_hud_entity(&m_entity);
    }

  private:

    bool do_early_process_event(const gf::Event& event) override
    {
      return m_manager.process_event(event);
    }

    void do_update(gf::Time time) override
    {
      m_manager.update(time);

      ImGui::NewFrame();

      update_entities(time);

      ImGui::ShowDemoWindow(&m_show_demo);

      ImGui::EndFrame();
      ImGui::Render();

      m_entity.set_draw_data(ImGui::GetDrawData());
    }

    gf::ImguiManager m_manager;
    gf::ImguiEntity m_entity;
    bool m_show_demo = true;
  };

}

int main()
{
  const std::filesystem::path assets_directory = GF_EXAMPLE_ASSETS_DIRECTORY;
  const std::filesystem::path font_file = assets_directory / "Oxanium-Regular.ttf";

  gf::SingleSceneManager scene_manager("15-imgui | gf2", gf::vec(1600, 900));

  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.IniFilename = nullptr;
  io.Fonts->AddFontFromFileTTF(font_file.string().c_str(), 20);

  ImguiScene scene(&scene_manager);
  scene_manager.run(&scene);

  ImGui::DestroyContext();
  return EXIT_SUCCESS;
}
