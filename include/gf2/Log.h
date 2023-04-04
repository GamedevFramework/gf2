// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_LOG_H
#define GF_LOG_H

#include <cstdarg>
#include <cstdint>
#include <cstdlib>

#include "CoreApi.h"
#include "Portability.h"

namespace gf {

  class GF_CORE_API Log {
  public:
    Log() = delete;

    enum Level : uint32_t {
      Debug,
      Info,
      Warn,
      Error,
      Fatal,
    };

    static void set_level(Level level);

    static void debug(const char* fmt, ...) GF_FORMAT(1, 2)
    {
      va_list ap;
      va_start(ap, fmt);          // NOLINT
      log(Level::Debug, fmt, ap); // NOLINT
      va_end(ap);                 // NOLINT
    }

    static void info(const char* fmt, ...) GF_FORMAT(1, 2)
    {
      va_list ap;
      va_start(ap, fmt);         // NOLINT
      log(Level::Info, fmt, ap); // NOLINT
      va_end(ap);                // NOLINT
    }

    static void warning(const char* fmt, ...) GF_FORMAT(1, 2)
    {
      va_list ap;
      va_start(ap, fmt);         // NOLINT
      log(Level::Warn, fmt, ap); // NOLINT
      va_end(ap);                // NOLINT
    }

    static void error(const char* fmt, ...) GF_FORMAT(1, 2)
    {
      va_list ap;
      va_start(ap, fmt);          // NOLINT
      log(Level::Error, fmt, ap); // NOLINT
      va_end(ap);                 // NOLINT
    }

    static void fatal(const char* fmt, ...) GF_FORMAT(1, 2)
    {
      va_list ap;
      va_start(ap, fmt);          // NOLINT
      log(Level::Fatal, fmt, ap); // NOLINT
      va_end(ap);                 // NOLINT

      std::abort();
    }

  private:
    static Level s_level; // NOLINT
    static void log(Level level, const char* fmt, va_list ap);
  };

} // namespace gf

#endif // GF_LOG_H
