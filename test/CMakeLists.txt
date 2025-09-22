include(${CMAKE_SCRIPTS_DIR}/install_gtest.cmake)

add_executable(AddTest add_test.cpp)
target_include_directories(AddTest PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(AddTest PRIVATE Add)
target_link_libraries(AddTest PRIVATE zstd::libzstd)
enable_gtest(AddTest)

add_executable(ZstdppTest zstd/test_zstdpp.cpp)
target_include_directories(ZstdppTest PRIVATE ${CMAKE_SOURCE_DIR}/src/zstd)
target_link_libraries(ZstdppTest PRIVATE Zstdpp)
target_link_libraries(ZstdppTest PRIVATE zstd::libzstd)
enable_gtest(ZstdppTest)
