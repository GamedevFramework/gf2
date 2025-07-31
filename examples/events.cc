// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Log.h>

#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>

namespace {

  class EventScene : public gf::Scene {
  public:
    EventScene()
    {
      set_world_size({ 1000.0f, 1000.0f });
      set_world_center({ 0.0f, 0.0f });
    }

  private:
    void do_process_event(const gf::Event& event) override
    {
      switch (event.type()) {
        // case gf::EventType::MouseMoved:
        //   {
        //     const auto& mouse_motion_event = event.from<gf::EventType::MouseMoved>();
        //     auto mouse_position = mouse_motion_event.position;
        //     auto mouse_location = position_to_world_location(mouse_position);
        //
        //     gf::Log::info("mouse: position = ({},{}), location = ({},{})", mouse_position.x, mouse_position.y, mouse_location.x, mouse_location.y);
        //   }
        //   break;

        case gf::EventType::MouseButtonPressed:
          {
            const auto& event_data = event.from<gf::EventType::MouseButtonPressed>();
            auto mouse_position = event_data.position;
            auto mouse_location = position_to_world_location(mouse_position);

            gf::Log::info("mouse: position = ({},{}), location = ({},{})", mouse_position.x, mouse_position.y, mouse_location.x, mouse_location.y);
          }
          break;

        default:
          break;
      }
    }
  };

}

int main()
{
  gf::SingleSceneManager scene_manager("events | gf2", gf::vec(1600, 900));
  EventScene scene;
  return scene_manager.run(&scene);
}
