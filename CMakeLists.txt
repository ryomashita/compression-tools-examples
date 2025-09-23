cmake_minimum_required(VERSION 3.24)

project(
  CompressionExample
  VERSION 0.1
  LANGUAGES CXX)

# load dotenv file
include(${CMAKE_SCRIPTS_DIR}/env.cmake)
load_env(${CMAKE_SOURCE_DIR}/.env)

# CMake options (can be set via a command line option.
# e.g., cmake ... -DENABLE_SANITIZERS=ON)
option(CppTemplateProject_OPTION_ENABLE_SANITIZERS "Run AddressSanitizer" OFF)

# set C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# generate build/compile_commands.json (for clang-tools)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# include cmake scripts
# (`CMAKE_SCRIPTS_DIR` is set in CMakePresets.json)
include(${CMAKE_SCRIPTS_DIR}/set_compile_props.cmake)
include(${CMAKE_SCRIPTS_DIR}/set_warnings_as_errors.cmake)
include(${CMAKE_SCRIPTS_DIR}/enable_memcheck.cmake)
if(CppTemplateProject_OPTION_ENABLE_SANITIZERS)
  include(${CMAKE_SCRIPTS_DIR}/enable_sanitizers.cmake)
  enable_sanitizers()
endif()

# add packages
find_package(zstd CONFIG REQUIRED)
find_package(lz4 CONFIG REQUIRED)

# create ctest configuration in current directory.
# This command should be in the source directory root.
enable_testing()

add_subdirectory(src)
add_subdirectory(test)

# I refered the following projecst and articles to make cmake scripts.
# https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C++.html
# https://github.com/boost-ext/ut

# CMake Style Guidelines:
# https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1
# https://fujii.github.io/2015/10/10/cmake-best-practice/
# https://learn.microsoft.com/ja-jp/vcpkg/contributing/cmake-guidelines
# https://www.slideshare.net/DanielPfeifer1/cmake-48475415
