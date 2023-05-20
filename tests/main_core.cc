// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#include <gf2/Log.h>

#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
  gf::Log::set_level(gf::Log::Fatal);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
