// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_KEYBOARD_H
#define GF_KEYBOARD_H

#include <vector>
#include <tuple>

#include <gf2/core/Flags.h>
#include <gf2/core/KeyboardTypes.h>
#include <gf2/core/Keycode.h>
#include <gf2/core/Modifier.h>
#include <gf2/core/Scancode.h>
#include <gf2/core/ZString.h>

#include "GraphicsApi.h"

namespace gf {

  struct GF_GRAPHICS_API Keyboard {
    static bool connected();
    static std::vector<KeyboardId> devices();
    static ZString name(KeyboardId id);

    static const char* scancode_name(Scancode scancode);
    static Scancode scancode_from_name(const char* name);

    static const char* keycode_name(Keycode keycode);
    static Keycode keycode_from_name(const char* name);

    static Keycode localize(Scancode scancode, Flags<Modifier> modifiers);
    static std::tuple<Scancode, Flags<Modifier>> unlocalize(Keycode keycode);
  };

} // namespace gf

#endif // GF_KEYBOARD_H
