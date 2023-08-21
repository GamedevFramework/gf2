#include <type_traits>

#include <gf2/BinaryHeap.h>

#include "gtest/gtest.h"

TEST(BinaryHeapTest, DefaultConstructor) {
  gf::BinaryHeap<int> max_heap;

  EXPECT_EQ(max_heap.size(), 0u);
  EXPECT_TRUE(max_heap.empty());

  gf::BinaryHeap<int, std::greater<int>> min_heap;

  EXPECT_EQ(min_heap.size(), 0u);
  EXPECT_TRUE(min_heap.empty());
}

TEST(BinaryHeapTest, Top) {
  constexpr int values[] = { 4, 0, 1, 3, 5, 2 };

  gf::BinaryHeap<int> max_heap;
  gf::BinaryHeap<int, std::greater<int>> min_heap;

  for (auto value : values) {
    max_heap.push(value);
    min_heap.push(value);
  }

  EXPECT_EQ(max_heap.size(), std::size(values));
  EXPECT_FALSE(max_heap.empty());
  EXPECT_EQ(max_heap.top(), 5);

  EXPECT_EQ(min_heap.size(), std::size(values));
  EXPECT_FALSE(min_heap.empty());
  EXPECT_EQ(min_heap.top(), 0);
}

TEST(BinaryHeapTest, Handle) {
  gf::BinaryHeap<int> heap;
  std::vector<gf::BinaryHeap<int>::handle_type> handles;

  constexpr int values[] = { 4, 0, 1, 5, 2 };

  for (auto value : values) {
    handles.push_back(heap.push(value));
  }

  EXPECT_EQ(heap.size(), std::size(values));
  EXPECT_EQ(heap.top(), 5);

  for (std::size_t i = 0; i < std::size(values); ++i) {
    EXPECT_EQ(heap(handles[i]), values[i]);
  }
}

namespace {

  struct Node {
    constexpr Node(int p)
    : priority(p) { }

    int priority;
  };

  bool operator<(const Node& lhs, const Node& rhs)
  {
    return lhs.priority < rhs.priority;
  }

}

TEST(BinaryHeapTest, Increase) {
  gf::BinaryHeap<Node> heap;
  std::vector<gf::BinaryHeap<Node>::handle_type> handles;

  constexpr int values[] = { 4, 0, 1, 5, 2 };

  for (auto value : values) {
    handles.push_back(heap.push({ value }));
  }

  EXPECT_EQ(heap.size(), std::size(values));
  EXPECT_EQ(heap.top().priority, 5);

  heap(handles[2]).priority = 3;
  heap.increase(handles[2]);

  EXPECT_EQ(heap.size(), std::size(values));
  EXPECT_EQ(heap.top().priority, 5);

  heap(handles[2]).priority = 6;
  heap.increase(handles[2]);

  EXPECT_EQ(heap.size(), std::size(values));
  EXPECT_EQ(heap.top().priority, 6);
}

TEST(BinaryHeapTest, Decrease) {
  gf::BinaryHeap<Node> heap;
  std::vector<gf::BinaryHeap<Node>::handle_type> handles;

  constexpr int values[] = { 4, 0, 1, 5, 2 };

  for (auto value : values) {
    handles.push_back(heap.push({ value }));
  }

  EXPECT_EQ(heap.size(), std::size(values));
  EXPECT_EQ(heap.top().priority, 5);

  heap(handles[3]).priority = 3;
  heap.decrease(handles[3]);

  EXPECT_EQ(heap.size(), std::size(values));
  EXPECT_EQ(heap.top().priority, 4);

  heap(handles[3]).priority = -1;
  heap.decrease(handles[3]);

  EXPECT_EQ(heap.size(), std::size(values));
  EXPECT_EQ(heap.top().priority, 4);
}
