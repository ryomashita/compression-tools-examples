# load environment variables from a dotenv file
# e.g., key="value" in ${CMAKE_SOURCE_DIR}/.env
# usage: 
#  include(${CMAKE_SCRIPTS_DIR}/env.cmake)
#  load_env(${CMAKE_SOURCE_DIR}/.env)
function(load_env ENV_FILE_PATH)
    file(STRINGS ${ENV_FILE_PATH} ENV_FILE)
    foreach(VAR ${ENV_FILE})
        string(REGEX MATCH "^[^=]*" KEY ${VAR})
        string(REGEX REPLACE "^[^=]*=" "" VALUE ${VAR})
        set(ENV{${KEY}} ${VALUE})
    endforeach()
endfunction()
