function(set_warnings_as_errors target)
  if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC"
     OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES
                                                   "MSVC"))
    target_compile_options(${target} PRIVATE /WX)
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
      # replace /W+ with /W4
      string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
      target_compile_options(${target} PRIVATE /W4)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${target} PRIVATE -Werror)
  else()
    message(FATAL_ERROR "Not supported compiler")
  endif()
endfunction()
