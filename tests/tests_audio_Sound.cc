#include <thread>

#include <gf2/AudioManager.h>
#include <gf2/Sound.h>

#include "config.h"

#include "gtest/gtest.h"

TEST(SoundTest, Constructor) {
  gf::AudioManager audio;

  EXPECT_NO_THROW({
    gf::Sound sound(GF_TEST_ASSETS_DIRECTORY "/mining.ogg", { gf::ref(audio) });
  });
}

TEST(SoundTest, StartStop) {
  using namespace std::chrono_literals;

  gf::AudioManager audio;
  gf::Sound sound(GF_TEST_ASSETS_DIRECTORY "/mining.ogg", { gf::ref(audio) });

  sound.start();
  sound.stop();
}
