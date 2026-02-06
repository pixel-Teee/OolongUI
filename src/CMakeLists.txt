cmake_minimum_required(VERSION 3.16)

file(GLOB SourceFile "main.cpp"
"Core/GPUResource/*.h"
"Core/GPUResource/*.cpp"
"Core/Application/*.h"
"Core/Application/*.cpp"
"Core/Renderer/*.h"
"Core/Renderer/*.cpp"
"Core/File/*.h"
"Core/File/*.cpp"
"Core/Str/*.h"
"Core/Str/utf8/*.h"
"Core/Widget/*.h"
"Core/Widget/*.cpp"
"Core/Math/*.h"
"Core/Math/*.cpp")

add_executable(OolongUI ${SourceFile})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} 
    PREFIX "Source Files"
    FILES ${SourceFile}
)

target_link_libraries(OolongUI PRIVATE SDL3::SDL3 PRIVATE SDL3_shadercross-static)

message(${CMAKE_SOURCE_DIR}/vendored/SDLShaderCross)
target_include_directories(OolongUI PRIVATE ${CMAKE_SOURCE_DIR}/vendored/SDLShaderCross/include 
PRIVATE ${CMAKE_SOURCE_DIR}/src/Core)