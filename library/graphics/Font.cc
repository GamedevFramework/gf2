// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/graphics/Font.h>

namespace gf {

  Font::Font(const std::filesystem::path& filename, FontContext context)
  : m_face(filename, context.font_manager)
  , m_atlas(&m_face, context.size, context.render_manager)
  {
  }

}
