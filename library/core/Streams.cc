// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Streams.h>

#include <cassert>

#include <algorithm>
#include <string>
#include <utility>

#include <gf2/core/Log.h>

namespace gf {

  /*
   * FileInputStream
   */

  FileInputStream::FileInputStream(const std::filesystem::path& path)
  {
    if (!std::filesystem::is_regular_file(path)) {
      Log::error("Could not find the following file for streaming: '{}'", path.string());
      return;
    }

    m_file = std::fopen(path.string().c_str(), "rb");

    if (m_file == nullptr) {
      Log::error("Could not open the following file for streaming: '{}'", path.string());
    }
  }

  FileInputStream::FileInputStream(FileInputStream&& other) noexcept
  : m_file(std::exchange(other.m_file, nullptr))
  {
  }

  FileInputStream::~FileInputStream()
  {
    if (m_file != nullptr) {
      std::fclose(m_file);
    }
  }

  FileInputStream& FileInputStream::operator=(FileInputStream&& other) noexcept
  {
    std::swap(m_file, other.m_file);
    return *this;
  }

  std::size_t FileInputStream::read(Span<uint8_t> buffer)
  {
    if (m_file == nullptr) {
      return 0;
    }

    return std::fread(buffer.data(), sizeof(uint8_t), buffer.size(), m_file);
  }

  void FileInputStream::seek(std::ptrdiff_t position)
  {
    if (m_file == nullptr) {
      return;
    }

    std::fseek(m_file, static_cast<long>(position), SEEK_SET);
  }

  void FileInputStream::skip(std::ptrdiff_t position)
  {
    if (m_file == nullptr) {
      return;
    }

    std::fseek(m_file, static_cast<long>(position), SEEK_CUR);
  }

  bool FileInputStream::finished()
  {
    if (m_file == nullptr) {
      return true;
    }

    return std::feof(m_file) != 0;
  }

  /*
   * MemoryInputStream
   */

  MemoryInputStream::MemoryInputStream(Span<const uint8_t> memory)
  : m_memory(memory)
  {
  }

  std::size_t MemoryInputStream::read(Span<uint8_t> buffer)
  {
    if (buffer.empty()) {
      return 0;
    }

    const std::size_t available = m_memory.size() - m_offset;
    const std::size_t count = std::min(buffer.size(), available);

    if (count > 0) {
      std::copy_n(m_memory.data() + m_offset, count, buffer.data());
      m_offset += count;
    }

    return count;
  }

  void MemoryInputStream::seek(std::ptrdiff_t position)
  {
    if (position < 0) {
      return;
    }

    auto offset = static_cast<std::size_t>(position);
    m_offset = std::min(offset, m_memory.size());
  }

  void MemoryInputStream::skip(std::ptrdiff_t position)
  {
    if (position < 0) {
      auto offset = static_cast<std::size_t>(-position);
      m_offset = offset < m_offset ? m_offset - offset : 0;
    } else {
      auto offset = static_cast<std::size_t>(position);
      m_offset = std::min(m_offset + offset, m_memory.size());
    }
  }

  bool MemoryInputStream::finished()
  {
    return m_offset == m_memory.size();
  }

  /*
   * CompressedInputStream
   */

  CompressedInputStream::CompressedInputStream(InputStream* compressed)
  : m_compressed(compressed)
  , m_stream()
  , m_buffer()
  {
    assert(compressed != nullptr);
    m_stream.zalloc = nullptr;
    m_stream.zfree = nullptr;
    [[maybe_unused]] const int err = inflateInit(&m_stream);
    assert(err == Z_OK); // throw?
  }

  CompressedInputStream::CompressedInputStream(CompressedInputStream&& other) noexcept
  : m_compressed(other.m_compressed)
  , m_stream()
  , m_start(other.m_start)
  , m_stop(other.m_stop)
  , m_eof(std::exchange(other.m_eof, true))
  , m_buffer(other.m_buffer)
  {
    [[maybe_unused]] const int err_copy = inflateCopy(&m_stream, &other.m_stream);
    assert(err_copy == Z_OK);
    [[maybe_unused]] const int err_reset = inflateReset(&other.m_stream);
    assert(err_reset == Z_OK);
  }

  CompressedInputStream::~CompressedInputStream()
  {
    [[maybe_unused]] const int err = inflateEnd(&m_stream);
    assert(err == Z_OK);
  }

  CompressedInputStream& CompressedInputStream::operator=(CompressedInputStream&& other) noexcept
  {
    m_compressed = other.m_compressed;

    [[maybe_unused]] const int err_end = inflateEnd(&m_stream);
    assert(err_end == Z_OK);
    [[maybe_unused]] const int err_copy = inflateCopy(&m_stream, &other.m_stream);
    assert(err_copy == Z_OK);
    [[maybe_unused]] const int err_reset = inflateReset(&other.m_stream);
    assert(err_reset == Z_OK);

    m_start = other.m_start;
    m_stop = other.m_stop;
    m_eof = std::exchange(other.m_eof, true);
    m_buffer = other.m_buffer;
    return *this;
  }

  std::size_t CompressedInputStream::read(Span<uint8_t> buffer)
  {
    if (m_eof) {
      return 0;
    }

    m_stream.next_out = buffer.data();
    m_stream.avail_out = static_cast<uInt>(buffer.size());

    while (m_stream.avail_out > 0) {
      if (m_start == m_stop) {
        m_start = 0;
        m_stop = static_cast<uInt>(m_compressed->read(m_buffer));
      }

      const uInt remaining = m_stop - m_start;

      m_stream.next_in = m_buffer.data() + m_start;
      m_stream.avail_in = remaining;
      const int err = inflate(&m_stream, Z_NO_FLUSH);

      if (err != Z_OK && err != Z_STREAM_END && err != Z_BUF_ERROR) {
        Log::debug("Error while calling inflate: {} '{}'\n", err, m_stream.msg);
      }

      if (err == Z_STREAM_END) {
        m_eof = true;
        return buffer.size() - m_stream.avail_out;
      }

      assert(err == Z_OK || err == Z_STREAM_END || err == Z_BUF_ERROR);
      m_start += remaining - m_stream.avail_in;
    }

    return buffer.size();
  }

  void CompressedInputStream::seek([[maybe_unused]] std::ptrdiff_t position)
  {
    Log::fatal("Not supported");
  }

  void CompressedInputStream::skip([[maybe_unused]] std::ptrdiff_t position)
  {
    Log::fatal("Not supported");
  }

  bool CompressedInputStream::finished()
  {
    return m_eof;
  }

  /*
   * BufferInputStream
   */

  BufferInputStream::BufferInputStream(const std::vector<uint8_t>* bytes)
  : m_bytes(bytes)
  {
    assert(bytes != nullptr);
  }

  std::size_t BufferInputStream::read(Span<uint8_t> buffer)
  {
    if (buffer.empty()) {
      return 0;
    }

    const std::size_t available = m_bytes->size() - m_offset;
    const std::size_t count = std::min(buffer.size(), available);

    if (count > 0) {
      std::copy_n(m_bytes->data() + m_offset, count, buffer.data());
      m_offset += count;
    }

    return count;
  }

  void BufferInputStream::seek(std::ptrdiff_t position)
  {
    if (position < 0) {
      return;
    }

    auto offset = static_cast<std::size_t>(position);
    m_offset = std::min(offset, m_bytes->size());
  }

  void BufferInputStream::skip(std::ptrdiff_t position)
  {
    if (position < 0) {
      auto offset = static_cast<std::size_t>(-position);
      m_offset = offset < m_offset ? m_offset - offset : 0;
    } else {
      auto offset = static_cast<std::size_t>(position);
      m_offset = std::min(m_offset + offset, m_bytes->size());
    }
  }

  bool BufferInputStream::finished()
  {
    return m_offset == m_bytes->size();
  }

  /*
   * HashedInputStream
   */

  HashedInputStream::HashedInputStream(InputStream* stream)
  : m_stream(stream)
  {
  }

  SecureHash::Hash HashedInputStream::hash()
  {
    return m_hash.result();
  }

  std::size_t HashedInputStream::read(Span<uint8_t> buffer)
  {
    auto result = m_stream->read(buffer);
    m_hash.input(buffer);
    return result;
  }

  void HashedInputStream::seek([[maybe_unused]] std::ptrdiff_t position)
  {
    Log::fatal("Not supported");
  }

  void HashedInputStream::skip([[maybe_unused]] std::ptrdiff_t position)
  {
    Log::fatal("Not supported");
  }

  bool HashedInputStream::finished()
  {
    return m_stream->finished();
  }

  /*
   * FileOutputStream
   */

  namespace {
    const char* to_string(FileOutputStream::Mode mode)
    {
      switch (mode) {
        case FileOutputStream::Mode::Write:
          return "wb";
        case FileOutputStream::Mode::Append:
          return "ab";
      }

      assert(false);
      return "";
    }
  } // namespace

  FileOutputStream::FileOutputStream(const std::filesystem::path& path, Mode mode)
  {
    m_file = std::fopen(path.string().c_str(), to_string(mode));

    if (m_file == nullptr) {
      Log::error("Could not open the following file for streaming: '{}'\n", path.string());
    }
  }

  FileOutputStream::FileOutputStream(FileOutputStream&& other) noexcept
  : m_file(std::exchange(other.m_file, nullptr))
  {
  }

  FileOutputStream::~FileOutputStream()
  {
    if (m_file != nullptr) {
      std::fclose(m_file);
    }
  }

  FileOutputStream& FileOutputStream::operator=(FileOutputStream&& other) noexcept
  {
    std::swap(m_file, other.m_file);
    std::swap(m_written, other.m_written);
    return *this;
  }

  std::size_t FileOutputStream::write(Span<const uint8_t> buffer)
  {
    if (m_file == nullptr) {
      return 0;
    }

    if (buffer.empty()) {
      return 0;
    }

    m_written += buffer.size();

    return std::fwrite(buffer.data(), sizeof(uint8_t), buffer.size(), m_file);
  }

  std::size_t FileOutputStream::written_bytes() const
  {
    return m_written;
  }

  /*
   * MemoryOutputStream
   */

  MemoryOutputStream::MemoryOutputStream(Span<uint8_t> memory)
  : m_memory(memory)
  {
  }

  std::size_t MemoryOutputStream::write(Span<const uint8_t> buffer)
  {
    const std::size_t remaining = m_memory.size() - m_offset;
    const std::size_t size = std::min(remaining, buffer.size());
    std::copy_n(buffer.data(), size, m_memory.data() + m_offset);
    m_offset += size;
    return size;
  }

  std::size_t MemoryOutputStream::written_bytes() const
  {
    return m_offset;
  }

  /*
   * CompressedOutputStream
   */

  CompressedOutputStream::CompressedOutputStream(OutputStream* compressed)
  : m_compressed(compressed)
  , m_stream()
  , m_buffer()
  {
    assert(compressed != nullptr);
    m_stream.zalloc = nullptr;
    m_stream.zfree = nullptr;
    [[maybe_unused]] const int err = deflateInit(&m_stream, Z_DEFAULT_COMPRESSION);
    assert(err == Z_OK);
  }

  CompressedOutputStream::CompressedOutputStream(CompressedOutputStream&& other) noexcept
  : m_compressed(other.m_compressed)
  , m_stream()
  , m_buffer(other.m_buffer)
  {
    [[maybe_unused]] const int err_copy = deflateCopy(&m_stream, &other.m_stream);
    assert(err_copy == Z_OK);
    [[maybe_unused]] const int err_reset = deflateReset(&other.m_stream);
    assert(err_reset == Z_OK);
  }

  CompressedOutputStream::~CompressedOutputStream()
  {
    const uInt buffer_size = static_cast<uInt>(m_buffer.size());

    m_stream.next_in = nullptr;
    m_stream.avail_in = 0;
    m_stream.next_out = m_buffer.data();
    m_stream.avail_out = buffer_size;

    for (;;) {
      const int err = deflate(&m_stream, Z_FINISH);
      assert(err == Z_OK || err == Z_STREAM_END);
      const uInt written = buffer_size - m_stream.avail_out;

      if (written > 0) {
        [[maybe_unused]] const std::size_t flushed = m_compressed->write(gf::span(m_buffer.data(), written));
        assert(flushed == written);

        m_stream.next_out = m_buffer.data();
        m_stream.avail_out = buffer_size;
      }

      if (err == Z_STREAM_END) {
        break;
      }
    }

    [[maybe_unused]] const int err = deflateEnd(&m_stream);
    assert(err == Z_OK);
  }

  CompressedOutputStream& CompressedOutputStream::operator=(CompressedOutputStream&& other) noexcept
  {
    m_compressed = other.m_compressed;

    [[maybe_unused]] const int err_end = deflateEnd(&m_stream);
    assert(err_end == Z_OK);
    [[maybe_unused]] const int err_copy = deflateCopy(&m_stream, &other.m_stream);
    assert(err_copy == Z_OK);
    [[maybe_unused]] const int err_reset = deflateReset(&other.m_stream);
    assert(err_reset == Z_OK);

    m_buffer = other.m_buffer;
    return *this;
  }

  std::size_t CompressedOutputStream::write(Span<const uint8_t> buffer)
  {
    const uInt buffer_size = static_cast<uInt>(m_buffer.size());

    m_stream.next_in = buffer.data();
    m_stream.avail_in = static_cast<uInt>(buffer.size());
    m_stream.next_out = m_buffer.data();
    m_stream.avail_out = buffer_size;

    while (m_stream.avail_in > 0) {
      [[maybe_unused]] const int err = deflate(&m_stream, Z_NO_FLUSH);
      assert(err == Z_OK);
      const uInt written = buffer_size - m_stream.avail_out;

      if (written > 0) {
        [[maybe_unused]] const std::size_t flushed = m_compressed->write(gf::span(m_buffer.data(), written));
        assert(flushed == written);

        m_stream.next_out = m_buffer.data();
        m_stream.avail_out = buffer_size;
      } else {
        assert(m_stream.avail_in == 0);
      }
    }

    return buffer.size();
  }

  std::size_t CompressedOutputStream::written_bytes() const
  {
    return m_compressed->written_bytes();
  }

  /*
   * BufferOutputStream
   */

  BufferOutputStream::BufferOutputStream(std::vector<uint8_t>* bytes)
  : m_bytes(bytes)
  {
    assert(bytes != nullptr);
  }

  std::size_t BufferOutputStream::write(Span<const uint8_t> buffer)
  {
    std::copy_n(buffer.data(), buffer.size(), std::back_inserter(*m_bytes));
    return buffer.size();
  }

  std::size_t BufferOutputStream::written_bytes() const
  {
    return m_bytes->size();
  }

  /*
   * HashedOutputStream
   */

  HashedOutputStream::HashedOutputStream(OutputStream* stream)
  : m_stream(stream)
  {
  }

  SecureHash::Hash HashedOutputStream::hash()
  {
    return m_hash.result();
  }

  std::size_t HashedOutputStream::write(Span<const uint8_t> buffer)
  {
    m_hash.input(buffer);
    return m_stream->write(buffer);
  }

  std::size_t HashedOutputStream::written_bytes() const
  {
    return m_stream->written_bytes();
  }

} // namespace gf
