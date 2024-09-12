cmake_minimum_required(VERSION 3.10)

project(SDL2_gfx VERSION 1.0.4)

include(FetchContent)

FetchContent_Declare(
    SDL2_gfx
    URL https://www.ferzkopp.net/Software/SDL2_gfx/SDL2_gfx-1.0.4.tar.gz
    URL_HASH MD5=15f9866c6464ca298f28f62fe5b36d9f
)

FetchContent_MakeAvailable(SDL2_gfx)

file(GLOB SDL2_GFX_SOURCES 
    "${CMAKE_BINARY_DIR}/_deps/sdl2_gfx-src/*.c"
)

set(SDL2_GFX_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/_deps/sdl2_gfx-src" PARENT_SCOPE)

find_package(SDL2 REQUIRED)

add_library(SDL2_gfx STATIC ${SDL2_GFX_SOURCES})

target_include_directories(SDL2_gfx PRIVATE ${SDL2_INCLUDE_DIRS})

# Might link SDL2 here