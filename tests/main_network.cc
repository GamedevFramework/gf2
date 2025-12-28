// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#include <gf2/core/Log.h>

#include <gf2/network/NetworkInitializer.h>

#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
  gf::Log::set_level(gf::LogLevel::Fatal);

  [[maybe_unused]] const gf::NetworkInitializer network_initializer;

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
