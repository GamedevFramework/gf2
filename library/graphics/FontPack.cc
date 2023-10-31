// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/graphics/FontPack.h>
// clang-format on

namespace gf {

  FontPack::FontPack(const std::filesystem::path& filename, FontPackContext context)
  : m_font(filename, context.font_manager)
  , m_atlas(&m_font, context.size, context.render_manager)
  {
  }

}
