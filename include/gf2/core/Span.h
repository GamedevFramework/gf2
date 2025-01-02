// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_SPAN_H
#define GF_SPAN_H

#include <cassert>
#include <cstdint>

#include <array>
#include <vector>

namespace gf {

  template<typename T>
  class Span;

  template<typename T, std::size_t N>
  class StaticSpan {
  public:
    static constexpr std::size_t Size = N;

    constexpr StaticSpan() noexcept
    : m_data(nullptr)
    {
    }

    constexpr StaticSpan(std::nullptr_t) noexcept
    : m_data(nullptr)
    {
    }

    constexpr StaticSpan(T* data, [[maybe_unused]] std::size_t size) noexcept
    : m_data(data)
    {
      assert(size == Size);
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
    constexpr StaticSpan(U (&data)[N]) noexcept
    : m_data(std::begin(data))
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
    constexpr StaticSpan(std::array<U, N>& values) noexcept
    : m_data(values.data())
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
    constexpr StaticSpan(StaticSpan<U, N> other) noexcept
    : m_data(other.data())
    {
    }

    constexpr const T* data() const noexcept
    {
      return m_data;
    }

    constexpr T* data() noexcept
    {
      return m_data;
    }

    constexpr std::size_t size() const noexcept
    {
      return Size;
    }

    constexpr bool empty() const noexcept
    {
      return m_data == nullptr || Size == 0;
    }

    constexpr const T* begin() const noexcept
    {
      return m_data;
    }

    constexpr T* begin() noexcept
    {
      return m_data;
    }

    constexpr const T* end() const noexcept
    {
      return m_data == nullptr ? nullptr : m_data + Size;
    }

    constexpr T* end() noexcept
    {
      return m_data == nullptr ? nullptr : m_data + Size;
    }

    constexpr const T& operator[](std::size_t index) const noexcept
    {
      assert(m_data != nullptr && index < Size);
      return m_data[index];
    }

    constexpr T& operator[](std::size_t index) noexcept
    {
      assert(m_data != nullptr && index < Size);
      return m_data[index];
    }

    constexpr const T& front() const noexcept
    {
      assert(m_data != nullptr && Size > 0);
      return m_data[0];
    }

    constexpr T& front() noexcept
    {
      assert(m_data != nullptr && Size > 0);
      return m_data[0];
    }

    constexpr const T& back() const noexcept
    {
      assert(m_data != nullptr && Size > 0);
      return m_data[Size - 1];
    }

    constexpr T& back() noexcept
    {
      assert(m_data != nullptr && Size > 0);
      return m_data[Size - 1];
    }

    template<std::size_t I>
    constexpr const T& get() const noexcept
    {
      static_assert(I < N, "Out of bounds.");
      assert(m_data != nullptr);
      return m_data[I];
    }

    template<std::size_t I>
    constexpr T& get() noexcept
    {
      static_assert(I < N, "Out of bounds.");
      assert(m_data != nullptr);
      return m_data[I];
    }

    constexpr Span<T> slice(std::size_t b, std::size_t e) const noexcept
    {
      assert(b <= e && e <= Size);
      return Span<T>(m_data + b, e - b);
    }

    template<std::size_t B, std::size_t E>
    constexpr StaticSpan<T, E - B> slice() const noexcept
    {
      static_assert(B <= E && E <= Size, "Out of bounds.");
      return StaticSpan<T, E - B>(m_data + B);
    }

    constexpr Span<T> first(std::size_t count) const noexcept
    {
      return slice(0, count);
    }

    template<std::size_t C>
    constexpr StaticSpan<T, C> first() const noexcept
    {
      static_assert(C <= Size, "Out of bounds.");
      return slice<0, C>();
    }

    constexpr Span<T> last(std::size_t count) const noexcept
    {
      assert(count <= Size);
      return slice(Size - count, Size);
    }

    template<std::size_t C>
    constexpr StaticSpan<T, C> last() const noexcept
    {
      static_assert(C <= Size, "Out of bounds.");
      return slice<Size - C, Size>();
    }

    constexpr Span<T> first_except(std::size_t count) const noexcept
    {
      assert(count <= Size);
      return slice(0, Size - count);
    }

    template<std::size_t C>
    constexpr StaticSpan<T, Size - C> first_except() const noexcept
    {
      static_assert(C <= Size, "Out of bounds.");
      return slice<0, Size - C>();
    }

    constexpr Span<T> last_except(std::size_t count) const noexcept
    {
      return slice(count, Size);
    }

    template<std::size_t C>
    constexpr StaticSpan<T, Size - C> last_except() const noexcept
    {
      static_assert(C <= Size, "Out of bounds.");
      return slice<C, Size>();
    }

  private:
    T* m_data;
  };

  template<typename T>
  class Span {
  public:
    constexpr Span()
    : m_data(nullptr)
    , m_size(0)
    {
    }

    constexpr Span(std::nullptr_t)
    : m_data(nullptr)
    , m_size(0)
    {
    }

    constexpr Span(T* data, std::size_t size)
    : m_data(data)
    , m_size(size)
    {
    }

    template<typename U, std::size_t N, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
    constexpr Span(U (&data)[N])
    : m_data(std::begin(data))
    , m_size(N)
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
    Span(std::vector<U>& values)
    : m_data(values.data())
    , m_size(values.size())
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]> && std::is_const_v<T>>>
    Span(const std::vector<U>& values)
    : m_data(values.data())
    , m_size(values.size())
    {
    }

    template<typename U, std::size_t N, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
    Span(std::array<U, N>& values)
    : m_data(values.data())
    , m_size(values.size())
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
    constexpr Span(Span<U> other)
    : m_data(other.data())
    , m_size(other.size())
    {
    }

    template<typename U, std::size_t N, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
    constexpr Span(StaticSpan<U, N> other)
    : m_data(other.data())
    , m_size(other.size())
    {
    }

    constexpr const T* data() const noexcept
    {
      return m_data;
    }

    constexpr T* data() noexcept
    {
      return m_data;
    }

    constexpr std::size_t size() const noexcept
    {
      return m_size;
    }

    constexpr bool empty() const noexcept
    {
      return m_size == 0;
    }

    constexpr const T* begin() const noexcept
    {
      return m_data;
    }

    constexpr T* begin() noexcept
    {
      return m_data;
    }

    constexpr const T* end() const noexcept
    {
      return m_data + m_size;
    }

    constexpr T* end() noexcept
    {
      return m_data + m_size;
    }

    constexpr const T& operator[](std::size_t index) const
    {
      return m_data[index];
    }

    constexpr T& operator[](std::size_t index)
    {
      return m_data[index];
    }

    constexpr const T& front() const noexcept
    {
      assert(m_data != nullptr && m_size > 0);
      return m_data[0];
    }

    constexpr T& front() noexcept
    {
      assert(m_data != nullptr && m_size > 0);
      return m_data[0];
    }

    constexpr const T& back() const noexcept
    {
      assert(m_data != nullptr && m_size > 0);
      return m_data[m_size - 1];
    }

    constexpr T& back() noexcept
    {
      assert(m_data != nullptr && m_size > 0);
      return m_data[m_size - 1];
    }

    constexpr Span<T> slice(std::size_t b, std::size_t e) const noexcept
    {
      assert(b <= e && e <= m_size);
      return Span<T>(m_data + b, e - b);
    }

    template<std::size_t B, std::size_t E>
    constexpr StaticSpan<T, E - B> slice() const noexcept
    {
      static_assert(B <= E, "Out of bounds.");
      assert(E <= m_size);
      return StaticSpan<T, E - B>(m_data + B);
    }

    constexpr Span<T> first(std::size_t count) const noexcept
    {
      return slice(0, count);
    }

    template<std::size_t C>
    constexpr StaticSpan<T, C> first() const noexcept
    {
      assert(C <= m_size);
      return StaticSpan<T, C>(m_data, C);
    }

    constexpr Span<T> last(std::size_t count) const noexcept
    {
      assert(count <= m_size);
      return slice(m_size - count, m_size);
    }

    template<std::size_t C>
    constexpr StaticSpan<T, C> last() const noexcept
    {
      assert(C <= m_size);
      return StaticSpan<T, C>(m_data + m_size - C, C);
    }

    constexpr Span<T> first_except(std::size_t count) const noexcept
    {
      assert(count <= m_size);
      return slice(0, m_size - count);
    }

    template<std::size_t C>
    constexpr Span<T> first_except() const noexcept
    {
      assert(C <= m_size);
      return slice(0, m_size - C);
    }

    constexpr Span<T> last_except(std::size_t count) const noexcept
    {
      return slice(count, m_size);
    }

    template<std::size_t C>
    constexpr Span<T> last_except() const noexcept
    {
      assert(C <= m_size);
      return slice(C, m_size);
    }

  private:
    T* m_data;
    std::size_t m_size;
  };

  template<typename T>
  constexpr Span<T> span(T* data, std::size_t size)
  {
    return Span<T>(data, size);
  }

  template<typename T, std::size_t N>
  constexpr StaticSpan<T, N> span(T (&data)[N])
  {
    return StaticSpan<T, N>(data);
  }

  template<typename T>
  constexpr Span<uint8_t> bytes(T* object)
  {
    return span(reinterpret_cast<uint8_t*>(object), sizeof(T)); // NOLINT
  }

} // namespace gf

#endif // GF_SPAN_H
