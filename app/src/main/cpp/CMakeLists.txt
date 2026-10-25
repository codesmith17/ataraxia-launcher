cmake_minimum_required(VERSION 3.22.1)

project("ataraxiacore")

add_library(
    ataraxiacore
    SHARED
    native_core.cpp
)

find_library(
    log-lib
    log
)

target_link_libraries(
    ataraxiacore
    ${log-lib}
)
