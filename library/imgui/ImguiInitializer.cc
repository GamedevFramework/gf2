// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/imgui/ImguiInitializer.h>
// clang-format on

#include <imgui.h>

namespace gf {

  ImguiInitializer::ImguiInitializer()
  {
    ImGui::CreateContext();
  }

  ImguiInitializer::~ImguiInitializer()
  {
    ImGui::DestroyContext();
  }

}
