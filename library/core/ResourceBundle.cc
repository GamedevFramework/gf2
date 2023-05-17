// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/ResourceBundle.h>
// clang-format on

namespace gf {

  void ResourceBundle::load_from(ResourceManager& manager)
  {
    if (m_callback) {
      m_callback(*this, manager, ResourceBundleAction::Load);
    }
  }

  void ResourceBundle::unload_from(ResourceManager& manager)
  {
    if (m_callback) {
      m_callback(*this, manager, ResourceBundleAction::Unload);
    }
  }

} // namespace gf
