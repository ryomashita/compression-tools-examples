# ファイル（テスト用データなど）をビルドディレクトリにシンボリックリンクでコピーする
function(create_symlink_to_build target filepath)
  cmake_path(GET filepath FILENAME filename)
  add_custom_target(
    create_symlink_${target}
    COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      "${CMAKE_CURRENT_SOURCE_DIR}/${filepath}"
      "$<TARGET_FILE_DIR:${target}>/${filename}")
  add_dependencies(${target} create_symlink_${target})
endfunction()
