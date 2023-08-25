// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cassert>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <string>

#include <gf2/Font.h>
#include <gf2/FontManager.h>
#include <gf2/Range.h>
#include <gf2/StringUtils.h>

int main(int argc, char* argv[])
{
  if (argc == 1 || argc > 4) {
    std::cerr << "gf2_glyph_display <font> [char] [size]\n";
    return EXIT_FAILURE;
  }

  gf::FontManager font_manager;
  gf::Font font(argv[1], &font_manager);

  std::cout << "backend: " << font_manager.backend() << '\n';
  std::cout << "family: " << font.family_name() << '\n';
  std::cout << "style: " << font.style_name() << '\n';

  char32_t c = U'A';

  if (argc > 2) {
    auto codepoints = gf::codepoints(argv[2]);

    if (codepoints.begin() != codepoints.end()) {
      c = *codepoints.begin();
    }
  }

  unsigned character_size = 32;

  if (argc > 3) {
    character_size = static_cast<unsigned>(std::atoi(argv[3]));
  }

  const auto glyph = font.create_glyph(c, character_size);
  const auto size = glyph.bitmap.size();

  constexpr std::string_view Gray = " .:-=+*#%@";

  for (auto position : gf::position_range(size)) {
    auto gray = glyph.bitmap(position);
    const std::size_t index = gray * Gray.size() / 256;

    assert(index < Gray.size());
    std::cout << Gray[index]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

    if (position.x + 1 == size.w) {
      std::cout << '\n';
    }
  }

  return EXIT_SUCCESS;
}
