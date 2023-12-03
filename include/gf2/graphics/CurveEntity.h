// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_CURVE_ENTITY_H
#define GF_CURVE_ENTITY_H

#include "Curve.h"
#include "GraphicsApi.h"
#include "Texture.h"
#include "TransformableEntity.h"

namespace gf {
  class RenderManager;

  class GF_GRAPHICS_API CurveEntity : public TransformableEntity {
  public:
    CurveEntity(const CurveData& data, RenderManager* render_manager);

    void render(RenderRecorder& recorder) override;

  private:
    Curve m_curve;
  };

}

#endif // GF_CURVE_ENTITY_H
