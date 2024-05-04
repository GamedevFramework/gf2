// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_LOG_H
#define GF_LOG_H

#include <cstdint>
#include <cstdlib>

#include <stdexcept>

#include <fmt/core.h>

#include "CoreApi.h"

namespace gf {

  class GF_CORE_API Log {
  public:
    Log() = delete;

    enum Level : uint8_t {
      Debug,
      Info,
      Warn,
      Error,
      Fatal,
    };

    static void set_level(Level level);

    template<typename... T>
    static void debug(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(Level::Debug, string);
    }

    template<typename... T>
    static void info(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(Level::Info, string);
    }

    template<typename... T>
    static void warning(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(Level::Warn, string);
    }

    template<typename... T>
    static void error(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(Level::Error, string);
    }

    template<typename... T>
    [[noreturn]] static void fatal(fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(Level::Fatal, string);
      throw std::runtime_error(string);
    }

    template<typename... T>
    static void print(Level level, fmt::format_string<T...> fmt, T&&... args)
    {
      auto string = fmt::format(fmt, std::forward<T>(args)...);
      log(level, string);
    }

  private:
    static void log(Level level, const std::string& string);
  };

} // namespace gf

#endif // GF_LOG_H
