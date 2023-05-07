// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_STREAM_H
#define GF_STREAM_H

#include <cstddef>
#include <cstdint>

#include "CoreApi.h"
#include "Span.h"

namespace gf {

  class GF_CORE_API InputStream {
  public:
    InputStream() = default;
    InputStream(const InputStream&) = delete;
    InputStream(InputStream&&) noexcept = default;
    virtual ~InputStream();

    InputStream& operator=(const InputStream&) = delete;
    InputStream& operator=(InputStream&&) noexcept = default;

    virtual std::size_t read(Span<uint8_t> buffer) = 0;
    virtual void seek(std::ptrdiff_t position) = 0;
    virtual void skip(std::ptrdiff_t position) = 0;
    virtual bool finished() = 0;

    std::size_t read(uint8_t& byte)
    {
      return read(Span<uint8_t>(&byte, 1));
    }
  };

  class GF_CORE_API OutputStream {
  public:
    OutputStream() = default;
    OutputStream(const OutputStream&) = delete;
    OutputStream(OutputStream&&) noexcept = default;
    virtual ~OutputStream();

    OutputStream& operator=(const OutputStream&) = delete;
    OutputStream& operator=(OutputStream&&) noexcept = default;

    virtual std::size_t write(Span<const uint8_t> buffer) = 0;
    virtual std::size_t written_bytes() const = 0;

    std::size_t write(uint8_t byte)
    {
      return write(Span<const uint8_t>(&byte, 1));
    }
  };

} // namespace gf

#endif // GF_STREAM_H
