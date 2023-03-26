#include <gf2/Span.h>

#include "gtest/gtest.h"

/*
 * Span
 */

TEST(SpanTest, DefaultCtor) {
  {
    gf::Span<int> span;

    EXPECT_TRUE(span.empty());
    EXPECT_EQ(0u, span.size());
    EXPECT_EQ(nullptr, span.data());

    for ([[maybe_unused]] auto item : span) {
      FAIL();
    }
  }
  {
    gf::Span<const int> span;

    EXPECT_TRUE(span.empty());
    EXPECT_EQ(0u, span.size());
    EXPECT_EQ(nullptr, span.data());

    for ([[maybe_unused]] auto item : span) {
      FAIL();
    }
  }
  {
    constexpr gf::Span<int> span;

    static_assert(span.empty(), "");
    static_assert(span.size() == 0, "");
    static_assert(span.data() == nullptr, "");
  }
}

TEST(SpanTest, PointerSizeCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::Span<int> span(input, 8);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input, span.data());
  }
  {
    gf::Span<const int> span(input, 8);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input, span.data());
  }
}

TEST(SpanTest, RawArrayCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::Span<int> span(input);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input, span.data());
  }
  {
    gf::Span<const int> span(input);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input, span.data());
  }
}

TEST(SpanTest, StdArrayCtor) {
  std::array<int, 8> input = {{ 1, 2, 3, 4, 5, 6, 7, 8 }};

  {
    gf::Span<int> span(input);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input.data(), span.data());
  }
  {
    gf::Span<const int> span(input);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input.data(), span.data());
  }
}

TEST(SpanTest, CompatibleCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::Span<int> span0(input);
  gf::Span<const int> span1(span0);

  EXPECT_FALSE(span1.empty());
  EXPECT_EQ(8u, span1.size());
  EXPECT_EQ(input, span1.data());
}

TEST(SpanTest, StaticSpanCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::StaticSpan<int, 8> span0(input);
  gf::Span<int> span1(span0);

  EXPECT_FALSE(span1.empty());
  EXPECT_EQ(8u, span1.size());
  EXPECT_EQ(input, span1.data());
}


TEST(SpanTest, Subscript) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::Span<int> span(input);

    EXPECT_EQ(8u, span.size());

    for (std::size_t i = 0; i < span.size(); ++i) {
      EXPECT_EQ(span[i], input[i]);
    }
  }
  {
    gf::Span<const int> span(input);

    EXPECT_EQ(8u, span.size());

    for (std::size_t i = 0; i < span.size(); ++i) {
      EXPECT_EQ(span[i], input[i]);
    }
  }
}

TEST(SpanTest, SpanFunction) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  auto span = gf::span(input, 5);
  EXPECT_FALSE(span.empty());
  EXPECT_EQ(5u, span.size());
  EXPECT_EQ(input, span.data());
  static_assert(std::is_same_v<decltype(span), gf::Span<int>>, "Check Span");
}


/*
 * StaticSpan
 */

TEST(StaticSpanTest, DefaultCtor) {
  {
    gf::StaticSpan<int, 10> span;

    EXPECT_TRUE(span.empty());
    EXPECT_EQ(10u, span.size());
    EXPECT_EQ(nullptr, span.data());

    for ([[maybe_unused]] auto item : span) {
      FAIL();
    }
  }
  {
    gf::StaticSpan<const int, 10> span;

    EXPECT_TRUE(span.empty());
    EXPECT_EQ(10u, span.size());
    EXPECT_EQ(nullptr, span.data());

    for ([[maybe_unused]] auto item : span) {
      FAIL();
    }
  }
}

TEST(StaticSpanTest, PointerSizeCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::StaticSpan<int, 8> span(input, 8);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input, span.data());
  }
  {
    gf::StaticSpan<const int, 8> span(input, 8);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input, span.data());
  }
}

TEST(StaticSpanTest, RawArrayCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::StaticSpan<int, 8> span(input);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input, span.data());
  }
  {
    gf::StaticSpan<const int, 8> span(input);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input, span.data());
  }
}

TEST(StaticSpanTest, StdArrayCtor) {
  std::array<int, 8> input = {{ 1, 2, 3, 4, 5, 6, 7, 8 }};

  {
    gf::StaticSpan<int, 8> span(input);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input.data(), span.data());
  }
  {
    gf::StaticSpan<const int, 8> span(input);

    EXPECT_FALSE(span.empty());
    EXPECT_EQ(8u, span.size());
    EXPECT_EQ(input.data(), span.data());
  }
}

TEST(StaticSpanTest, CompatibleCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::StaticSpan<int, 8> span0(input);
  gf::StaticSpan<const int, 8> span1(span0);

  EXPECT_FALSE(span1.empty());
  EXPECT_EQ(8u, span1.size());
  EXPECT_EQ(input, span1.data());
}

TEST(StaticSpanTest, Subscript) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::StaticSpan<int, 8> span(input);

    EXPECT_EQ(8u, span.size());

    for (std::size_t i = 0; i < span.size(); ++i) {
      EXPECT_EQ(span[i], input[i]);
    }
  }
  {
    gf::StaticSpan<const int, 8> span(input);

    EXPECT_EQ(8u, span.size());

    for (std::size_t i = 0; i < span.size(); ++i) {
      EXPECT_EQ(span[i], input[i]);
    }
  }
}

TEST(StaticSpanTest, Get) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::StaticSpan<int, 8> span(input);

    EXPECT_EQ(8u, span.size());

    EXPECT_EQ(span.get<0>(), input[0]);
    EXPECT_EQ(span.get<1>(), input[1]);
    EXPECT_EQ(span.get<2>(), input[2]);
    EXPECT_EQ(span.get<3>(), input[3]);
    EXPECT_EQ(span.get<4>(), input[4]);
    EXPECT_EQ(span.get<5>(), input[5]);
    EXPECT_EQ(span.get<6>(), input[6]);
    EXPECT_EQ(span.get<7>(), input[7]);
  }
  {
    gf::StaticSpan<const int, 8> span(input);

    EXPECT_EQ(8u, span.size());

    EXPECT_EQ(span.get<0>(), input[0]);
    EXPECT_EQ(span.get<1>(), input[1]);
    EXPECT_EQ(span.get<2>(), input[2]);
    EXPECT_EQ(span.get<3>(), input[3]);
    EXPECT_EQ(span.get<4>(), input[4]);
    EXPECT_EQ(span.get<5>(), input[5]);
    EXPECT_EQ(span.get<6>(), input[6]);
    EXPECT_EQ(span.get<7>(), input[7]);
  }
}

TEST(StaticSpanTest, SpanFunction) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  auto span = gf::span(input);
  EXPECT_FALSE(span.empty());
  EXPECT_EQ(8u, span.size());
  EXPECT_EQ(input, span.data());
  static_assert(std::is_same_v<decltype(span), gf::StaticSpan<int, 8>>, "Check Span");
}
