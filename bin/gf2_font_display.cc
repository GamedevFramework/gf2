#include <cassert>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <gf2/FontManager.h>
#include <gf2/Font.h>
#include <gf2/Range.h>

int main(int argc, char *argv[]) {
  if (argc == 1 || argc > 4) {
    std::cerr << "gf2_font_display <font> [char] [size]" << std::endl;
    return 1;
  }

  gf::FontManager font_manager;
  gf::Font font(argv[1], &font_manager);

  std::cout << "family: " << font.family_name() << '\n';
  std::cout << "style: " << font.style_name() << '\n';

  char c = 'A';

  if (argc > 2) {
    c = argv[2][0];
  }

  unsigned character_size = 32;

  if (argc > 3) {
    character_size = static_cast<unsigned>(std::atoi(argv[3]));
  }

  const auto glyph = font.create_glyph(c, character_size);
  const auto size = glyph.bitmap.size();

  constexpr char Gray[] = " .:-=+*#%@";
  const auto GrayLength = std::strlen(Gray);

  for (auto position : gf::position_range(size)) {
    auto gray = glyph.bitmap(position);
    std::size_t index = static_cast<std::size_t>(static_cast<double>(gray) / 256.0 * GrayLength);

    assert(index < GrayLength);
    std::cout << Gray[index];

    if (position.x + 1 == size.w) {
      std::cout << '\n';
    }
  }

  return 0;
}
