add_library(Add add.cpp)
set_normal_compile_options(Add)
set_warnings_as_errors(Add)

add_executable(zstd_example zstd/simple_compression.cpp)
set_normal_compile_options(zstd_example)
set_warnings_as_errors(zstd_example)
target_link_libraries(zstd_example PRIVATE zstd::libzstd)

add_library(Zstdpp INTERFACE ${CMAKE_SOURCE_DIR}/src/zstd)
