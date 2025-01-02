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
    m_status = Status::Paused;
    do_pause();
  }

  void BasicScene::resume()
  {
    m_status = Status::Resumed;
    do_resume();
  }

  bool BasicScene::paused() const
  {
    return m_status == Status::Paused;
  }

  void BasicScene::hide()
  {
    m_visibility = Visibility::Hidden;
    do_hide();
  }

  void BasicScene::show()
  {
    m_visibility = Visibility::Shown;
    do_show();
  }

  bool BasicScene::hidden() const
  {
    return m_visibility == Visibility::Hidden;
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

  void BasicScene::render_part(RenderRecorder& recorder, ScenePart& part)
  {
    part.camera.update(m_surface_size);
    recorder.update_view(part.camera.compute_view_matrix(), part.camera.compute_viewport(m_surface_size));
    part.entities.render(recorder);
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
    m_world.camera.type = CameraType::Extend;
    m_hud.camera.type = CameraType::Window;
  }

  void Scene::set_world_center(Vec2F center)
  {
    m_world.camera.center = center;
  }

  void Scene::set_world_size(Vec2F size)
  {
    m_world.camera.size = size;
  }

  void Scene::add_model(Model* model)
  {
    m_models.add_model(model);
  }

  void Scene::add_world_entity(Entity* entity)
  {
    m_world.entities.add_entity(entity);
  }

  void Scene::add_hud_entity(Entity* entity)
  {
    m_hud.entities.add_entity(entity);
  }

  Vec2F Scene::position_to_world_location(Vec2I position)
  {
    return m_world.camera.position_to_location(position, surface_size());
  }

  Vec2I Scene::world_location_to_position(Vec2F location)
  {
    return m_world.camera.location_to_position(location, surface_size());
  }

  void Scene::update_entities(Time time)
  {
    m_models.update(time);
    m_world.entities.update(time);
    m_hud.entities.update(time);
  }

  void Scene::render_entities(RenderRecorder& recorder)
  {
    if (!m_world.entities.empty()) {
      render_part(recorder, m_world);
    }

    if (!m_hud.entities.empty()) {
      render_part(recorder, m_hud);
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
