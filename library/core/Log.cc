// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Log.h>
// clang-format on

#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <ctime>

#include <array>
#include <chrono>
#include <utility>

namespace gf {

  namespace {

    const char* to_string(Log::Level level)
    {
      switch (level) {
      case Log::Debug:
        return "Debug";
      case Log::Info:
        return "Info";
      case Log::Warn:
        return "Warn";
      case Log::Error:
        return "Error";
      case Log::Fatal:
        return "Fatal";
      }

      assert(false);
      return "?";
    }

  } // namespace

  Log::Level Log::s_level = Log::Debug; // NOLINT

  void Log::set_level(Level level)
  {
    s_level = level;
  }

  void Log::log(Level level, const char* fmt, va_list ap)
  {
    if (level < s_level) {
      return;
    }

    const auto now = std::chrono::system_clock::now();
    const auto epoch = now.time_since_epoch();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    const std::time_t integer_part = seconds.count();

    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(epoch) % std::chrono::seconds(1);
    const int64_t fractional_part = microseconds.count();

    static constexpr std::size_t BufferSize = 1024;
    std::array<char, BufferSize> buffer = {};

    const std::size_t size = std::strftime(buffer.data(), buffer.size(), "%F %T", std::localtime(&integer_part));
    std::snprintf(buffer.data() + size, buffer.size() - size, ".%06" PRIi64, fractional_part);

    std::fprintf(stderr, "[%s][%s] ", buffer.data(), to_string(level));
    std::vfprintf(stderr, fmt, ap);
  }

} // namespace gf
