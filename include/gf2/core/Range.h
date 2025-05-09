#ifndef GF_RANGE_H
#define GF_RANGE_H

#include <cstddef>

#include <iterator>
#include <tuple>
#include <utility>

#include "Rect.h"
#include "Vec2.h"

namespace gf {

  /*
   * Range
   */

  template<typename T>
  class Range {
  public:
    class Iterator {
    public:
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::bidirectional_iterator_tag;

      constexpr Iterator(T index) noexcept
      : m_index(index)
      {
      }

      void swap(Iterator& other) noexcept
      {
        using std::swap;
        swap(m_index, other.m_index);
      }

      reference operator*() noexcept { return m_index; }
      pointer operator->() noexcept { return m_index; }

      Iterator& operator++() noexcept
      {
        ++m_index;
        return *this;
      }

      Iterator operator++(int) noexcept
      {
        Iterator copy = *this;
        ++m_index;
        return copy;
      }

      Iterator& operator--() noexcept
      {
        --m_index;
        return *this;
      }

      Iterator operator--(int) noexcept
      {
        Iterator copy = *this;
        --m_index;
        return copy;
      }

      constexpr bool operator!=(const Iterator& other) const noexcept { return m_index != other.m_index; }
      constexpr bool operator==(const Iterator& other) const noexcept { return m_index == other.m_index; }

    private:
      T m_index;
    };

    constexpr Range(T lo, T hi) noexcept
    : m_lo(lo)
    , m_hi(hi)
    {
    }

    constexpr T lo() const noexcept
    {
      return m_lo;
    }

    constexpr T hi() const noexcept
    {
      return m_hi;
    }

    constexpr bool contains(T value) const noexcept
    {
      return m_lo <= value && value < m_hi;
    }

    constexpr Iterator begin() const noexcept
    {
      return { m_lo };
    }

    constexpr Iterator end() const noexcept
    {
      return { m_hi };
    }

    constexpr T size() const noexcept
    {
      return m_hi - m_lo;
    }

    constexpr bool empty() const noexcept
    {
      return m_lo >= m_hi;
    }

    constexpr bool valid() const noexcept
    {
      return m_lo <= m_hi;
    }

    constexpr Range skip(T count) const noexcept
    {
      return { details::min(m_lo + count, m_hi), m_hi };
    }

  private:
    T m_lo; // The lower endpoint (included)
    T m_hi; // The higher endpoint (excluded)
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
    return { std::size_t(0), size };
  }

  /*
   * RangeId
   */

  template<typename Id>
  class RangeId {
  public:
    class Iterator {
    public:
      using difference_type = std::ptrdiff_t;
      using value_type = Id;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::bidirectional_iterator_tag;

      constexpr Iterator(std::size_t index)
      : m_index(index)
      {
      }

      constexpr void swap(Iterator& other) noexcept
      {
        std::swap(m_index, other.m_index);
      }

      constexpr reference operator*() noexcept
      {
        return Id{ m_index };
      }

      constexpr pointer operator->() noexcept
      {
        return Id{ m_index };
      }

      constexpr Iterator& operator++() noexcept
      {
        ++m_index;
        return *this;
      }

      constexpr Iterator operator++(int) noexcept
      {
        Iterator copy = *this;
        ++m_index;
        return copy;
      }

      constexpr Iterator& operator--() noexcept
      {
        --m_index;
        return *this;
      }

      constexpr bool operator!=(const Iterator& other) const noexcept
      {
        return m_index != other.m_index;
      }

      constexpr bool operator==(const Iterator& other) const noexcept
      {
        return m_index == other.m_index;
      }

    private:
      std::size_t m_index;
    };

    constexpr RangeId(std::size_t size)
    : m_size(size)
    {
    }

    constexpr Iterator begin() const noexcept
    {
      return Iterator{ 0 };
    }

    constexpr Iterator end() const noexcept
    {
      return Iterator{ m_size };
    }

    constexpr std::size_t size() const noexcept
    {
      return m_size;
    }

  private:
    std::size_t m_size;
  };

  template<typename Id>
  inline void swap(typename RangeId<Id>::Iterator& lhs, typename RangeId<Id>::Iterator& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  /*
   * IteratorRange
   */

  template<typename Iterator>
  class IteratorRange {
  public:
    IteratorRange(Iterator begin, Iterator end)
    : m_begin(begin)
    , m_end(end)
    {
    }

    Iterator begin() const noexcept
    {
      return m_begin;
    }

    Iterator end() const noexcept
    {
      return m_end;
    }

  private:
    Iterator m_begin;
    Iterator m_end;
  };


  /*
   * Range2D
   */

  template<typename T>
  class Range2D {
  public:
    class Iterator {
    public:
      using difference_type = std::ptrdiff_t;
      using value_type = Vec2<T>;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::forward_iterator_tag;

      constexpr Iterator(Range<T> dx, Vec2<T> position) noexcept
      : m_dx(dx)
      , m_position(position)
      {
      }

      void swap(Iterator& other) noexcept
      {
        using std::swap;
        swap(m_dx, other.m_dx);
        swap(m_position, other.m_position);
      }

      reference operator*() noexcept { return m_position; }
      pointer operator->() noexcept { return m_position; }

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

      constexpr bool operator!=(const Iterator& other) const noexcept { return m_position != other.m_position; }
      constexpr bool operator==(const Iterator& other) const noexcept { return m_position == other.m_position; }

    private:
      void step() noexcept
      {
        ++m_position.x;

        if (m_position.x >= m_dx.hi()) {
          m_position.x = m_dx.lo();
          ++m_position.y;
        }
      }

      Range<T> m_dx;
      Vec2<T> m_position;
    };

    constexpr Range2D(Range<T> dx, Range<T> dy) noexcept
    : m_dx(dx)
    , m_dy(dy)
    {
    }

    constexpr Iterator begin() const noexcept
    {
      return Iterator(m_dx, { m_dx.lo(), m_dy.lo() });
    }

    constexpr Iterator end() const noexcept
    {
      return Iterator(m_dx, { m_dx.lo(), m_dy.hi() });
    }

  private:
    Range<T> m_dx;
    Range<T> m_dy;
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

  constexpr PositionRange rectangle_range(RectI rectangle)
  {
    return { range(rectangle.offset.x, rectangle.offset.x + rectangle.extent.w), range(rectangle.offset.y, rectangle.offset.y + rectangle.extent.h) };
  }

  /*
   * NeighborSquareRange
   */

  template<typename T>
  class NeighborSquareRange {
  public:
    class Iterator {
    public:
      using difference_type = std::ptrdiff_t;
      using value_type = Vec2<T>;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::forward_iterator_tag;

      constexpr Iterator(Vec2<T> current, Range<T> dx, Range<T> dy, Vec2<T> origin) noexcept
      : m_current(current)
      , m_dx(dx)
      , m_dy(dy)
      , m_origin(origin)
      {
        if (!valid()) {
          step();
        }
      }

      void swap(Iterator& other) noexcept
      {
        using std::swap;
        swap(m_current, other.m_current);
        swap(m_dx, other.m_dx);
        swap(m_dy, other.m_dy);
        swap(m_origin, other.m_origin);
      }

      reference operator*() noexcept { return m_current; }
      pointer operator->() noexcept { return m_current; }

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

      constexpr bool operator!=(Vec2<T> other) const noexcept { return m_current != other; }
      constexpr bool operator==(Vec2<T> other) const noexcept { return m_current == other; }

    private:
      void step() noexcept
      {
        for (;;) {
          ++m_current.x;

          if (m_current.x >= m_dx.hi()) {
            m_current.x = m_dx.lo();
            ++m_current.y;
          }

          if (valid()) {
            break;
          }
        }
      }

      bool valid() const noexcept
      {
        return m_current != m_origin;
      }

      Vec2<T> m_current;
      Range<T> m_dx;
      Range<T> m_dy;
      Vec2<T> m_origin;
    };

    constexpr NeighborSquareRange(Range<T> dx, Range<T> dy, Vec2<T> origin) noexcept
    : m_dx(dx)
    , m_dy(dy)
    , m_origin(origin)
    {
    }

    constexpr Iterator begin() const
    {
      return {
        { m_dx.lo(), m_dy.lo() },
        m_dx,
        m_dy,
        m_origin
      };
    }

    constexpr Vec2<T> end() const
    {
      return { m_dx.lo(), m_dy.hi() };
    }

  private:
    Range<T> m_dx;
    Range<T> m_dy;
    Vec2<T> m_origin;
  };

  template<typename T>
  constexpr NeighborSquareRange<T> neighbor_square_range(Vec2<T> origin, T radius = T(1)) noexcept
  {
    return { range(origin.x - radius, origin.x + radius + 1), range(origin.y - radius, origin.y + radius + 1), origin };
  }

  template<typename T>
  inline void swap(typename NeighborSquareRange<T>::Iterator& lhs, typename NeighborSquareRange<T>::Iterator& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  /*
   * NeighborDiamondRange
   */

  template<typename T>
  class NeighborDiamondRange {
  public:
    class Iterator {
    public:
      using difference_type = std::ptrdiff_t;
      using value_type = Vec2<T>;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::forward_iterator_tag;

      constexpr Iterator(Vec2<T> current, Range<T> dx, Range<T> dy, Vec2<T> origin, T radius) noexcept
      : m_current(current)
      , m_dx(dx)
      , m_dy(dy)
      , m_origin(origin)
      , m_radius(radius)
      {
        if (!valid()) {
          step();
        }
      }

      void swap(Iterator& other) noexcept
      {
        using std::swap;
        swap(m_current, other.m_current);
        swap(m_dx, other.m_dx);
        swap(m_dy, other.m_dy);
        swap(m_origin, other.m_origin);
        swap(m_radius, other.m_radius);
      }

      reference operator*() noexcept { return m_current; }
      pointer operator->() noexcept { return m_current; }

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

      constexpr bool operator!=(Vec2<T> other) const noexcept { return m_current != other; }
      constexpr bool operator==(Vec2<T> other) const noexcept { return m_current == other; }

    private:
      void step() noexcept
      {
        for (;;) {
          ++m_current.x;

          if (m_current.x >= m_dx.hi()) {
            m_current.x = m_dx.lo();
            ++m_current.y;

            if (m_current.y >= m_dy.hi()) {
              // outside the square
              break;
            }
          }

          if (valid()) {
            break;
          }
        }
      }

      bool valid() noexcept
      {
        return m_current != m_origin && manhattan_distance(m_current, m_origin) <= m_radius;
      }

      Vec2<T> m_current;
      Range<T> m_dx;
      Range<T> m_dy;
      Vec2<T> m_origin;
      T m_radius;
    };

    constexpr NeighborDiamondRange(Range<T> dx, Range<T> dy, Vec2<T> origin, T radius) noexcept
    : m_dx(dx)
    , m_dy(dy)
    , m_origin(origin)
    , m_radius(radius)
    {
    }

    constexpr Iterator begin() const
    {
      return {
        { m_dx.lo(), m_dy.lo() },
        m_dx,
        m_dy,
        m_origin,
        m_radius
      };
    }

    constexpr Vec2<T> end() const
    {
      return { m_dx.lo(), m_dy.hi() };
    }

  private:
    Range<T> m_dx;
    Range<T> m_dy;
    Vec2<T> m_origin;
    T m_radius;
  };

  template<typename T>
  constexpr NeighborDiamondRange<T> neighbor_diamond_range(Vec2<T> origin, T radius = T(1)) noexcept
  {
    return { range(origin.x - radius, origin.x + radius + 1), range(origin.y - radius, origin.y + radius + 1), origin, radius };
  }

  template<typename T>
  inline void swap(typename NeighborDiamondRange<T>::Iterator& lhs, typename NeighborDiamondRange<T>::Iterator& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  /*
   * Enumerate
   */

  template<typename T>
  class Enumerate {
  public:
    using IteratorType = decltype(std::begin(std::declval<T>()));
    using SentinelType = decltype(std::end(std::declval<T>()));

    class Iterator {
    public:
      constexpr Iterator(size_t index, IteratorType iterator) noexcept
      : m_index(index)
      , m_iterator(iterator)
      {
      }

      void operator++()
      {
        ++m_index;
        ++m_iterator;
      }

      auto operator*() { return std::tie(m_index, *m_iterator); }
      bool operator!=(SentinelType other) const { return m_iterator != other; }

    private:
      size_t m_index;
      IteratorType m_iterator;
    };

    Enumerate(T iterable)
    : m_iterable(iterable)
    {
    }

    auto begin() { return Iterator{ 0, std::begin(m_iterable) }; }
    auto end() { return std::end(m_iterable); }

  private:
    T m_iterable;
  };

  template<typename T>
  inline auto enumerate(T&& iterable)
  {
    return Enumerate<T>(std::forward<T>(iterable));
  }

} // namespace gf

#endif // GF_RANGE_H
