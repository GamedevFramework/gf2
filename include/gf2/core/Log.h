// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_LOG_H
#define GF_LOG_H

#include <cstdint>
#include <cstdlib>

#include <stdexcept>

#include <fmt/format.h>

#include "CoreApi.h"

namespace gf {

  enum class LogLevel : uint8_t {
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
  };

  class GF_CORE_API Log {
  public:
    Log() = delete;

    static void set_level(LogLevel level);

    template<typename... T>
    static void debug(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(LogLevel::Debug, string);
    }

    template<typename... T>
    static void info(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(LogLevel::Info, string);
    }

    template<typename... T>
    static void warning(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(LogLevel::Warn, string);
    }

    template<typename... T>
    static void error(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(LogLevel::Error, string);
    }

    template<typename... T>
    [[noreturn]] static void fatal(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(LogLevel::Fatal, string);
      throw std::runtime_error(string);
    }

    template<typename... T>
    static void print(LogLevel level, fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(level, string);
    }

  private:
    static void log(LogLevel level, const std::string& string);
  };

} // namespace gf

#endif // GF_LOG_H
