function(set_target_cxx20 target)
  target_compile_features(${target} PRIVATE cxx_std_20)
endfunction()

function(set_normal_compile_options target)
  # Set Language Standard
  set_target_cxx20(${target})

  # Set Compile Options
  if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC"
     OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES
                                                   "MSVC"))
    # using Visual Studio C++ (/W4) # 例: 警告レベルを設定
    target_compile_options(${target} PRIVATE /W4)
    target_compile_options(${target} PRIVATE $<$<CONFIG:Release>:/O2>)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # using GCC or Clang
    target_compile_options(${target} PRIVATE -O2)
    # Wextra : 有用でないor回避しづらい警告を有効にする Wpedantic : コンパイラ拡張機能を警告する Wshadow=local :
    # ローカル変数での変数名の重複を警告する
    target_compile_options(${target} PRIVATE -Wextra -Wpedantic)

    # Recommended Compile Options by OpenSSF
    # https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C++.html
    target_compile_options(
      ${target}
      PRIVATE -Wall
              -Wformat=2
              -Wconversion
              -Wimplicit-fallthrough
              -U_FORTIFY_SOURCE
              -D_FORTIFY_SOURCE=3
              -D_GLIBCXX_ASSERTIONS
              -fstrict-flex-arrays=3 # from GCC 13 & Clang 16.0.0
              -fstack-clash-protection
              -fstack-protector-strong)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      target_compile_options(
        ${target}
        PRIVATE -Wshadow=local
                -Wtrampolines # Only GCC
                -Wl,-z,nodlopen
                -Wl,-z,noexecstack # linker options are warned in clang
                -Wl,-z,relro
                -Wl,-z,now
                -fPIE
                -pie
                -fPIC
                -shared)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION
                                                      GREATER_EQUAL 10.0)
      # deprecated-copy-dtor : C++17以降で非推奨となったコピーコンストラクタを警告する newline-eof :
      # ファイルの最後に改行がない場合に警告する
      target_compile_options(${target} PRIVATE -Wshadow -Wdeprecated-copy-dtor
                                               -Wnewline-eof)
    endif()
  else()
    # if(CMAKE_CXX_COMPILER_ID MATCHES  "Intel")
    message(FATAL_ERROR "Not supported compiler")
  endif()
endfunction()
