#include <iostream>
#include <string>

#include <gf2/AudioManager.h>
#include <gf2/Sound.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "gf2_play_sound <sound>" << std::endl;
    return 1;
  }

  gf::AudioManager audio_manager;
  gf::Sound sound(argv[1], &audio_manager);
  sound.start();

  std::cout << "Press enter to stop...\n";
  std::string s;
  std::getline(std::cin, s);

  sound.stop();
  return 0;
}
