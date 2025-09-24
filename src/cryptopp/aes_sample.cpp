#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rijndael.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "utility.hpp"

// From: https://www.cryptopp.com/wiki/Advanced_Encryption_Standard
// The following program shows how to operate AES in CBC mode using a pipeline.
// using ./aes_sample [input_file_path]
int main(int argc, const char **argv) {
  using namespace CryptoPP;

  AutoSeededRandomPool prng;

#if 0
  // The key is declared on the stack using a SecByteBlock to ensure the
  // sensitive material is zeroized.
  SecByteBlock key(AES::DEFAULT_KEYLENGTH);
  prng.GenerateBlock(key, key.size());
#else
  // 固定値を key に使う例.
  // [WARN] あくまで ASCIIコード値で解釈される.
  // 16進数表記のバイト列を入力したい場合は別途変換が必要.
  std::string static_key = "BAF7D2A2B1EAF3BE64AA64C3A0938E06";
  if (static_key.size() < AES::MAX_KEYLENGTH)
    throw std::runtime_error("static key too short");
  SecByteBlock key((const byte *)&static_key[0], AES::MAX_KEYLENGTH);
#endif

  SecByteBlock iv(AES::BLOCKSIZE);
  prng.GenerateBlock(iv, iv.size());
  // SecByteBloc -> std::vector conversion
  const char *iv_cstr = reinterpret_cast<const char *>(&iv[0]);
  std::vector iv_vec(iv_cstr, iv_cstr + iv.size());
  if (iv_vec.size() != AES::BLOCKSIZE)
    throw std::runtime_error("iv size incorrect");

  auto tp_input_start = utils::now();

  // road input
  std::vector<byte> plain;
  if (argc > 1) {
    // コマンドライン引数がある場合は、argv[1] をファイル名として読み込む.
    std::string filename = argv[1];
    std::ifstream ifs(filename);
    if (!ifs) {
      utils::PrintLn("failed to open file: {}", filename);
      return 1;
    }
    plain = std::vector<byte>((std::istreambuf_iterator<char>(ifs)),
                              std::istreambuf_iterator<char>());
    utils::PrintLn("read {:L} bytes from {}", plain.size(), filename);
  } else {
    std::string plain_txt = "CBC Mode Test for pipeline processing";
    plain = std::vector<byte>(plain_txt.begin(), plain_txt.end());
  }

  std::vector<byte> cipher, recovered; // ciphertext and recovered text

  auto tp_input_end = utils::now();
  utils::PrintElapsed(tp_input_start, tp_input_end, "input", plain.size());
  /*********************************\
  \*********************************/
  auto tp_start_encryption = utils::now();

  // Crypto++ では Source -> Filter -> Sink のパイプラインで処理を行う。 (Unix
  // pipe を目指している) Source, Sink には以下の種類がある:
  // - StringSource, StringSink : std::string
  // - ArraySource, ArraySink : byte* + size (実装は String~ と同じ)
  // - VectorSource, VectorSink : std::vector<byte>
  // - FileSource, FileSink : std::ifstream or char* filename (open ifstream
  // internally)
  // - SocketSource, SocketSink : socket_t (platform dependent)
  // - WindowPipeSource, WindowPipeSink
  // - RandomNumberSource

  // encryption
  try {
    CBC_Mode<AES>::Encryption e;
    e.SetKeyWithIV(key, key.size(), iv);

#if 1
    // Same as: `StringSource s(plain, ...` (see definition of StringSource)
    ArraySource s((const byte *)plain.data(), (size_t)plain.size(), true,
                  new StreamTransformationFilter(
                      e,
                      new VectorSink(cipher)) // StreamTransformationFilter
    );                                        // StringSource
#else
    // 一度に処理する代わりに、Put() を使い手動でデータを流し込むこともできる。
    StreamTransformationFilter encryptor(e, new StringSink(cipher));
    const size_t CHUNK_SIZE = 16;
    for (size_t i = 0; i < plain.size(); i += CHUNK_SIZE) {
      size_t len = std::min((size_t)CHUNK_SIZE, plain.size() - i);
      encryptor.Put((const byte *)&plain[i], len);
    }
    encryptor.MessageEnd();
#endif
  } catch (const Exception &e) {
    utils::PrintError(e);
    exit(1);
  }

  auto tp_end_encryption = utils::now();
  utils::PrintElapsed(tp_start_encryption, tp_end_encryption,
                      "encryption", plain.size());
  /*********************************\
  \*********************************/

  // display key, iv, cipher

  utils::PrintLn("key length (default): {}", (uint32_t)AES::DEFAULT_KEYLENGTH);
  utils::PrintLn("key length (min): {}", (uint32_t)AES::MIN_KEYLENGTH);
  utils::PrintLn("key length (max): {}", (uint32_t)AES::MAX_KEYLENGTH);
  utils::PrintLn("block size: {}", (uint32_t)AES::BLOCKSIZE);
  utils::PrintLn("key size [bytes]: {}", key.size());

#if 0
  // HexEcoder を使うと, 16進数表記で Sink に出力できる.
  HexEncoder encoder(new FileSink(std::cout));
  std::cout << "key: ";
  encoder.Put(key, key.size());
  encoder.MessageEnd();
  std::cout << std::endl;
#else
  utils::Print("key: ");
  utils::PrintHexArray((const std::byte *)key.data(), key.size());
#endif

  utils::Print("iv size [bytes]: {}\n", iv_vec.size());
  utils::Print("iv: ");
  utils::PrintHexArray((const std::byte *)iv_vec.data(), iv_vec.size());

  utils::PrintLn("plane size [bytes]: {}", plain.size());
  utils::PrintLn("cipher size [bytes]: {}", cipher.size());
#if 0
  std::cout << "cipher text: ";
  encoder.Put((const byte *)&cipher[0], cipher.size());
  encoder.MessageEnd();
  std::cout << std::endl;
#endif

  /*********************************\
  \*********************************/
  auto tp_start_decryption = utils::now();

  // decryption
  try {
    CBC_Mode<AES>::Decryption d;
    d.SetKeyWithIV(key, key.size(), iv);

    VectorSource s(cipher, true,
                   new StreamTransformationFilter(
                       d,
                       new VectorSink(recovered)) // StreamTransformationFilter
    );                                            // StringSource

    // std::cout << "recovered text: " << recovered << std::endl;
  } catch (const Exception &e) {
    utils::PrintError(e);
    exit(1);
  }

  auto tp_end_decryption = utils::now();
  utils::PrintElapsed(tp_start_decryption, tp_end_decryption,
                      "decryption", cipher.size());

  /*********************************\
  \*********************************/
  /* Verify the result */
  bool success = plain == recovered;
  utils::PrintLn("AES CBC mode encryption and decryption: {}",
                 success ? "successful" : "failed");

  return 0;
}
