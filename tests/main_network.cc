// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include "gtest/gtest.h"

#include <gf2/Log.h>
#include <gf2/NetworkInitializer.h>

int main(int argc, char *argv[]) {
  gf::Log::set_level(gf::Log::Fatal);

  [[maybe_unused]] gf::NetworkInitializer network_initializer;

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
