cmake_minimum_required(VERSION 3.5)

project(si-patch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(si-patch
    si-patch-main.cpp
    diff-makeone-v0.bin.cpp
    diff-makeone-v3.bin.cpp
    diff-stunt-v0.bin.cpp
    diff-stunt-v3.bin.cpp
)


add_executable(gen-diff gen-diff.cpp)

