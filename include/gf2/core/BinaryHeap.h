// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_BINARY_HEAP_H
#define GF_BINARY_HEAP_H

#include <cassert>

#include <limits>
#include <vector>

namespace gf {

  namespace details {

    template<typename T>
    struct LessCompare {
      constexpr bool operator()(const T& lhs, const T& rhs) const
      {
        return lhs < rhs;
      }
    };

  }

  template<typename T, typename Compare = details::LessCompare<T>>
  class BinaryHeap {
  public:
    using value_compare = Compare;
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    struct Handle {
      size_type index = std::numeric_limits<size_type>::max();
    };

    using handle_type = Handle;

    BinaryHeap() = default;
    BinaryHeap(const value_compare& compare)
    : m_compare(compare)
    {
    }

    reference operator()(handle_type handle)
    {
      return m_elements[handle.index].element;
    }

    const_reference operator()(handle_type handle) const
    {
      return m_elements[handle.index].element;
    }

    const_reference top() const
    {
      assert(!empty());
      assert(m_heap[0] < m_elements.size());
      return element(0);
    }

    bool empty() const
    {
      return m_size == 0;
    }

    size_type size() const
    {
      return m_size;
    }

    handle_type push(const value_type& value)
    {
      size_type index = 0;

      if (has_available_index()) {
        index = available_index();
        ElementIndex& ei = m_elements[index];
        ei.element = value;
        ei.heap_index = m_size;
      } else {
        index = m_size;
        m_elements.push_back({ value, index });
        m_heap.push_back(index);
      }

      ++m_size;
      assert(check_index(m_size - 1));
      siftup(m_size - 1);
      return { index };
    }

    handle_type push(value_type&& value)
    {
      size_type index = 0;

      if (has_available_index()) {
        index = available_index();
        ElementIndex& ei = m_elements[index];
        ei.element = std::move(value);
        ei.heap_index = m_size;
      } else {
        index = m_size;
        m_elements.push_back({ std::move(value), index });
        m_heap.push_back(index);
      }

      ++m_size;
      assert(check_index(m_size - 1));
      siftup(m_size - 1);
      return { index };
    }

    void pop()
    {
      assert(m_size > 0);
      element(0) = value_type();

      if (m_size > 1) {
        swap(0, m_size - 1);
        --m_size;
        siftdown(0);
      } else {
        --m_size;
      }
    }

    void increase(handle_type handle)
    {
      siftup(m_elements[handle.index].heap_index);
    }

    void decrease(handle_type handle)
    {
      siftdown(m_elements[handle.index].heap_index);
    }

    void clear()
    {
      m_size = 0;
      m_elements.clear();
      m_heap.clear();
    }

  private:
    struct ElementIndex {
      T element;
      std::size_t heap_index;
    };

    static constexpr size_type parent_index(size_type index)
    {
      return (index - 1) / 2;
    }

    static constexpr size_type left_child_index(size_type index)
    {
      return (2 * index) + 1;
    }

    static constexpr size_type right_child_index(size_type index)
    {
      return (2 * index) + 2;
    }

    bool leaf(size_type index) const
    {
      return left_child_index(index) >= m_size;
    }

    ElementIndex& element_index(size_type index)
    {
      assert(index < m_size);
      return m_elements[m_heap[index]];
    }

    const ElementIndex& element_index(size_type index) const
    {
      assert(index < m_size);
      return m_elements[m_heap[index]];
    }

    reference element(size_type index)
    {
      return element_index(index).element;
    }

    const_reference element(size_type index) const
    {
      return element_index(index).element;
    }

    bool compare(size_type index0, size_type index1)
    {
      return m_compare(element(index0), element(index1));
    }

    void swap(size_type index0, size_type index1)
    {
      ElementIndex& e0 = element_index(index0);
      ElementIndex& e1 = element_index(index1);

      std::swap(e0.heap_index, e1.heap_index);
      std::swap(m_heap[index0], m_heap[index1]);

      assert(check_index(index0));
      assert(check_index(index1));
    }

    void siftup(size_type index)
    {
      assert(index < m_elements.size());

      while (index != 0) {
        const size_type parent = parent_index(index);

        if (compare(parent, index)) {
          swap(index, parent);
          index = parent;
        } else {
          return;
        }
      }
    }

    void siftdown(size_type index)
    {
      while (!leaf(index)) {
        const size_type child = top_child(index);

        if (!compare(child, index)) {
          swap(index, child);
          index = child;
        } else {
          return;
        }
      }
    }

    size_type top_child(size_type index)
    {
      const size_type left_child = left_child_index(index);
      assert(left_child < m_size);

      const size_type right_child = right_child_index(index);

      if (right_child >= m_size || !compare(left_child, right_child)) {
        return left_child;
      }

      return right_child;
    }

    bool has_available_index() const
    {
      return m_size < m_heap.size();
    }

    size_type available_index()
    {
      return m_heap[m_size];
    }

    bool check_index(size_type index) const
    {
      if (index >= m_size) {
        return false;
      }

      if (m_elements[m_heap[index]].heap_index != index) {
        return false;
      }

      return true;
    }

    value_compare m_compare;
    std::size_t m_size = 0;
    std::vector<ElementIndex> m_elements;
    std::vector<size_type> m_heap;
  };

}

#endif // GF_BINARY_HEAP_H
