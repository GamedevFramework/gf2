// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_MOUSE_H
#define GF_MOUSE_H

#include <vector>

#include <gf2/core/MouseTypes.h>
#include <gf2/core/ZString.h>

#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API Mouse {

    static bool connected();
    static std::vector<MouseId> devices();
    static ZString name(MouseId id);
    static void capture(bool captured = true);

  };

}

#endif // GF_MOUSE_H
