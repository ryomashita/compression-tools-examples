function(enable_sanitizers)
  # set_target_properties(${target} PROPERTIES COMPILE_OPTIONS "" )
  # target_compile_options(${target} PRIVATE -O0 -g -fno-omit-frame-pointer
  # -fsanitize=address -fsanitize=leak -fsanitize=undefined )
  # 個別にコンパイルオプションを設定すると、リンカーエラーが発生するため一括で設定する
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -O0 -g -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak -fsanitize=undefined"
  )
endfunction()

# AddressSanitizer Reference
# https://github.com/google/sanitizers/wiki/AddressSanitizer
