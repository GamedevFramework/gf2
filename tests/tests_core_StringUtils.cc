#include <iterator>

#include <gf2/StringUtils.h>

#include "gtest/gtest.h"

TEST(StringUtilsTest, NiceNum) {
  EXPECT_EQ(gf::nice_num(0, 1.0f), "0");
  EXPECT_EQ(gf::nice_num(0, 0.0001f), "0");

  EXPECT_EQ(gf::nice_num(3.14, 10.0f), "0");
  EXPECT_EQ(gf::nice_num(3.14, 1.0f), "3");
  EXPECT_EQ(gf::nice_num(3.14, 0.1f), "3.1");
  EXPECT_EQ(gf::nice_num(3.14, 0.01f), "3.14");
  EXPECT_EQ(gf::nice_num(3.14, 0.001f), "3.140");

  EXPECT_EQ(gf::nice_num(-3.14, 10.0f), "0");
  EXPECT_EQ(gf::nice_num(-3.14, 1.0f), "-3");
  EXPECT_EQ(gf::nice_num(-3.14, 0.1f), "-3.1");
  EXPECT_EQ(gf::nice_num(-3.14, 0.01f), "-3.14");
  EXPECT_EQ(gf::nice_num(-3.14, 0.001f), "-3.140");
}

TEST(StringUtilsTest, EscapeString) {
  EXPECT_EQ(gf::escape_string("abc"), "abc");
  EXPECT_EQ(gf::escape_string("a\nb\tc"), "a\\nb\\tc");
  EXPECT_EQ(gf::escape_string("a\\b\"c"), "a\\\\b\\\"c");
}

TEST(StringUtilsTest, SplitInParagraphs) {
  auto para0 = gf::split_in_paragraphs("");
  EXPECT_EQ(para0.size(), 0u);

  auto para1 = gf::split_in_paragraphs("\n");
  EXPECT_EQ(para1.size(), 0u);

  auto para2 = gf::split_in_paragraphs("abc");
  EXPECT_EQ(para2.size(), 1u);
  EXPECT_EQ(para2[0], "abc");

  auto para3 = gf::split_in_paragraphs("abc\n");
  EXPECT_EQ(para3.size(), 1u);
  EXPECT_EQ(para3[0], "abc");

  auto para4 = gf::split_in_paragraphs("\nabc");
  EXPECT_EQ(para4.size(), 1u);
  EXPECT_EQ(para4[0], "abc");

  auto para5 = gf::split_in_paragraphs("abc\ndef");
  EXPECT_EQ(para5.size(), 2u);
  EXPECT_EQ(para5[0], "abc");
  EXPECT_EQ(para5[1], "def");

  auto para6 = gf::split_in_paragraphs("abc\n\ndef");
  EXPECT_EQ(para6.size(), 2u);
  EXPECT_EQ(para6[0], "abc");
  EXPECT_EQ(para6[1], "def");
}

TEST(StringUtilsTest, SplitInWords) {
  auto words0 = gf::split_in_words("");
  EXPECT_EQ(words0.size(), 0u);

  auto words1 = gf::split_in_words(" ");
  EXPECT_EQ(words1.size(), 0u);

  auto words2 = gf::split_in_words("\t");
  EXPECT_EQ(words2.size(), 0u);

  auto words3 = gf::split_in_words(" \t");
  EXPECT_EQ(words3.size(), 0u);

  auto words4 = gf::split_in_words("abc");
  EXPECT_EQ(words4.size(), 1u);
  EXPECT_EQ(words4[0], "abc");

  auto words5 = gf::split_in_words("abc ");
  EXPECT_EQ(words5.size(), 1u);
  EXPECT_EQ(words5[0], "abc");

  auto words6 = gf::split_in_words("\tabc");
  EXPECT_EQ(words6.size(), 1u);
  EXPECT_EQ(words6[0], "abc");

  auto words7 = gf::split_in_words("abc def");
  EXPECT_EQ(words7.size(), 2u);
  EXPECT_EQ(words7[0], "abc");
  EXPECT_EQ(words7[1], "def");

  auto words8 = gf::split_in_words("abc \t  \t  def");
  EXPECT_EQ(words8.size(), 2u);
  EXPECT_EQ(words8[0], "abc");
  EXPECT_EQ(words8[1], "def");
}

TEST(StringUtilsTest, Codepoints) {
  {
    auto cp = gf::codepoints("");
    EXPECT_EQ(cp.begin(), cp.end());
    EXPECT_EQ(std::distance(cp.begin(), cp.end()), 0u);
  }

  {
    auto cp = gf::codepoints("abc");
    EXPECT_EQ(std::distance(cp.begin(), cp.end()), 3u);
  }

  {
    auto cp = gf::codepoints("éçù€");
    EXPECT_EQ(std::distance(cp.begin(), cp.end()), 4u);
  }

  {
    auto cp = gf::codepoints("東京");
    EXPECT_EQ(std::distance(cp.begin(), cp.end()), 2u);
    EXPECT_EQ(*cp.begin(), U'東');
  }

  {
    auto cp = gf::codepoints("🎮");
    EXPECT_EQ(std::distance(cp.begin(), cp.end()), 1u);
    EXPECT_EQ(*cp.begin(), U'🎮');
    EXPECT_EQ(*cp.begin(), U'\U0001F3AE');
  }
}
