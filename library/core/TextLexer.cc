// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard

#include <gf2/core/TextLexer.h>

#include <gf2/core/Log.h>

namespace gf {

  namespace {

    bool is_alpha(char c) { return 'a' <= c && c <= 'z'; }

    bool is_space(char c) { return c == ' ' || c == '\t'; }

  }

  /*
   * TextLexer
   */

  void TextLexer::initialize(std::string_view string)
  {
    m_source = string;
    m_current = string.data();
  }


  bool TextLexer::at_end() const
  {
    return m_current == m_source.data() + m_source.size();
  }

  char TextLexer::peek() const
  {
    return *m_current;
  }

  char TextLexer::advance()
  {
    return *m_current++;
  }

  bool TextLexer::match(char expected)
  {
    if (at_end()) {
      return false;
    }

    if (*m_current != expected) {
      return false;
    }

    ++m_current;
    return true;
  }

  std::string_view TextLexer::source() const
  {
    return m_source;
  }

  const char* TextLexer::current() const
  {
    return m_current;
  }

  std::size_t TextLexer::current_index() const
  {
    return m_current - m_source.data();
  }

  /*
   * SimpleTextLexer
   */

  TextToken SimpleTextLexer::next()
  {
    if (at_end()) {
      return TextToken::make_end();
    }

    if (match('\n')) {
      return TextToken::make_newline();
    }

    if (is_space(peek())) {
      while (!at_end() && is_space(peek())) {
        advance();
      }

      return TextToken::make_space();
    }

    const char* word_start = current();

    while (!at_end() && !is_space(peek()) && peek() != '\n' && peek() != '<') {
      advance();
    }

    std::string_view word(word_start, static_cast<std::size_t>(current() - word_start));
    return TextToken::make_word(word);
  }

  /*
   * RichTextLexer
   */

  TextToken RichTextLexer::next()
  {
    if (at_end()) {
      return TextToken::make_end();
    }

    if (match('<')) {
      // is it end tag?
      if (match('/')) {
        if (!match('>')) {
          Log::fatal("Wrong end tag in string '{}' at {}.", source(), current_index());
        }

        return TextToken::make_end_tag();
      }

      const char* tag_start = current();

      while (!at_end() && is_alpha(peek())) {
        advance();
      }

      std::string_view tag(tag_start, static_cast<std::size_t>(current() - tag_start));
      std::string_view value;

      if (match('=')) {
        const char* value_start = current();

        while (!at_end() && peek() != '>') {
          advance();
        }

        value = std::string_view(value_start, static_cast<std::size_t>(current() - value_start));
      }

      if (!match('>')) {
        Log::fatal("Unclosed tag in string '{}' at index {}.", source(), current_index());
      }

      return TextToken::make_start_tag(tag, value);
    }

    if (match('\n')) {
      return TextToken::make_newline();
    }

    if (is_space(peek())) {
      while (!at_end() && is_space(peek())) {
        advance();
      }

      return TextToken::make_space();
    }

    const char* word_start = current();

    while (!at_end() && !is_space(peek()) && peek() != '\n' && peek() != '<') {
      advance();
    }

    std::string_view word(word_start, static_cast<std::size_t>(current() - word_start));
    return TextToken::make_word(word);
  }


}
