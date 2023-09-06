#include <gf2/audio/AudioManager.h>

#include "gtest/gtest.h"

TEST(AudioManagerTest, Constructor) {
  EXPECT_NO_THROW({
    gf::AudioManager audio;
  });
}
