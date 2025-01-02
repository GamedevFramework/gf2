// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_TEXT_LEXER_H
#define GF_TEXT_LEXER_H

#include <cstdint>

#include <string_view>
#include <variant>

#include "CoreApi.h"

namespace gf {

  enum class TextTokenType : uint8_t {
    Word,
    Space,
    Newline,
    StartTag,
    EndTag,
    End,
  };

  struct GF_CORE_API TextTag {
    std::string_view tag;
    std::string_view value;
  };

  struct GF_CORE_API TextToken {
    static TextToken make_word(std::string_view word)
    {
      return { TextTokenType::Word, word };
    }

    static TextToken make_space()
    {
      return { TextTokenType::Space, std::monostate() };
    }

    static TextToken make_newline()
    {
      return { TextTokenType::Newline, std::monostate() };
    }

    static TextToken make_start_tag(std::string_view tag, std::string_view value)
    {
      TextTag data = { tag, value };
      return { TextTokenType::StartTag, data };
    }

    static TextToken make_end_tag()
    {
      return { TextTokenType::EndTag, std::monostate() };
    }

    static TextToken make_end()
    {
      return { TextTokenType::End, std::monostate() };
    }

    TextTokenType type;
    std::variant<std::monostate, std::string_view, TextTag> data;
  };

  class GF_CORE_API TextLexer {
  public:
    TextLexer() = default;
    TextLexer(const TextLexer&) = default;
    TextLexer(TextLexer&&) noexcept = default;
    virtual ~TextLexer() = default;

    TextLexer& operator=(const TextLexer&) = default;
    TextLexer& operator=(TextLexer&&) noexcept = default;

    void initialize(std::string_view string);
    virtual TextToken next() = 0;

  protected:
    bool at_end() const;
    char peek() const;
    char advance();
    bool match(char expected);
    std::string_view source() const;
    const char* current() const;
    std::size_t current_index() const;

  private:
    std::string_view m_source;
    const char* m_current = nullptr;
  };

  class GF_CORE_API SimpleTextLexer : public TextLexer {
  public:
    TextToken next() override;
  };

  class GF_CORE_API RichTextLexer : public TextLexer {
  public:
    TextToken next() override;
  };

}

#endif // GF_TEXT_LEXER_H
