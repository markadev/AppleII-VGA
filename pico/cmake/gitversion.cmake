cmake_minimum_required(VERSION 3.0.0)

message(STATUS "Resolving GIT Version")

set(_git_hash "unknown")

find_package(Git)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        OUTPUT_VARIABLE _git_hash
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "GIT hash: ${_git_hash}")
else()
    message(STATUS "GIT not found")
endif()

add_definitions("-DGIT_REVISION=\"${_git_hash}\"")
