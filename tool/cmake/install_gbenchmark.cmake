# install google/benchmark

# options for google/benchmark 
# reference: https://github.com/google/benchmark/blob/main/CMakeLists.txt
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE) # downloading unmet dependencies
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE) # testing of the benchmark library
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE) # building the unit tests
set(BENCHMARK_ENABLE_LIBPFM ON CACHE BOOL "" FORCE) # using libpfm

include(FetchContent)
FetchContent_Declare(
  gbenchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
      DOWNLOAD_EXTRACT_TIMESTAMP OFF)
FetchContent_MakeAvailable(gbenchmark)

function(gbenchmark_download_script target_dir)
  add_custom_command(
    OUTPUT ${target_dir}/compare.py
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${gbenchmark_SOURCE_DIR}/tools ${target_dir}
    DEPENDS ${gbenchmark_SOURCE_DIR}/tools
    # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "[gbenchmark] Copying download script to ${target_dir}"
    VERBATIM # Escape all arguments properly
  )
  # need to build this target specifically to download the script (or add `ALL` option)
  add_custom_target(download_gbench_script DEPENDS ${target_dir}/compare.py)
endfunction()

# link google-benchmark to target
function(link_gbenchmark target)
  target_link_libraries(${target} benchmark::benchmark)
endfunction()