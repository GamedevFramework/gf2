// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/Scene.h>
// clang-format on

#include <gf2/graphics/RenderRecorder.h>

namespace gf {

  /*
   * Scene
   */

  Scene::~Scene() = default;

  void Scene::set_surface_size(Vec2I size)
  {
    m_surface_size = size;
  }

  Vec2I Scene::surface_size() const
  {
    return m_surface_size;
  }

  void Scene::set_clear_color(Color color)
  {
    m_clear_color = gf::srgb_to_linear(color);
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

  void Scene::process_event(const Event& event)
  {
    if (!do_early_process_event(event)) {
      do_process_event(event);
    }
  }

  void Scene::handle_actions()
  {
    do_handle_actions();
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
    part.camera.update(m_surface_size);
    recorder.update_view(part.camera.compute_view_matrix(), part.camera.compute_viewport(m_surface_size));
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

  void Scene::do_handle_actions()
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
   * StandardScene
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

  void StandardScene::add_model(Model* model)
  {
    m_models.add_model(model);
  }

  void StandardScene::add_world_entity(Entity* entity)
  {
    m_world.entities.add_entity(entity);
  }

  void StandardScene::add_hud_entity(Entity* entity)
  {
    m_hud.entities.add_entity(entity);
  }

  Vec2F StandardScene::position_to_world_location(Vec2I position)
  {
    const RectF viewport = m_world.camera.compute_viewport(surface_size());

    /* simulate inverse projection transform
     * i.e. compute normalized device coordinates from screen coordinates
     *
     * 0 +---------+     -1 +---------+
     *   |         |        |         |
     *   |         | ===>   |         |
     *   |         |        |         |
     * h +---------+      1 +---------+
     *   0         w       -1         1
     */
    const Vec2F normalized = 2 * (position - viewport.offset) / viewport.extent - 1;

    /* apply inverse view transform
     * i.e. compute world coordinates from normalized device coordinates
     */
    return transform_point(inverse(m_world.camera.compute_view_matrix()), normalized);
  }

  Vec2I StandardScene::world_location_to_position(Vec2F location)
  {
    const RectF viewport = m_world.camera.compute_viewport(surface_size());

    /* apply view transform
     * i.e. compute normalized device coordinates from world coordinates
     */
    const Vec2F normalized = transform_point(m_world.camera.compute_view_matrix(), location);

    /* simulate projection transform
     * i.e. compute screen coordinates from normalized device coordinates
     *
     * -1 +---------+     0 +---------+
     *    |         |       |         |
     *    |         | ===>  |         |
     *    |         |       |         |
     *  1 +---------+     h +---------+
     *   -1         1       0         w
     */
    return (1 + normalized) / 2 * viewport.extent + viewport.offset;
  }

  void StandardScene::update_entities(Time time)
  {
    m_models.update(time);
    m_world.entities.update(time);
    m_hud.entities.update(time);
  }

  void StandardScene::render_entities(RenderRecorder& recorder)
  {
    if (!m_world.entities.empty()) {
      render_part(recorder, m_world);
    }

    if (!m_hud.entities.empty()) {
      render_part(recorder, m_hud);
    }
  }

  void StandardScene::do_update(Time time)
  {
    update_entities(time);
  }

  void StandardScene::do_render(RenderRecorder& recorder)
  {
    render_entities(recorder);
  }

}
