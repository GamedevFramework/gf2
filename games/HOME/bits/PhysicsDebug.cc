#include "PhysicsDebug.h"

namespace home {

  PhysicsDebug::PhysicsDebug(gf::RenderManager* render_manager)
  : m_entity(render_manager)
  {
  }

  void PhysicsDebug::start_debug()
  {
    m_entity.start_debug();
  }

  void PhysicsDebug::stop_debug()
  {
    m_entity.stop_debug();
  }

  void PhysicsDebug::draw_circle(gf::Vec2F center, float angle, float radius, gf::Color outline_color, gf::Color fill_color)
  {
    m_entity.draw_circle(center, angle, radius, outline_color, fill_color);
  }

  void PhysicsDebug::draw_segment(gf::Vec2F a, gf::Vec2F b, gf::Color color)
  {
    m_entity.draw_segment(a, b, color);
  }

  void PhysicsDebug::draw_fat_segment(gf::Vec2F a, gf::Vec2F b, float radius, gf::Color outline_color, gf::Color fill_color)
  {
    m_entity.draw_fat_segment(a, b, radius, outline_color, fill_color);
  }

  void PhysicsDebug::draw_polygon(gf::Span<const gf::Vec2F> points, float radius, gf::Color outline_color, gf::Color fill_color)
  {
    m_entity.draw_polygon(points, radius, outline_color, fill_color);
  }

  void PhysicsDebug::draw_dot(float size, gf::Vec2F location, gf::Color color)
  {
    m_entity.draw_dot(size, location, color);
  }

}
