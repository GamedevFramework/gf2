// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CLIPBOARD_H
#define GF_CLIPBOARD_H

#include <string>

#include "GraphicsApi.h"

namespace gf {

  class GF_GRAPHICS_API Clipboard {
  public:
    static std::string text();
    static void set_text(const std::string& text);
  };

}

#endif // GF_CLIPBOARD_H
