// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <cstdlib>

#include <iostream>
#include <string>

#include <gf2/audio/AudioManager.h>
#include <gf2/audio/Sound.h>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: gf2_play_sound <sound>\n";
    return EXIT_FAILURE;
  }

  gf::AudioManager audio_manager;
  gf::Sound sound(argv[1], &audio_manager);
  sound.start();

  std::cout << "Press enter to stop...\n";
  std::string s;
  std::getline(std::cin, s);

  sound.stop();
  return EXIT_SUCCESS;
}
