// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Scene.h>
// clang-format on

#include <gf2/graphics/RenderRecorder.h>

#include "gf2/core/Camera.h"

namespace gf {

  /*
   * Scene
   */

  Scene::~Scene() = default;

  void Scene::update_framebuffer_size(Vec2I size)
  {
    m_framebuffer_size = size;
  }

  void Scene::set_clear_color(Color color)
  {
    m_clear_color = color;
  }

  Color Scene::clear_color() const
  {
    return m_clear_color;
  }

  void Scene::set_rank(SceneRank rank)
  {
    if (m_rank != rank) {
      on_rank_change(m_rank, rank);
      m_rank = rank;
    }
  }

  void Scene::pause()
  {
    m_status = Status::Paused;
    do_pause();
  }

  void Scene::resume()
  {
    m_status = Status::Resumed;
    do_resume();
  }

  bool Scene::paused() const
  {
    return m_status == Status::Paused;
  }

  void Scene::hide()
  {
    m_visibility = Visibility::Hidden;
    do_hide();
  }

  void Scene::show()
  {
    m_visibility = Visibility::Shown;
    do_show();
  }

  bool Scene::hidden() const
  {
    return m_visibility == Visibility::Hidden;
  }

  void Scene::process_event([[maybe_unused]] const Event& event)
  {
    // TODO
  }

  void Scene::update(Time time)
  {
    if (paused()) {
      return;
    }

    do_update(time);
  }

  void Scene::render(RenderRecorder& recorder)
  {
    if (hidden()) {
      return;
    }

    do_render(recorder);
  }

  void Scene::render_part([[maybe_unused]] RenderRecorder& recorder, [[maybe_unused]] ScenePart& part)
  {
    part.camera.update(m_framebuffer_size);
    recorder.update_view(part.camera.compute_view_matrix(), part.camera.compute_viewport(m_framebuffer_size));
    part.entities.render(recorder);
  }

  void Scene::on_rank_change([[maybe_unused]] SceneRank old_rank, [[maybe_unused]] SceneRank new_rank)
  {
    // do nothing by default
  }

  void Scene::do_pause()
  {
    // do nothing by default
  }

  void Scene::do_resume()
  {
    // do nothing by default
  }

  void Scene::do_show()
  {
    // do nothing by default
  }

  void Scene::do_hide()
  {
    // do nothing by default
  }

  bool Scene::do_early_process_event([[maybe_unused]] const Event& event)
  {
    // do nothing by default
    return false;
  }

  void Scene::do_process_event([[maybe_unused]] const Event& event)
  {
    // do nothing by default
  }

  void Scene::do_update([[maybe_unused]] Time time)
  {
    // do nothing by default
  }

  void Scene::do_render([[maybe_unused]] RenderRecorder& recorder)
  {
    // do nothing by default
  }

  /*
   * SingleScene
   */

  StandardScene::StandardScene()
  {
    m_world.camera.type = CameraType::Extend;
    m_hud.camera.type = CameraType::Window;
  }

  void StandardScene::set_world_center(Vec2F center)
  {
    m_world.camera.center = center;
  }

  void StandardScene::set_world_size(Vec2F size)
  {
    m_world.camera.size = size;
  }

  void StandardScene::add_world_entity(Entity* entity)
  {
    m_world.entities.add_entity(entity);
  }

  void StandardScene::add_hud_entity(Entity* entity)
  {
    m_hud.entities.add_entity(entity);
  }

  void StandardScene::do_update([[maybe_unused]] Time time)
  {
    m_world.entities.update(time);
    m_hud.entities.update(time);
  }

  void StandardScene::do_render(RenderRecorder& recorder)
  {
    if (!m_world.entities.empty()) {
      render_part(recorder, m_world);
    }

    if (!m_hud.entities.empty()) {
      render_part(recorder, m_hud);
    }
  }

}