include_directories(${CMAKE_SOURCE_DIR}/src)

# add
add_library(Add add.cpp)
set_normal_compile_options(Add)
set_warnings_as_errors(Add)

# zstd
add_executable(zstd_example zstd/simple_compression.cpp)
set_normal_compile_options(zstd_example)
set_warnings_as_errors(zstd_example)
target_link_libraries(zstd_example PRIVATE zstd::libzstd)

add_library(Zstdpp INTERFACE ${CMAKE_SOURCE_DIR}/src/zstd)

# lz4
add_executable(lz4_example lz4/simple_buffer.cpp)
set_normal_compile_options(lz4_example)
target_link_libraries(lz4_example PRIVATE lz4::lz4)

# crypto++
add_executable(cryptopp_aes_example cryptopp/aes_sample.cpp)
set_normal_compile_options(cryptopp_aes_example)
target_link_libraries(cryptopp_aes_example PRIVATE cryptopp::cryptopp)

