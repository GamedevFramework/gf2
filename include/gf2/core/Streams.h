// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_STREAMS_H
#define GF_STREAMS_H

#include <cstddef>
#include <cstdio>

#include <array>
#include <filesystem>

#include <zlib.h>

#include "CoreApi.h"
#include "Stream.h"

namespace gf {

  class GF_CORE_API FileInputStream : public InputStream {
  public:
    explicit FileInputStream(const std::filesystem::path& path);
    FileInputStream(const FileInputStream&) = delete;
    FileInputStream(FileInputStream&& other) noexcept;

    ~FileInputStream() override;

    FileInputStream& operator=(const FileInputStream&) = delete;
    FileInputStream& operator=(FileInputStream&& other) noexcept;

    std::size_t read(Span<uint8_t> buffer) override;
    void seek(std::ptrdiff_t position) override;
    void skip(std::ptrdiff_t position) override;
    bool finished() override;

  private:
    std::FILE* m_file = nullptr;
  };

  class GF_CORE_API MemoryInputStream : public InputStream {
  public:
    explicit MemoryInputStream(Span<const uint8_t> memory);

    std::size_t read(Span<uint8_t> buffer) override;
    void seek(std::ptrdiff_t position) override;
    void skip(std::ptrdiff_t position) override;
    bool finished() override;

  private:
    Span<const uint8_t> m_memory;
    std::size_t m_offset = 0;
  };

  class GF_CORE_API CompressedInputStream : public InputStream {
  public:
    explicit CompressedInputStream(InputStream* compressed);
    CompressedInputStream(const CompressedInputStream&) = delete;
    CompressedInputStream(CompressedInputStream&& other) noexcept;
    ~CompressedInputStream() override;

    CompressedInputStream& operator=(const CompressedInputStream&) = delete;
    CompressedInputStream& operator=(CompressedInputStream&& other) noexcept;

    std::size_t read(Span<uint8_t> buffer) override;
    void seek(std::ptrdiff_t position) override;
    void skip(std::ptrdiff_t position) override;
    bool finished() override;

  private:
    InputStream* m_compressed;
    z_stream m_stream;
    uInt m_start = 0;
    uInt m_stop = 0;
    bool m_eof = false;
    std::array<Bytef, 256> m_buffer;
  };

  class GF_CORE_API BufferInputStream : public InputStream {
  public:
    explicit BufferInputStream(std::vector<uint8_t>* bytes);

    std::vector<uint8_t>& bytes()
    {
      return *m_bytes;
    }

    std::size_t read(Span<uint8_t> buffer) override;
    void seek(std::ptrdiff_t position) override;
    void skip(std::ptrdiff_t position) override;
    bool finished() override;

  private:
    std::vector<uint8_t>* m_bytes;
    std::size_t m_offset = 0;
  };

  class GF_CORE_API FileOutputStream : public OutputStream {
  public:
    enum class Mode : uint8_t {
      Write,
      Append,
    };

    explicit FileOutputStream(const std::filesystem::path& path, Mode mode = Mode::Write);
    FileOutputStream(const FileOutputStream&) = delete;
    FileOutputStream(FileOutputStream&& other) noexcept;
    ~FileOutputStream() override;

    FileOutputStream& operator=(const FileOutputStream&) = delete;
    FileOutputStream& operator=(FileOutputStream&& other) noexcept;

    std::size_t write(Span<const uint8_t> buffer) override;
    std::size_t written_bytes() const override;

  private:
    std::FILE* m_file = nullptr;
    std::size_t m_written = 0;
  };

  class GF_CORE_API MemoryOutputStream : public OutputStream {
  public:
    explicit MemoryOutputStream(Span<uint8_t> memory);

    std::size_t write(Span<const uint8_t> buffer) override;
    std::size_t written_bytes() const override;

  private:
    Span<uint8_t> m_memory;
    std::size_t m_offset = 0;
  };

  class GF_CORE_API CompressedOutputStream : public OutputStream {
  public:
    explicit CompressedOutputStream(OutputStream* compressed);
    CompressedOutputStream(const CompressedOutputStream&) = delete;
    CompressedOutputStream(CompressedOutputStream&& other) noexcept;
    ~CompressedOutputStream() override;

    CompressedOutputStream& operator=(const CompressedOutputStream&) = delete;
    CompressedOutputStream& operator=(CompressedOutputStream&& other) noexcept;

    std::size_t write(Span<const uint8_t> buffer) override;
    std::size_t written_bytes() const override;

  private:
    OutputStream* m_compressed;
    z_stream m_stream;
    std::array<Bytef, 256> m_buffer;
  };

  class GF_CORE_API BufferOutputStream : public OutputStream {
  public:
    explicit BufferOutputStream(std::vector<uint8_t>* bytes);

    std::size_t write(Span<const uint8_t> buffer) override;
    std::size_t written_bytes() const override;

    std::vector<uint8_t>& bytes()
    {
      return *m_bytes;
    }

  private:
    std::vector<uint8_t>* m_bytes;
  };

} // namespace gf

#endif // GF_STREAMS_H
