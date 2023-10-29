// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_KEYBOARD_H
#define GF_KEYBOARD_H

#include <gf2/core/Keycode.h>
#include <gf2/core/Modifier.h>
#include <gf2/core/Scancode.h>

#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API Keyboard {
    static const char* scancode_name(Scancode scancode);
    static Scancode scancode_from_name(const char* name);

    static const char* keycode_name(Keycode keycode);
    static Keycode keycode_from_name(const char* name);

    static Keycode localize(Scancode scancode);
    static Scancode unlocalize(Keycode keycode);
  };

} // namespace gf

#endif // GF_KEYBOARD_H
