// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

// clang-format off: main header
#include <gf2/Buffer.h>
// clang-format on

#include <utility>

namespace gf {

  Buffer::Buffer(Buffer&& other) noexcept
  : m_allocator(std::exchange(other.m_allocator, nullptr))
  , m_buffer(std::exchange(other.m_buffer, VK_NULL_HANDLE))
  , m_allocation(std::exchange(other.m_allocation, nullptr))
  , m_type(other.m_type)
  , m_usage(other.m_usage)
  {
  }

  Buffer::~Buffer()
  {
    if (m_buffer != VK_NULL_HANDLE) {
      vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
  }

  Buffer& Buffer::operator=(Buffer&& other) noexcept
  {
    std::swap(m_allocator, other.m_allocator);
    std::swap(m_buffer, other.m_buffer);
    std::swap(m_allocation, other.m_allocation);
    std::swap(m_type, other.m_type);
    std::swap(m_usage, other.m_usage);
    return *this;
  }

}
