cmake_minimum_required(VERSION 3.16)
project(OolongUI)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDLSHADERCROSS_DXC OFF)
add_subdirectory("vendored/SDL")

set(SDLSHADERCROSS_DXC OFF CACHE BOOL "")
set(SPIRV_CROSS_CLI OFF CACHE BOOL "Build SPIRV-Cross CLI" FORCE)
set(SPIRV_CROSS_SHARED ON CACHE BOOL "Build shared lib" FORCE)
set(SPIRV_CROSS_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory("vendored/SpirvCross")

set(SDL3_SOURCE_DIR ${CMAKE_BINARY_DIR}/vendored/SDL)
set(spirv_cross_c_shared_DIR ${CMAKE_BINARY_DIR}/vendored/SpirvCross)
add_subdirectory("vendored/SDLShaderCross")

add_subdirectory("src")

