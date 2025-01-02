// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <gf2/core/Color.h>

#include <gf2/graphics/CurveEntity.h>
#include <gf2/graphics/GraphicsInitializer.h>
#include <gf2/graphics/Scene.h>
#include <gf2/graphics/SceneManager.h>

int main()
{
  gf::SingleSceneManager scene_manager("14-curve | gf2", gf::vec(1600, 900));

  constexpr gf::Vec2F SplinePoints[] = {
    {  50.0f, 500.0f },
    { 150.0f, 550.0f },
    { 150.0f, 450.0f },
    { 300.0f, 500.0f },
    { 700.0f, 500.0f },
    { 700.0f, 200.0f },
    { 600.0f, 500.0f },
  };

  gf::CurveBuffer line_buffer = gf::CurveBuffer::make_line({ 10.0f, 10.0f }, { 250.0f, 450.0f });
  line_buffer.color = gf::Chartreuse;
  line_buffer.thickness = 10.0f;

  gf::CurveEntity line_entity(line_buffer, scene_manager.render_manager());

  gf::CurveBuffer spline_buffer = gf::CurveBuffer::make_cattmull_rom_spline(SplinePoints);
  spline_buffer.color = gf::Orange;
  spline_buffer.thickness = 8.0f;
  spline_buffer.outline_color = gf::darker(gf::Orange);
  spline_buffer.outline_thickness = 2.0f;

  gf::CurveEntity spline_entity(spline_buffer, scene_manager.render_manager());

  gf::Scene scene;
  scene.set_world_center({ 400.0f, 300.0f });
  scene.set_world_size({ 800.0f, 600.0f });

  scene.add_world_entity(&line_entity);
  scene.add_world_entity(&spline_entity);

  return scene_manager.run(&scene);
}
