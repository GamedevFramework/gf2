#include <thread>

#include <gf2/audio/AudioManager.h>
#include <gf2/audio/Sound.h>

#include "config.h"

#include "gtest/gtest.h"

namespace {

  const std::filesystem::path AssetsDirectory = GF_TEST_ASSETS_DIRECTORY;
  const std::filesystem::path OggFile = AssetsDirectory / "mining.ogg";

}

TEST(SoundTest, Constructor) {
  gf::AudioManager audio;

  EXPECT_NO_THROW({
    gf::Sound sound(OggFile, &audio);
  });
}

TEST(SoundTest, StartStop) {
  using namespace std::chrono_literals;

  gf::AudioManager audio;
  gf::Sound sound(OggFile, &audio);

  sound.start();
  sound.stop();
}
