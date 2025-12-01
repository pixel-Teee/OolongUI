cmake_minimum_required(VERSION 3.16)

file(GLOB SourceFile "main.cpp"
"Core/*.h"
"Core/*.cpp")

add_executable(OolongUI ${SourceFile})

target_link_libraries(OolongUI PRIVATE SDL3::SDL3)