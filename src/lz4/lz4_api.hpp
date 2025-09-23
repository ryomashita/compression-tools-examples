#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "lz4.h"

namespace lz4 {
    using byte_t = std::uint8_t;
    using compress_level_t = std::uint8_t;
    using threads_number_t = std::uint8_t;
    using buffer_t = std::vector<byte_t>;
    using string_t = std::string;
    using size_buffer_t = std::size_t;

    inline size_buffer_t compress(
        const buffer_t& src,
        buffer_t& dst,
        compress_level_t compress_level = 3
    ) {
        const int max_dst_size = LZ4_compressBound((int)src.size());
        dst.resize(max_dst_size);

        const int compress_size = LZ4_compress_default(
            (const char*)src.data(), 
            (char*)dst.data(), 
            (int)src.size(), 
            max_dst_size
        );

        if (compress_size <= 0) {
            std::cerr << "LZ4_compress_default() failed with code: " << compress_size << '\n';
            // Compression failed
            dst.clear();
            return 0;
        }

        dst.resize(compress_size);
        dst.shrink_to_fit();
        return dst.size();
    }
    
    inline size_buffer_t decompress(const buffer_t &src, buffer_t& dst, size_buffer_t original_size) {
        // Get the original size from the compressed data
        // Note: In a real-world scenario, you should store the original size separately
        // because LZ4 does not include it in the compressed data.
        // Here we assume the original size is known or fixed for simplicity.
        dst.resize(original_size);

        const int decomp_size = LZ4_decompress_safe(
            (const char*)src.data(), 
            (char*)dst.data(), 
            (int)src.size(), 
            (int)original_size
        );
        
        if (decomp_size < 0) {
            std::cerr << "LZ4_decompress_safe() failed with code: " << decomp_size << '\n';
            // Decompression failed
            dst.clear();
            return 0;
        }
        else if ((size_buffer_t)decomp_size != original_size) {
            std::cerr << "Warning: Decompressed size (" << decomp_size 
                      << ") does not match the expected original size (" 
                      << original_size << ").\n";
        }

        dst.resize(decomp_size);
        dst.shrink_to_fit();
        return dst.size();
    }
} // namespace lz4
