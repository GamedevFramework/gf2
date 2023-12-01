// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SCENE_H
#define GF_SCENE_H

#include <cstdint>

#include <gf2/core/Camera.h>
#include <gf2/core/Color.h>
#include <gf2/core/ModelContainer.h>
#include <gf2/core/Time.h>

#include "EntityContainer.h"
#include "Event.h"
#include "GraphicsApi.h"

namespace gf {
  class RenderRecorder;

  enum class SceneRank : uint8_t {
    None,
    Active,
    Top,
  };

  class GF_GRAPHICS_API Scene {
  public:
    Scene() = default;
    Scene(const Scene&) = delete;
    Scene(Scene&&) noexcept = default;
    virtual ~Scene();

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) noexcept = default;

    void set_surface_size(Vec2I size);
    Vec2I surface_size() const;

    void set_clear_color(Color color);
    Color clear_color() const;

    void set_rank(SceneRank rank);

    void pause();
    void resume();
    bool paused() const;

    void hide();
    void show();
    bool hidden() const;

    void process_event(const Event& event);
    void handle_actions();
    void update(Time time);
    void render(RenderRecorder& recorder);

  protected:
    struct ScenePart {
      EntityContainer entities;
      Camera camera;
    };

    void render_part(RenderRecorder& recorder, ScenePart& part);

  private:
    virtual void on_rank_change(SceneRank old_rank, SceneRank new_rank);

    virtual void do_pause();
    virtual void do_resume();
    virtual void do_show();
    virtual void do_hide();

    virtual bool do_early_process_event(const Event& event);
    virtual void do_process_event(const Event& event);
    virtual void do_handle_actions();
    virtual void do_update(Time time);
    virtual void do_render(RenderRecorder& recorder);

    SceneRank m_rank = SceneRank::None;

    enum class Status : uint8_t {
      Resumed,
      Paused,
    };

    Status m_status = Status::Resumed;

    enum class Visibility : uint8_t {
      Shown,
      Hidden,
    };

    Visibility m_visibility = Visibility::Shown;

    Vec2I m_surface_size = { 1, 1 };
    Color m_clear_color = Black;
  };

  class GF_GRAPHICS_API StandardScene : public Scene {
  public:
    StandardScene();

    void set_world_center(Vec2F center);
    void set_world_size(Vec2F size);

    void add_model(Model* model);

    void add_world_entity(Entity* entity);
    void add_hud_entity(Entity* entity);

    Vec2F position_to_world_location(Vec2I position);
    Vec2I world_location_to_position(Vec2F location);

  protected:
    void update_entities(Time time);
    void render_entities(RenderRecorder& recorder);

    Camera* world_camera()
    {
      return &m_world.camera;
    }

  private:
    void do_update(Time time) override;
    void do_render(RenderRecorder& recorder) override;

    ModelContainer m_models;
    ScenePart m_world;
    ScenePart m_hud;
  };

}

#endif // GF_SCENE_H
