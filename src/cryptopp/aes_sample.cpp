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

// From: https://www.cryptopp.com/wiki/Advanced_Encryption_Standard
// The following program shows how to operate AES in CBC mode using a pipeline.
int main() {
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

    
  std::string plain = "CBC Mode Test for pipeline processing";
  std::string cipher, recovered; // ciphertext and recovered text

  std::cout << "plain text: " << plain << std::endl;

  /*********************************\
  \*********************************/

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

#if 0
    // Same as: `StringSource s(plain, ...` (see definition of StringSource)
    ArraySource s((const byte*)plain.data(), (size_t)plain.size(),
                  true,
                   new StreamTransformationFilter(
                       e,
                       new StringSink(cipher)) // StreamTransformationFilter
    );                                         // StringSource
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
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  /*********************************\
  \*********************************/

  std::cout << "key length (default): " << AES::DEFAULT_KEYLENGTH << std::endl;
  std::cout << "key length (min): " << AES::MIN_KEYLENGTH << std::endl;
  std::cout << "key length (max): " << AES::MAX_KEYLENGTH << std::endl;
  std::cout << "block size: " << AES::BLOCKSIZE << std::endl;

  HexEncoder encoder(new FileSink(std::cout)); // Converts given data to base 16
  // display key, iv, and cipher text
  std::cout << "key size [bytes]: " << key.size() << std::endl;
  std::cout << "key: ";
  encoder.Put(key, key.size());
  encoder.MessageEnd();
  std::cout << std::endl;

  std::cout << "iv size [bytes]: " << iv.size() << std::endl;
  std::cout << "iv: ";
  encoder.Put(iv, iv.size());
  encoder.MessageEnd();
  std::cout << std::endl;

  std::cout << "plane size [bytes]: " << plain.size() << std::endl;
  std::cout << "cipher size [bytes]: " << cipher.size() << std::endl;
  std::cout << "cipher text: ";
  encoder.Put((const byte *)&cipher[0], cipher.size());
  encoder.MessageEnd();
  std::cout << std::endl;

  /*********************************\
  \*********************************/

  // decryption
  try {
    CBC_Mode<AES>::Decryption d;
    d.SetKeyWithIV(key, key.size(), iv);

    StringSource s(cipher, true,
                   new StreamTransformationFilter(
                       d,
                       new StringSink(recovered)) // StreamTransformationFilter
    );                                            // StringSource

    std::cout << "recovered text: " << recovered << std::endl;
  } catch (const Exception &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  /* Verify the result */
  if (plain == recovered)
    std::cout << "AES CBC mode encryption and decryption: successful"
              << std::endl;
  else
    std::cout << "AES CBC mode encryption and decryption: failed" << std::endl;

  return 0;
}
