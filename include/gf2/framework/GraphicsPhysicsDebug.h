#ifndef GF_GRAPHICS_PHYSICS_DEBUG_H
#define GF_GRAPHICS_PHYSICS_DEBUG_H

#include <gf2/graphics/Entity.h>
#include <gf2/graphics/Curve.h>
#include <gf2/graphics/Shape.h>

#include <gf2/physics/PhysicsDebug.h>
#include <gf2/physics/PhysicsWorld.h>

#include "FrameworkApi.h"

namespace gf {
  class RenderManager;
  class RenderRecorder;

  class GF_FRAMEWORK_API GraphicsPhysicsDebug : public PhysicsDebug {
  public:
    void clear();

    void draw_circle(Vec2F center, float angle, float radius, Color outline_color, Color fill_color) override;
    void draw_segment(Vec2F a, Vec2F b, Color color) override;
    void draw_fat_segment(Vec2F a, Vec2F b, float radius, Color outline_color, Color fill_color) override;
    void draw_polygon(Span<const Vec2F> points, float radius, Color outline_color, Color fill_color) override;
    void draw_dot(float size, Vec2F location, Color color) override;

    const ShapeGroupBuffer& shapes() const
    {
      return m_shape_group_buffer;
    }

    const CurveGroupBuffer& curves() const
    {
      return m_curve_group_buffer;
    }

  private:
    friend class GraphicsPhysicsDebugEntity;

    ShapeGroupBuffer m_shape_group_buffer;
    CurveGroupBuffer m_curve_group_buffer;
  };

  class GF_FRAMEWORK_API GraphicsPhysicsDebugEntity : public Entity {
  public:
    GraphicsPhysicsDebugEntity(PhysicsWorld* physics_world, RenderManager* render_manager);

    bool awake() const
    {
      return m_awake;
    }

    void set_awake(bool awake)
    {
      m_awake = awake;
    }

    void update(Time time) override;
    void render(RenderRecorder& recorder) override;

    GraphicsPhysicsDebug* physics_debug();

  private:
    PhysicsWorld* m_physics_world = nullptr;
    RenderManager* m_render_manager = nullptr;
    GraphicsPhysicsDebug m_debug;
    ShapeGroup m_shape_group;
    CurveGroup m_curve_group;
    bool m_awake = false;
  };

}

#endif // GF_GRAPHICS_PHYSICS_DEBUG_H
