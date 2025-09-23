#pragma once

#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rijndael.h>

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace cryptopp {
using byte_t = std::uint8_t;
using buffer_t = std::vector<byte_t>;
using string_t = std::string;
using size_buffer_t = std::size_t;

size_t GetCipherLen(size_t plain_len) {
  using namespace CryptoPP;
  // AES block size is 16 bytes
  const size_t block_size = AES::BLOCKSIZE;
  // Calculate padding length
  size_t padding_len = block_size - (plain_len % block_size);
  // Total cipher length is plain length + padding length
  return plain_len + padding_len;
}

bool AesCbcEncrypt(const buffer_t &key, const buffer_t &iv,
                   const buffer_t &plain, buffer_t &cipher) {
  using namespace CryptoPP;
  try {
    if (key.size() != AES::MAX_KEYLENGTH)
      throw std::runtime_error("key size incorrect");
    if (iv.size() != AES::BLOCKSIZE)
      throw std::runtime_error("iv size incorrect");

    CBC_Mode<AES>::Encryption e;
    e.SetKeyWithIV(key.data(), key.size(), iv.data());

    StringSource s(plain.data(), plain.size(), true,
                   new StreamTransformationFilter(
                       e,
                       new VectorSink(cipher)) // StreamTransformationFilter
    );                                         // StringSource

    return true;
  } catch (const Exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

bool AesCbcDecrypt(const buffer_t &key, const buffer_t &iv,
                   const buffer_t &cipher, buffer_t &plain) {
  using namespace CryptoPP;
  try {
    if (key.size() != AES::MAX_KEYLENGTH)
      throw std::runtime_error("key size incorrect");
    if (iv.size() != AES::BLOCKSIZE)
      throw std::runtime_error("iv size incorrect");

    CBC_Mode<AES>::Decryption d;
    d.SetKeyWithIV(key.data(), key.size(), iv.data());

    StringSource s(cipher.data(), cipher.size(), true,
                   new StreamTransformationFilter(
                       d,
                       new VectorSink(plain)) // StreamTransformationFilter
    );                                        // StringSource

    return true;
  } catch (const Exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}
} // namespace cryptopp
