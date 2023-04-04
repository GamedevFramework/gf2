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

    static void debug(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Debug, fmt, ap);
      va_end(ap);
    }

    static void info(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Info, fmt, ap);
      va_end(ap);
    }

    static void warning(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Warn, fmt, ap);
      va_end(ap);
    }

    static void error(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Error, fmt, ap);
      va_end(ap);
    }

    static void fatal(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Fatal, fmt, ap);
      va_end(ap);

      std::abort();
    }

  private:
    static Level s_level;
    static void log(Level level, const char *fmt, va_list ap);
  };

}

#endif // GF_LOG_H
