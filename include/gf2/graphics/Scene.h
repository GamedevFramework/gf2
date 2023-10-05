// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SCENE_H
#define GF_SCENE_H

#include <cstdint>

#include <gf2/core/Camera.h>
#include <gf2/core/Color.h>
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

    void update_framebuffer_size(Vec2I size);

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

    Vec2I m_framebuffer_size = { 1, 1 };
    Color m_clear_color = Black;
  };

  class GF_GRAPHICS_API StandardScene : public Scene {
  public:
    StandardScene();

    void set_world_center(Vec2F center);
    void set_world_size(Vec2F size);

    void add_world_entity(Entity* entity);
    void add_hud_entity(Entity* entity);

  private:
    void do_update(Time time) override;
    void do_render(RenderRecorder& recorder) override;

    ScenePart m_world;
    ScenePart m_hud;
  };

}

#endif // GF_SCENE_H
