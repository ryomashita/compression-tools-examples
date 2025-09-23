#include <gtest/gtest.h>

#include <filesystem>

#include "zstdpp_helper.hpp"

class ZstdppTestF : public ::testing::Test {
  protected:
    void SetUp() override {
      // Code here will be called immediately after the constructor (right
      // before each test).
    }
  
    void TearDown() override {
      // Code here will be called immediately after each test (right
      // before the destructor (right before each test).
    }
  
  public:
    // Objects declared here can be used by all tests in the test suite.
    const std::string input = "this is a string that I want to compress into a smaller\n"
                  "string. Just to make sure there is enough data in the\n"
                  "compression buffer, I'm going to fill this string with a\n"
                  "decent amount of content. Let's hope this works.\n";
};

TEST_F(ZstdppTestF, BlockRoundTrip) {
  // Compress and Decompress using value semantics
  constexpr auto compress_level = 3;
  buffer_t compressed = zstdpp::compress(input, compress_level);
  buffer_t decompressed = zstdpp::decompress(compressed);
  zstdpp::string_t decompressed_str = zstdpp::utils::to_string(decompressed);
  
  print_compress_result(input, compressed);
  std::cout << "\nDecompressed Data: " << decompressed_str << '\n';
  
  EXPECT_EQ(input, decompressed_str);
}

TEST_F(ZstdppTestF, StreamRoundTrip) {
  // Compress and Decompress using streams
  std::filesystem::path tempPath = std::filesystem::temp_directory_path();
  std::cout << "Using temp path: " << tempPath << '\n';
  std::string 
    infile{tempPath / "input_stream_c.txt"}, 
    outfile{tempPath / "output_stream_c.zip"},
    decomp_outfile{tempPath / "decompressed_stream_c.txt"};

  fs::create_file(infile, zstdpp::utils::to_bytes(input));
    
  zstdpp::stream_compress(infile, outfile);
  zstdpp::stream_decompress(outfile, decomp_outfile);
    
  auto const result_buffer = fs::read_file(outfile);
  print_compress_result(input, result_buffer);
  auto const decomp_buffer = fs::read_file(decomp_outfile);
  zstdpp::string_t decomp_str = zstdpp::utils::to_string(decomp_buffer);
  std::cout << "\nDecompressed Data from file: " << decomp_str << '\n';

  EXPECT_EQ(input, decomp_str);
}
