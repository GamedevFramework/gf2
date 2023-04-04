// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Log.h>
// clang-format on

#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <ctime>

#include <chrono>
#include <utility>

namespace gf {

  namespace {

    const char* to_string(Log::Level level) {
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

  Log::Level Log::s_level = Log::Debug;

  void Log::set_level(Level level) {
    s_level = level;
  }

  void Log::log(Level level, const char *fmt, va_list ap) {
    if (level < s_level) {
      return;
    }

    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    std::time_t integer_part = seconds.count();

    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(epoch) % std::chrono::seconds(1);
    int64_t fractional_part = microseconds.count();

    static constexpr std::size_t BufferSize = 1024;
    char buffer[BufferSize];
    std::size_t size = std::strftime(buffer, BufferSize, "%F %T", std::localtime(&integer_part));
    std::snprintf(buffer + size, BufferSize - size, ".%06" PRIi64, fractional_part);

    std::fprintf(stderr, "[%s][%s] ", buffer, to_string(level));
    std::vfprintf(stderr, fmt, ap);
  }

}
