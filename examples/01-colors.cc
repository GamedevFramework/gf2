// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <string_view>
#include <tuple>

#include <gf2/core/Color.h>
#include <gf2/core/Image.h>
#include <gf2/core/Vec2.h>

int main()
{

  constexpr gf::Vec2I Size = { 64, 64 };

  constexpr std::tuple<std::string_view, gf::Color> Colors[] = {
    { "black", gf::Black },
    { "white", gf::White },
    { "gray", gf::Gray },

    { "red", gf::Red },
    { "vermilion", gf::Vermilion },
    { "orange", gf::Orange },
    { "amber", gf::Amber },
    { "yellow", gf::Yellow },
    { "lime", gf::Lime },
    { "chartreuse", gf::Chartreuse },
    { "harlequin", gf::Harlequin },
    { "green", gf::Green },
    { "erin", gf::Erin },
    { "spring", gf::Spring },
    { "aquamarine", gf::Aquamarine },
    { "cyan", gf::Cyan },
    { "capri", gf::Capri },
    { "azure", gf::Azure },
    { "cerulean", gf::Cerulean },
    { "blue", gf::Blue },
    { "indigo", gf::Indigo },
    { "violet", gf::Violet },
    { "purple", gf::Purple },
    { "magenta", gf::Magenta },
    { "cerise", gf::Cerise },
    { "rose", gf::Rose },
    { "crimson", gf::Crimson },
  };


  for (auto [ name, color ] : Colors) {
    std::filesystem::path filename = name;
    filename.concat(".png");

    gf::Image image(Size, color);
    image.save_to_file(filename);
  }

}
