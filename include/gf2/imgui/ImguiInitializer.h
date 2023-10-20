// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_IMGUI_INITIALIZER_H
#define GF_IMGUI_INITIALIZER_H

#include "ImguiApi.h"

namespace gf {

  class GF_IMGUI_API ImguiInitializer {
  public:
    ImguiInitializer();
    ImguiInitializer(const ImguiInitializer&) = default;
    ImguiInitializer(ImguiInitializer&&) noexcept = default;
    ~ImguiInitializer();

    ImguiInitializer& operator=(const ImguiInitializer&) = default;
    ImguiInitializer& operator=(ImguiInitializer&&) noexcept = default;
  };

}

#endif // GF_IMGUI_INITIALIZER_H
