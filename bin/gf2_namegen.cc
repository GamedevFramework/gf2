// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <filesystem>
#include <fstream>
#include <iostream>

#include <gf2/core/Namegen.h>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: gf2_namegen <file>\n";
    std::cerr << "You can find word lists here: https://github.com/Tw1ddle/markov-namegen-lib/tree/master/word_lists\n";
    return EXIT_FAILURE;
  }

  const std::filesystem::path filename(argv[1]);
  std::ifstream file(filename);

  std::vector<std::string> data;

  for (std::string line; std::getline(file, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    data.push_back(std::move(line));
  }

  gf::NamegenSettings settings = {};
  settings.min_length = 3;
  settings.max_length = 15;

  gf::Random random;

  const gf::NamegenManager namegen(data, 3, 0.001, true);
  auto names = namegen.generate_multiple(random, 20, gf::seconds(0.5), settings);

  for (const std::string& name : names) {
    std::cout << name << '\n';
  }

  return EXIT_SUCCESS;
}
