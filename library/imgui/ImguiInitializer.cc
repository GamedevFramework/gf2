// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/imgui/ImguiInitializer.h>

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
