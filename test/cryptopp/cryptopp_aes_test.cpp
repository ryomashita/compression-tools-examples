#include <gtest/gtest.h>

#include "aes_api.hpp"

class CryptoPPTestF : public ::testing::Test {
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
  const cryptopp::string_t input =
      "this is a string that I want to encrypt and decrypt using AES "
      "algorithm in CBC mode. Just to make sure there is enough data in the "
      "encryption buffer, I'm going to fill this string with a decent amount "
      "of content. Let's hope this works.";
  cryptopp::buffer_t to_bytes(std::string const &str) {
    cryptopp::buffer_t bytes{};
    bytes.reserve(str.size());
    for (auto const &c : str) {
      bytes.push_back(c);
    }
    return bytes;
  }
  cryptopp::string_t to_string(cryptopp::buffer_t const &bytes) {
    cryptopp::string_t str{};
    str.reserve(bytes.size());
    for (auto const &byte : bytes) {
      str.push_back(byte);
    }
    return str;
  }
};

TEST_F(CryptoPPTestF, AesCbcRoundTrip) {
  // Encrypt and Decrypt using value semantics
  cryptopp::buffer_t key = to_bytes("BAF7D2A2B1EAF3BE64AA64C3A0938E06");
  cryptopp::buffer_t iv = to_bytes("000102030405060D");
  cryptopp::buffer_t plain = to_bytes(input);

  cryptopp::buffer_t cipher;
  cryptopp::buffer_t recovered;

  ASSERT_EQ(key.size(), CryptoPP::AES::MAX_KEYLENGTH);
  ASSERT_EQ(iv.size(), CryptoPP::AES::BLOCKSIZE);

  ASSERT_TRUE(cryptopp::AesCbcEncrypt(key, iv, plain, cipher));
  ASSERT_EQ(cipher.size(), cryptopp::GetCipherLen(plain.size()));

  ASSERT_TRUE(cryptopp::AesCbcDecrypt(key, iv, cipher, recovered));

  EXPECT_EQ(plain, recovered);
}