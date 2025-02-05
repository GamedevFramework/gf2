// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TOUCH_H
#define GF_TOUCH_H

#include <vector>

#include <gf2/core/TouchTypes.h>
#include <gf2/core/ZString.h>

#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API Touch {

    static std::vector<TouchId> devices();
    static ZString name(TouchId id);
    static TouchDeviceType type(TouchId id);

  };

}

#endif // GF_TOUCH_H
