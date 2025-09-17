cmake_minimum_required(VERSION 3.24)
project(
  CppTemplateProject
  VERSION 0.1
  LANGUAGES CXX)

# CMake options e.g., cmake ... -DENABLE_SANITIZERS=ON
option(CppTemplateProject_OPTION_ENABLE_SANITIZERS "Run AddressSanitizer" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# generate build/compile_commands.json (for clang-tools)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set cmake scripts directory
set(CMAKE_SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/tool/cmake)
include(${CMAKE_SCRIPTS_DIR}/set_compile_props.cmake)
include(${CMAKE_SCRIPTS_DIR}/set_warnings_as_errors.cmake)
include(${CMAKE_SCRIPTS_DIR}/enable_memcheck.cmake)
include(${CMAKE_SCRIPTS_DIR}/enable_sanitizers.cmake)

# set default install path (can be overwritten with -DCMAKE_INSTALL_PREFIX=...)
set(CMAKE_INSTALL_PREFIX
    ${CMAKE_SOURCE_DIR}/install
    CACHE PATH "..." FORCE)

# create ctest configuration in current directory. This command should be in the
# source directory root.
enable_testing()

if(CppTemplateProject_OPTION_ENABLE_SANITIZERS)
  enable_sanitizers()
endif()

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
