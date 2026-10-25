cmake_minimum_required(VERSION 3.22.1)

project("minimalistcore")

add_library(
    minimalistcore
    SHARED
    native_core.cpp
)

find_library(
    log-lib
    log
)

target_link_libraries(
    minimalistcore
    ${log-lib}
)
