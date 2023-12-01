// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ZOOM_H
#define GF_ZOOM_H

#include <gf2/core/Camera.h>

#include "Event.h"
#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API Zoom {
  public:
    Zoom(Camera* camera);

    void set_surface_size(Vec2I surface_size);

    void process_event(const Event& event);

  private:
    Camera* m_camera = nullptr;
    Vec2I m_mouse_position;
    Vec2I m_surface_size;

    enum class State {
      Stationary,
      Moving,
    };

    State m_state;
  };


};

#endif // GF_ZOOM_H
