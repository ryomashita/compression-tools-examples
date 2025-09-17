# GoogleTestを使用するためのCMakeの設定 使用方法： - testフォルタかプロジェクトルートフォルダで、
# `include(install_gtest.cmake)` を追加 - テストターゲットに `enable_gtest(<target>)` を追加する

# GoogleTestをダウンロードする
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
      DOWNLOAD_EXTRACT_TIMESTAMP OFF)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# GoogleTestを使用する場合は、 `GTest::gtest_main` をリンクする。
# `main` 関数をユーザ定義したい場合は、代わりに `GTest::gtest` をリンクする
function(link_gtest_main target)
  target_link_libraries(${target} PRIVATE GTest::gtest_main)
endfunction()
function(link_gtest target)
  # gtest : gtest 本体 (main関数は含まれず、別途定義の必要がある)
  target_link_libraries(${target} PRIVATE GTest::gtest)
endfunction()

# ターゲットをGoogleTestに登録する
function(register_gtest target)
  include(GoogleTest)
  gtest_discover_tests(${target})
endfunction()

# gtestのリンクと登録を一括で行う
function(enable_gtest target)
  link_gtest_main(${target})
  register_gtest(${target})
endfunction()
