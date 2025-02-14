#include <gf2/core/SecureHash.h>

#include <string_view>

#include <fmt/format.h>

#include "gtest/gtest.h"

struct HashTest {
  std::string_view input;
  std::size_t repeat = 0;
  std::string_view expected;
};


TEST(SecureHashTest, RFC4634) {
  constexpr HashTest Tests[] = {
    { "abc", 1, "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD" },
    { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 1, "248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1" },
    { "a", 1000000, "CDC76E5C9914FB9281A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0" },
    { "0123456701234567012345670123456701234567012345670123456701234567", 10, "594847328451BDFA85056225462CC1D867D877FB388DF0CE35F25AB5562BFBB5" },
    { "\x19", 1, "68AA2E2EE5DFF96E3355E6C7EE373E3D6A4E17F75F9518D843709C0C9BC3E3D4" },
    { "\xe3\xd7\x25\x70\xdc\xdd\x78\x7c\xe3\x88\x7a\xb2\xcd\x68\x46\x52", 1, "175EE69B02BA9B58E2B0A5FD13819CEA573F3940A94F825128CF4209BEABB4E8" },
  };

  for (const auto& test : Tests) {
    gf::SecureHash hash;

    gf::Span<const uint8_t> span = gf::span(reinterpret_cast<const uint8_t*>(test.input.data()), test.input.size()); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)

    for (std::size_t i = 0; i < test.repeat; ++i) {
      hash.input(span);
    }

    auto result = hash.result();

    std::string result_string;

    for (auto byte : result) {
      result_string += fmt::format("{:02X}", byte);
    }

    EXPECT_EQ(result_string, test.expected);
  }

}
