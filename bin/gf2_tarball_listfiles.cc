// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <cstdlib>

#include <iostream>

#include <gf2/core/Tarball.h>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "gf2_tarball_listfiles <tarball>\n";
    return EXIT_FAILURE;
  }

  const gf::Tarball tarball(argv[1]);
  auto entries = tarball.entries();

  for (const auto& entry : entries) {
    std::cout << entry << '\n';
  }

  return EXIT_SUCCESS;
}
