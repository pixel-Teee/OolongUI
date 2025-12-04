cmake_minimum_required(VERSION 3.16)

file(GLOB SourceFile "main.cpp"
"Core/GPUResource/*.h"
"Core/GPUResource/*.cpp"
"Core/Application/*.h"
"Core/Application/*.cpp")

add_executable(OolongUI ${SourceFile})

target_link_libraries(OolongUI PRIVATE SDL3::SDL3 PRIVATE SDL3_shadercross-static)

message(${CMAKE_SOURCE_DIR}/vendored/SDLShaderCross)
target_include_directories(OolongUI PRIVATE ${CMAKE_SOURCE_DIR}/vendored/SDLShaderCross/include 
PRIVATE ${CMAKE_SOURCE_DIR}/src/Core)