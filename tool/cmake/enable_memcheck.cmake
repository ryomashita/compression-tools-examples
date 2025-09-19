find_program(VALGRIND_EXECUTABLE valgrind)
if(VALGRIND_EXECUTABLE)
  set(ENABLE_MEMCHECK ON)
else()
  message(STATUS "valgrind not found. Skipping enable_memcheck().")
  set(ENABLE_MEMCHECK OFF)
endif()

# valgrind3.18 では clang ビルド時失敗するが, 3.22 では成功する (DWARF バージョン対応?が原因か)

if(ENABLE_MEMCHECK)
  function(enable_memcheck target)
    # message(STATUS "VALGRIND_EXECUTABLE: ${VALGRIND_EXECUTABLE}")
    get_target_property(target_type ${target} TYPE)
    if(NOT target_type STREQUAL "EXECUTABLE")
      message(
        FATAL_ERROR "enable_memcheck() can only be used with executable targets")
    endif()
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${VALGRIND_EXECUTABLE} --leak-check=full --error-exitcode=1
              $<TARGET_FILE:${target}>
      # --tool=memcheck, -s は　--leak-check=full があれば不要
    )
  endfunction()
else()
  function(enable_memcheck target)
    message(STATUS "enable_memcheck() is disabled because valgrind not found.")
  endfunction()
endif()
