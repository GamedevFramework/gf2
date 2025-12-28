#ifndef GF_GRAPHICS_PHYSICS_DEBUG_H
#define GF_GRAPHICS_PHYSICS_DEBUG_H

#include <gf2/core/Color.h>

#include <gf2/graphics/Curve.h>
#include <gf2/graphics/Entity.h>
#include <gf2/graphics/Shape.h>
#include <gf2/graphics/TextEntity.h>
#include <gf2/physics/PhysicsDebug.h>
#include <gf2/physics/PhysicsWorld.h>

#include "FrameworkApi.h"

namespace gf {
  class RenderManager;
  class RenderRecorder;

  struct GF_FRAMEWORK_API TextDebugData {
    std::string content;
    Color color;
    Vec2F location;
  };

  class GF_FRAMEWORK_API GraphicsPhysicsDebug : public PhysicsDebug {
  public:
    void clear();

    void draw_polygon(Span<const Vec2F> polygon, Color color) override;
    void draw_solid_polygon(Span<const Vec2F> polygon, float radius, Color color) override;
    void draw_circle(CircF circle, Color color) override;
    void draw_solid_circle(CircF circle, float angle, Color color) override;
    void draw_solid_capsule(const CapsuleF& capsule, Color color) override;
    void draw_line(const SegmentF& segment, Color color) override;
    void draw_transform(Vec2F location, float rotation) override;
    void draw_point(CircF point, Color color) override;
    void draw_string(Vec2F point, std::string_view string, Color color) override;

    const ShapeGroupBuffer& shapes() const
    {
      return m_shape_group_buffer;
    }

    const CurveGroupBuffer& curves() const
    {
      return m_curve_group_buffer;
    }

    const std::vector<TextDebugData>& texts() const
    {
      return m_texts;
    }

  private:
    ShapeGroupBuffer m_shape_group_buffer;
    CurveGroupBuffer m_curve_group_buffer;
    std::vector<TextDebugData> m_texts;
  };

  class GF_FRAMEWORK_API GraphicsPhysicsDebugEntity : public Entity {
  public:
    GraphicsPhysicsDebugEntity(const PhysicsDebugOptions& options, PhysicsWorld* physics_world, RenderManager* render_manager);

    bool awake() const
    {
      return m_awake;
    }

    void set_awake(bool awake)
    {
      m_awake = awake;
    }

    void set_options(const PhysicsDebugOptions& options)
    {
      m_options = options;
    }

    void set_text_parameters(FontAtlas* atlas, FontFace* font_face, float character_size)
    {
      m_atlas = atlas;
      m_font_face = font_face;
      m_character_size = character_size;
    }

    void update(Time time) override;
    void render(RenderRecorder& recorder) override;

    GraphicsPhysicsDebug* physics_debug();

  private:
    PhysicsDebugOptions m_options;
    PhysicsWorld* m_physics_world = nullptr;
    RenderManager* m_render_manager = nullptr;
    GraphicsPhysicsDebug m_debug;
    ShapeGroup m_shape_group;
    CurveGroup m_curve_group;
    FontAtlas* m_atlas = nullptr;
    FontFace* m_font_face = nullptr;
    float m_character_size = 0.0f;
    std::vector<TextEntity> m_texts;
    bool m_awake = false;
  };

}

#endif // GF_GRAPHICS_PHYSICS_DEBUG_H
