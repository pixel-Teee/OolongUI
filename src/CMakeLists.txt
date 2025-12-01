cmake_minimum_required(VERSION 3.16)

file(GLOB SourceFile "main.cpp"
"Core/*.h"
"Core/*.cpp")

add_executable(OolongUI ${SourceFile})

target_link_libraries(OolongUI PRIVATE SDL3::SDL3 PRIVATE SDL3_shadercross-static)

message(${CMAKE_SOURCE_DIR}/vendored/SDLShaderCross)
target_include_directories(OolongUI PRIVATE ${CMAKE_SOURCE_DIR}/vendored/SDLShaderCross/include)