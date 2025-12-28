// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Log.h>

#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <ctime>

#include <array>
#include <chrono>

#include <fmt/color.h>

namespace gf {

  namespace {

    LogLevel g_level = LogLevel::Debug; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

    const char* to_string(LogLevel level)
    {
      switch (level) {
        case LogLevel::Debug:
          return "Debug";
        case LogLevel::Info:
          return "Info";
        case LogLevel::Warn:
          return "Warn";
        case LogLevel::Error:
          return "Error";
        case LogLevel::Fatal:
          return "Fatal";
      }

      assert(false);
      return "?";
    }

    fmt::color to_color(LogLevel level)
    {
      switch (level) {
        case LogLevel::Debug:
          return fmt::color::blue;
        case LogLevel::Info:
          return fmt::color::green;
        case LogLevel::Warn:
          return fmt::color::orange;
        case LogLevel::Error:
          return fmt::color::red;
        case LogLevel::Fatal:
          return fmt::color::purple;
      }

      assert(false);
      return fmt::color::yellow;
    }

    void print_timestamp_and_level(LogLevel level)
    {
      const auto now = std::chrono::system_clock::now();
      const auto epoch = now.time_since_epoch();
      const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
      const std::time_t integer_part = seconds.count();

      const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(epoch) % std::chrono::seconds(1);
      const int64_t fractional_part = microseconds.count();

      static constexpr std::size_t BufferSize = 1024;
      std::array<char, BufferSize> buffer = {};

      const std::size_t size = std::strftime(buffer.data(), buffer.size(), "%F %T", std::localtime(&integer_part));
      std::snprintf(buffer.data() + size, buffer.size() - size, ".%06" PRIi64, fractional_part); // NOLINT

      fmt::print(stderr, fmt::emphasis::bold | fmt::fg(to_color(level)), "[{}][{}] ", buffer.data(), to_string(level));
    }

  } // namespace

  void Log::set_level(LogLevel level)
  {
    g_level = level;
  }

  void Log::log(LogLevel level, const std::string& string)
  {
    if (level < g_level) {
      return;
    }

    print_timestamp_and_level(level);
    fmt::print(stderr, "{}\n", string);
  }

} // namespace gf
