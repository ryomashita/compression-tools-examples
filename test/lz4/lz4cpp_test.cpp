#include <gtest/gtest.h>

#include "lz4_api.hpp"

class Lz4TestF : public ::testing::Test {
protected:
  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  void TearDown() override {
    // Code here will be called immediately after the constructor (right
    // before the destructor (right before each test).
  }

public:
  // Objects declared here can be used by all tests in the test suite.
  const std::string input =
      "this is a string that I want to compress into a smaller\n"
      "string. Just to make sure there is enough data in the\n"
      "compression buffer, I'm going to fill this string with a\n"
      "decent amount of content. Let's hope this works.\n";
  lz4::buffer_t to_bytes(std::string const &str) {
    lz4::buffer_t bytes{};
    bytes.reserve(str.size());

    for (auto const &c : str) {
      bytes.push_back(c);
    }
    return bytes;
  }
  lz4::string_t to_string(lz4::buffer_t const &bytes) {
    lz4::string_t str{};
    str.reserve(bytes.size());
    for (auto const &byte : bytes) {
      str.push_back(byte);
    }
    return str;
  }
};

TEST_F(Lz4TestF, BlockRoundTrip) {
  // Compress and Decompress using value semantics
  constexpr auto compress_level = 3;
  lz4::buffer_t src = to_bytes(input);

  lz4::buffer_t compressed;
  lz4::buffer_t decompressed;
  lz4::compress(src, compressed, compress_level);
  lz4::decompress(compressed, decompressed, src.size());
  lz4::string_t decompressed_str = to_string(decompressed);

  EXPECT_EQ(src, decompressed);
  EXPECT_EQ(input, decompressed_str);
}