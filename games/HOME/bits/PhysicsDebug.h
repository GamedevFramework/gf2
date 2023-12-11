#ifndef HOME_PHYSICS_DEBUG_H
#define HOME_PHYSICS_DEBUG_H

#include <gf2/graphics/DebugEntity.h>

#include <gf2/physics/PhysicsDebug.h>

namespace home {

  class PhysicsDebug : public gf::PhysicsDebug {
  public:
    PhysicsDebug(gf::RenderManager* render_manager);

    void start_debug();
    void stop_debug();

    void draw_circle(gf::Vec2F center, float angle, float radius, gf::Color outline_color, gf::Color fill_color) override;
    void draw_segment(gf::Vec2F a, gf::Vec2F b, gf::Color color) override;
    void draw_fat_segment(gf::Vec2F a, gf::Vec2F b, float radius, gf::Color outline_color, gf::Color fill_color) override;
    void draw_polygon(gf::Span<const gf::Vec2F> points, float radius, gf::Color outline_color, gf::Color fill_color) override;
    void draw_dot(float size, gf::Vec2F location, gf::Color color) override;

    gf::Entity* entity()
    {
      return &m_entity;
    }

  private:
    gf::DebugEntity m_entity;
  };

}

#endif // HOME_PHYSICS_DEBUG_H
