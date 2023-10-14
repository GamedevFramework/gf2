#ifndef GF_RANGE_H
#define GF_RANGE_H

#include <cstddef>

#include <iterator>
#include <utility>

#include "Vec2.h"

namespace gf {

  template<typename T>
  struct Range {
    T lo; // The lower endpoint (included)
    T hi; // The higher endpoint (excluded)

    struct Iterator {
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::bidirectional_iterator_tag;

      T index;

      constexpr Iterator(T index) noexcept
      : index(index)
      {
      }

      void swap(Iterator& other) noexcept
      {
        using std::swap;
        swap(index, other.index);
      }

      reference operator*() noexcept { return index; }
      pointer operator->() noexcept { return index; }

      Iterator& operator++() noexcept
      {
        ++index;
        return *this;
      }

      Iterator operator++(int) noexcept
      {
        Iterator copy = *this;
        ++index;
        return copy;
      }

      Iterator& operator--() noexcept
      {
        --index;
        return *this;
      }

      Iterator operator--(int) noexcept
      {
        Iterator copy = *this;
        --index;
        return copy;
      }

      constexpr bool operator!=(const Iterator& other) const noexcept { return index != other.index; }
      constexpr bool operator==(const Iterator& other) const noexcept { return index == other.index; }
    };

    constexpr bool contains(T value) const noexcept
    {
      return lo <= value && value < hi;
    }

    constexpr Iterator begin() const noexcept
    {
      return Iterator(lo);
    }

    constexpr Iterator end() const noexcept
    {
      return Iterator(hi);
    }

    constexpr T size() const noexcept
    {
      return hi - lo;
    }

    constexpr bool empty() const noexcept
    {
      return lo >= hi;
    }

    constexpr bool valid() const noexcept
    {
      return lo <= hi;
    }
  };

  template<typename T>
  inline void swap(typename Range<T>::Iterator& lhs, typename Range<T>::Iterator& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  template<typename T>
  constexpr Range<T> range(T lo, T hi)
  {
    return { lo, hi };
  }

  using IndexRange = Range<std::size_t>;

  constexpr IndexRange index_range(std::size_t size)
  {
    return { static_cast<std::size_t>(0), size };
  }

  template<typename T>
  struct Range2D {
    Range<T> dx;
    Range<T> dy;

    struct Iterator {
      using difference_type = std::ptrdiff_t;
      using value_type = Vec2<T>;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::forward_iterator_tag;

      Range<T> dx;
      Vec2<T> position;

      constexpr Iterator(Range<T> dx, Vec2<T> position) noexcept
      : dx(dx)
      , position(position)
      {
      }

      void swap(Iterator& other) noexcept
      {
        using std::swap;
        swap(dx, other.dx);
        swap(position, other.position);
      }

      reference operator*() noexcept { return position; }
      pointer operator->() noexcept { return position; }

      Iterator& operator++() noexcept
      {
        step();
        return *this;
      }

      Iterator operator++(int) noexcept
      {
        Iterator copy = *this;
        step();
        return copy;
      }

      constexpr bool operator!=(const Iterator& other) const noexcept { return position.x != other.position.x || position.y != other.position.y; }
      constexpr bool operator==(const Iterator& other) const noexcept { return position.x == other.position.x && position.y == other.position.y; }

    private:
      void step() noexcept
      {
        ++position.x;

        if (position.x >= dx.hi) {
          position.x = dx.lo;
          ++position.y;
        }
      }
    };

    constexpr Iterator begin() const noexcept
    {
      return Iterator(dx, { dx.lo, dy.lo });
    }

    constexpr Iterator end() const noexcept
    {
      return Iterator(dx, { dx.lo, dy.hi });
    }
  };

  template<typename T>
  inline void swap(typename Range2D<T>::Iterator& lhs, typename Range2D<T>::Iterator& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  using PositionRange = Range2D<int>;

  constexpr PositionRange position_range(Vec2I size)
  {
    return { range(0, size.x), range(0, size.y) };
  }

  template<typename IteratorType, typename SentinelType>
  struct Enumerate {
    IteratorType begin_iterator;
    SentinelType end_iterator;

    struct Iterator {
      size_t index;
      IteratorType iterator;

      using difference_type = std::ptrdiff_t;
      using value_type = std::pair<std::size_t, decltype(*std::declval<IteratorType>())>;
      using iterator_category = std::forward_iterator_tag;
      using pointer = value_type;
      using reference = value_type;

      void operator++()
      {
        ++index;
        ++iterator;
      }

      pointer operator->() const { return std::make_pair(index, *iterator); }
      reference operator*() const { return std::make_pair(index, *iterator); }

      bool operator!=(const Iterator& other) const { return iterator != other.iterator; }
      bool operator==(const Iterator& other) const { return iterator == other.iterator; }

      bool operator!=(const SentinelType& other) const { return iterator != other; }
      bool operator==(const SentinelType& other) const { return iterator == other; }
    };

    auto begin() { return Iterator{ 0, begin_iterator }; }
    auto end() { return end_iterator; }
  };

  template<typename T>
  constexpr auto enumerate(T& iterable)
  {
    using std::begin;
    using IteratorType = decltype(begin(std::declval<T>()));

    using std::end;
    using SentinelType = decltype(end(std::declval<T>()));

    return Enumerate<IteratorType, SentinelType>(begin(iterable), end(iterable));
  }

  template<typename T>
  constexpr auto enumerate(const T& iterable)
  {
    using std::begin;
    using IteratorType = decltype(begin(std::declval<T>()));

    using std::end;
    using SentinelType = decltype(end(std::declval<T>()));

    return Enumerate<IteratorType, SentinelType>(begin(iterable), end(iterable));
  }

} // namespace gf

#endif // GF_RANGE_H
