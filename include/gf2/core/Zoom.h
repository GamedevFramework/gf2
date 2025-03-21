// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_ZOOM_H
#define GF_ZOOM_H

#include <cstdint>

#include "Camera.h"
#include "CoreApi.h"
#include "Event.h"

namespace gf {

  class GF_CORE_API Zoom {
  public:
    Zoom(Camera* camera);

    void set_surface_size(Vec2I surface_size);

    void process_event(const Event& event);

  private:
    Camera* m_camera = nullptr;
    Vec2I m_mouse_position;
    Vec2I m_surface_size;

    enum class State : uint8_t {
      Stationary,
      Moving,
    };

    State m_state = State::Stationary;
  };

};

#endif // GF_ZOOM_H
