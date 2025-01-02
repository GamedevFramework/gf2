// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/ResourceBundle.h>

namespace gf {

  void ResourceBundle::load_from(ResourceManager* manager)
  {
    if (m_callback) {
      m_callback(this, manager, ResourceAction::Load);
    }
  }

  void ResourceBundle::unload_from(ResourceManager* manager)
  {
    if (m_callback) {
      m_callback(this, manager, ResourceAction::Unload);
    }
  }

} // namespace gf
