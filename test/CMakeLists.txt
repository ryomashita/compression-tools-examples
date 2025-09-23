include(${CMAKE_SCRIPTS_DIR}/install_gtest.cmake)

add_executable(AddTest add_test.cpp)
target_include_directories(AddTest PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(AddTest PRIVATE Add)
target_link_libraries(AddTest PRIVATE zstd::libzstd)
enable_gtest(AddTest)

add_executable(ZstdppTest zstd/zstdpp_test.cpp)
target_include_directories(ZstdppTest PRIVATE ${CMAKE_SOURCE_DIR}/src/zstd)
target_link_libraries(ZstdppTest PRIVATE Zstdpp)
target_link_libraries(ZstdppTest PRIVATE zstd::libzstd)
enable_gtest(ZstdppTest)

add_executable(Lz4Test lz4/lz4cpp_test.cpp)
target_include_directories(Lz4Test PRIVATE ${CMAKE_SOURCE_DIR}/src/lz4)
target_link_libraries(Lz4Test PRIVATE lz4::lz4)
enable_gtest(Lz4Test)

add_executable(AesSample cryptopp/cryptopp_aes_test.cpp)
target_include_directories(AesSample PRIVATE ${CMAKE_SOURCE_DIR}/src/cryptopp)
target_link_libraries(AesSample PRIVATE cryptopp::cryptopp)
enable_gtest(AesSample)

