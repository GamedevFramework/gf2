// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/graphics/Scene.h>

#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  /*
   * BasicScene
   */

  BasicScene::~BasicScene() = default;

  void BasicScene::set_surface_size(Vec2I size)
  {
    m_surface_size = size;
    on_resize(size);
  }

  Vec2I BasicScene::surface_size() const
  {
    return m_surface_size;
  }

  void BasicScene::set_clear_color(Color color)
  {
    m_clear_color = gf::srgb_to_linear(color);
  }

  Color BasicScene::clear_color() const
  {
    return m_clear_color;
  }

  void BasicScene::set_rank(SceneRank rank)
  {
    if (m_rank != rank) {
      on_rank_change(m_rank, rank);
      m_rank = rank;
    }
  }

  SceneRank BasicScene::rank() const
  {
    return m_rank;
  }

  void BasicScene::pause()
  {
    m_status = SceneStatus::Paused;
    do_pause();
  }

  void BasicScene::resume()
  {
    m_status = SceneStatus::Resumed;
    do_resume();
  }

  bool BasicScene::paused() const
  {
    return m_status == SceneStatus::Paused;
  }

  void BasicScene::hide()
  {
    m_visibility = SceneVisibility::Hidden;
    do_hide();
  }

  void BasicScene::show()
  {
    m_visibility = SceneVisibility::Shown;
    do_show();
  }

  bool BasicScene::hidden() const
  {
    return m_visibility == SceneVisibility::Hidden;
  }

  void BasicScene::process_event(const Event& event)
  {
    if (!do_early_process_event(event)) {
      do_process_event(event);
    }
  }

  void BasicScene::handle_actions()
  {
    do_handle_actions();
  }

  void BasicScene::update(Time time)
  {
    if (paused()) {
      return;
    }

    do_update(time);
  }

  void BasicScene::render(RenderRecorder& recorder)
  {
    if (hidden()) {
      return;
    }

    do_render(recorder);
  }

  void BasicScene::render_part_start(RenderRecorder& recorder, Camera& camera)
  {
    camera.update(m_surface_size);
    recorder.update_view(camera.compute_view_matrix(), camera.compute_viewport(m_surface_size));
  }

  void BasicScene::on_resize([[maybe_unused]] Vec2I surface_size)
  {
    // do nothing by default
  }

  void BasicScene::on_rank_change([[maybe_unused]] SceneRank old_rank, [[maybe_unused]] SceneRank new_rank)
  {
    // do nothing by default
  }

  void BasicScene::do_pause()
  {
    // do nothing by default
  }

  void BasicScene::do_resume()
  {
    // do nothing by default
  }

  void BasicScene::do_show()
  {
    // do nothing by default
  }

  void BasicScene::do_hide()
  {
    // do nothing by default
  }

  bool BasicScene::do_early_process_event([[maybe_unused]] const Event& event)
  {
    // do nothing by default
    return false;
  }

  void BasicScene::do_process_event([[maybe_unused]] const Event& event)
  {
    // do nothing by default
  }

  void BasicScene::do_handle_actions()
  {
    // do nothing by default
  }

  void BasicScene::do_update([[maybe_unused]] Time time)
  {
    // do nothing by default
  }

  void BasicScene::do_render([[maybe_unused]] RenderRecorder& recorder)
  {
    // do nothing by default
  }

  /*
   * Scene
   */

  Scene::Scene()
  {
    m_world_camera.type = CameraType::Extend;
    m_hud_camera.type = CameraType::Window;
  }

  void Scene::set_world_center(Vec2F center)
  {
    m_world_camera.center = center;
  }

  void Scene::set_world_size(Vec2F size)
  {
    m_world_camera.size = size;
  }

  void Scene::add_model(Model* model)
  {
    m_models.add_model(model);
  }

  void Scene::add_world_entity(Entity* entity)
  {
    if (entity != nullptr) {
      m_world_entities.push_back(entity);
    }
  }

  void Scene::add_hud_entity(HudEntity* entity)
  {
    if (entity != nullptr) {
      m_hud_entities.push_back(entity);
    }
  }

  Vec2F Scene::position_to_world_location(Vec2I position)
  {
    return m_world_camera.position_to_location(position, surface_size());
  }

  Vec2I Scene::world_location_to_position(Vec2F location)
  {
    return m_world_camera.location_to_position(location, surface_size());
  }

  void Scene::update_entities(Time time)
  {
    m_models.update(time);

    for (Entity* entity : m_world_entities) {
      entity->update(time);
    }

    for (HudEntity* entity : m_hud_entities) {
      entity->update(time);
    }
  }

  void Scene::render_entities(RenderRecorder& recorder)
  {
    if (!m_world_entities.empty()) {
      render_part(recorder, m_world_entities, m_world_camera);
    }

    if (!m_hud_entities.empty()) {
      render_part(recorder, m_hud_entities, m_hud_camera);
    }
  }

  void Scene::on_resize(Vec2I surface_size)
  {
    for (HudEntity* entity : m_hud_entities) {
      entity->resize(surface_size);
    }
  }

  void Scene::do_update(Time time)
  {
    update_entities(time);
  }

  void Scene::do_render(RenderRecorder& recorder)
  {
    render_entities(recorder);
  }

}
