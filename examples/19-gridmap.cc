// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/Color.h>
#include <gf2/core/GridMap.h>
#include <gf2/core/Log.h>
#include <gf2/core/Range.h>
#include <gf2/core/ShapeBuffer.h>

#include <gf2/graphics/ActionGroup.h>
#include <gf2/graphics/Entity.h>
#include <gf2/graphics/RenderObject.h>
#include <gf2/graphics/RenderRecorder.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>
#include <gf2/graphics/Shape.h>

namespace {
  using namespace gf::literals;

  constexpr std::string_view Grid[] = {
    "##################################################",
    "#        #        #            #                 #",
    "#  ~~~~  #        #            #                 #",
    "#  ~~~~  #                                       #",
    "#  ~~~~  #        #            #                 #",
    "#  ~~~~  #        #            #                 #",
    "#        #        ##### ########### ######## #####",
    "####++####        #         #          #         #",
    "#                 #         #          #         #",
    "#   ##            #         #                    #",
    "#  #  #                                #         #",
    "# #    #          #         #          #         #",
    "##      #         #         #          #         #",
    "#                 ############# ############ #####",
    "##      #         #               #              #",
    "# #    #          #               #              #",
    "#  #  #           #       #       #              #",
    "#   ##    # # # # #       #       #              #",
    "#         #       #       #       #      ##      #",
    "#         #               #              ##      #",
    "##### #####       #       #       #              #",
    "#         #       #               #              #",
    "#         #       #               #              #",
    "#         # # # # ####################### ########",
    "#                                                #",
    "#    ##         ###            ~~~~~~~~~~        #",
    "#   ###         ###             ~~~~~~~~~~~~     #",
    "#         ##   ###                 ~~~~~~~~~~    #",
    "#         #     ##     #####      ~~~~~~~~       #",
    "#        ##     ##    ######        ~~~~~~~~     #",
    "###     #####        #######        ~~~~~~       #",
    "####               #########          ~~~~~~     #",
    "####                #####            ~~~~~~~     #",
    "#####    ####                                    #",
    "##       ######                ####  #### ####   #",
    "#            ####             #     #         #  #",
    "#      #########      ##      #     #         #  #",
    "#         ###         ##      # ##  ####   ###   #",
    "#        #  ##      ####      #   # #     #      #",
    "#      ##  ###     #####      #   # #     #      #",
    "#     ###   ##                 ###  #     #####  #",
    "#     ####       #####                           #",
    "#     ####     ####  ####                        #",
    "#      ##               ##                       #",
    "#               ##       ## ####                 #",
    "#   ###  ##    ###       #####                   #",
    "#    #####     ##                                #",
    "#      ##      ###                               #",
    "#              ##                                #",
    "##################################################",
  };

  constexpr gf::Vec2I MapSize = { 50, 50 };
  constexpr gf::Vec2I CellSize = { 64, 64 };

  template<typename Iterator, typename Predicate>
  constexpr bool check(Iterator begin, Iterator end, Predicate predicate)
  {
    for (Iterator iterator = begin; iterator != end; ++iterator) {
      if (!predicate(*iterator)) {
        return false;
      }
    }

    return true;
  }

  static_assert(std::size(Grid) == static_cast<std::size_t>(MapSize.y));
  static_assert(check(std::begin(Grid), std::end(Grid), [](std::string_view line) { return line.size() == std::size_t(MapSize.x); }));

  class GridEntity : public gf::Entity {
  public:
    GridEntity(gf::BasicSceneManager* scene_manager)
    : m_scene_manager(scene_manager)
    , m_grid_map(gf::GridMap::make_orthogonal(MapSize, CellSize))
    {
      for (auto position : gf::position_range(MapSize)) {
        switch (Grid[position.y][position.x]) { // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
          case ' ':
            m_grid_map.set_properties(position, gf::CellProperty::Walkable | gf::CellProperty::Transparent);
            break;
          case '+':
            m_grid_map.set_properties(position, gf::CellProperty::Walkable);
            break;
          case '~':
            m_grid_map.set_properties(position, gf::CellProperty::Transparent);
            break;
          case '#':
            m_grid_map.set_properties(position, gf::None);
            break;
          default:
            assert(false);
            break;
        }
      }
    }

    void update([[maybe_unused]] gf::Time time) override
    {
      gf::ShapeGroupBuffer buffer;

      for (auto position : gf::position_range(MapSize)) {
        gf::ShapeBuffer shape = gf::ShapeBuffer::make_rectangle(gf::RectF::from_position_size(position * CellSize, CellSize));

        if (m_grid_map.transparent(position) && m_grid_map.walkable(position)) {
          // walkable, transparent (ground)
          shape.color = gf::Color(0xAB7A48);
        } else if (m_grid_map.walkable(position)) {
          // walkable, no transparent (invisible wall)
          shape.color = gf::Yellow;
        } else if (m_grid_map.transparent(position)) {
          // transparent, not walkable (water)
          shape.color = gf::Capri;
        } else {
          // not transparent, not walkable (wall)
          shape.color = gf::Gray;
        }

        if (m_grid_map.visible(position)) {
          shape.color = gf::lighter(shape.color);
        } else if (m_grid_map.explored(position)) {
          shape.color = gf::lighter(shape.color, 0.1f);
        }

        buffer.shapes.push_back(std::move(shape));
      }

      m_grid_display.update(buffer, m_scene_manager->render_manager());
    }

    void render(gf::RenderRecorder& recorder) override
    {
      gf::RenderObject object;
      object.geometry = m_grid_display.geometry();

      if (object.geometry.size > 0) {
        recorder.record(object);
      }
    }

    gf::GridMap& map()
    {
      return m_grid_map;
    }

    const gf::AnyGrid& grid() const
    {
      return m_grid_map.grid();
    }

  private:
    gf::BasicSceneManager* m_scene_manager = nullptr;
    gf::GridMap m_grid_map;
    gf::ShapeGroup m_grid_display;
  };

  struct State {
    gf::Vec2I light = { 0, 0 };
    int radius = 6;
    gf::Visibility visibility = gf::Visibility::ShadowCast;
  };

  class GridScene : public gf::Scene {
  public:
    GridScene(gf::BasicSceneManager* scene_manager)
    : m_scene_manager(scene_manager)
    , m_grid_entity(scene_manager)
    {
      m_actions.add_action("RayCast"_id, gf::ActionSettings().add_keycode_control(gf::Keycode::Num1));
      m_actions.add_action("ShadowCast"_id, gf::ActionSettings().add_keycode_control(gf::Keycode::Num2));
      m_actions.add_action("DiamondWalls"_id, gf::ActionSettings().add_keycode_control(gf::Keycode::Num3));
      m_actions.add_action("Permissive"_id, gf::ActionSettings().add_keycode_control(gf::Keycode::Num4));
      m_actions.add_action("Improved"_id, gf::ActionSettings().add_keycode_control(gf::Keycode::Num5));

      m_actions.add_action("clear"_id, gf::ActionSettings().add_keycode_control(gf::Keycode::C));

      set_world_size(MapSize * CellSize);
      set_world_center(MapSize * CellSize / 2);

      add_world_entity(&m_grid_entity);
    }

  private:
    void do_process_event(const gf::Event& event) override
    {
      switch (event.type()) {
        case gf::EventType::MouseMoved:
          {
            const auto& mouse_motion_event = event.from<gf::EventType::MouseMoved>();
            auto mouse_position = mouse_motion_event.position;
            auto mouse_location = position_to_world_location(mouse_position);
            const gf::Vec2I position = m_grid_entity.grid().compute_position(mouse_location);

            if (position != m_state.light) {
              m_state.light = position;
              m_grid_entity.map().clear_visible();
              m_grid_entity.map().compute_field_of_vision(m_state.light, m_state.radius, m_state.visibility);
            }
          }
          break;
        default:
          break;
      }

      m_actions.process_event(event);
    }

    void do_handle_actions() override
    {
      gf::Visibility visibility = m_state.visibility;

      if (m_actions.active("RayCast"_id)) {
        gf::Log::info("Visibility: RayCast");
        visibility = gf::Visibility::RayCast;
      }

      if (m_actions.active("ShadowCast"_id)) {
        gf::Log::info("Visibility: ShadowCast");
        visibility = gf::Visibility::ShadowCast;
      }

      if (m_actions.active("DiamondWalls"_id)) {
        gf::Log::info("Visibility: DiamondWalls");
        visibility = gf::Visibility::DiamondWalls;
      }

      if (m_actions.active("Permissive"_id)) {
        gf::Log::info("Visibility: Permissive");
        visibility = gf::Visibility::Permissive;
      }

      if (m_actions.active("Improved"_id)) {
        gf::Log::info("Visibility: Improved");
        visibility = gf::Visibility::Improved;
      }

      if (visibility != m_state.visibility) {
        m_state.visibility = visibility;
        m_grid_entity.map().clear_visible();
        m_grid_entity.map().compute_field_of_vision(m_state.light, m_state.radius, m_state.visibility);
      }

      if (m_actions.active("clear"_id)) {
        m_grid_entity.map().clear_explored();
      }

      m_actions.reset();
    }

    gf::BasicSceneManager* m_scene_manager = nullptr;
    GridEntity m_grid_entity;
    gf::ActionGroup m_actions;
    State m_state;
  };

}

int main()
{
  gf::SingleSceneManager scene_manager("18-activities | gf2", gf::vec(1600, 900));

  GridScene scene(&scene_manager);

  return scene_manager.run(&scene);
}
