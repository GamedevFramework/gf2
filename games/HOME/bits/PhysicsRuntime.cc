#include "PhysicsRuntime.h"

namespace home {

  PhysicsRuntime::PhysicsRuntime(gf::RenderManager* render_manager)
  : m_entity(render_manager)
  {
  }

  void PhysicsRuntime::start_debug()
  {
    m_entity.start_debug();
  }

  void PhysicsRuntime::stop_debug()
  {
    m_entity.stop_debug();
  }

  void PhysicsRuntime::draw_circle(gf::Vec2F center, float angle, float radius, gf::Color outline_color, gf::Color fill_color)
  {
    m_entity.draw_circle(center, angle, radius, outline_color, fill_color);
  }

  void PhysicsRuntime::draw_segment(gf::Vec2F a, gf::Vec2F b, gf::Color color)
  {
    m_entity.draw_segment(a, b, color);
  }

  void PhysicsRuntime::draw_fat_segment(gf::Vec2F a, gf::Vec2F b, float radius, gf::Color outline_color, gf::Color fill_color)
  {
    m_entity.draw_fat_segment(a, b, radius, outline_color, fill_color);
  }

  void PhysicsRuntime::draw_polygon(gf::Span<const gf::Vec2F> points, float radius, gf::Color outline_color, gf::Color fill_color)
  {
    m_entity.draw_polygon(points, radius, outline_color, fill_color);
  }

  void PhysicsRuntime::draw_dot(float size, gf::Vec2F location, gf::Color color)
  {
    m_entity.draw_dot(size, location, color);
  }

}
