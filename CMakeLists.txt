cmake_minimum_required(VERSION 3.10)

project(ECS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# disable box2d tests build
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Disable Box2D Unit Tests build" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Disable Box2D Testbed build" FORCE)

# disable tileson tests and examples build
set(BUILD_TESTS OFF CACHE BOOL "Disable Tileson tests build" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable Tileson examples build" FORCE)

# build SDL libraries as shared-object
set(SDL_STATIC OFF CACHE BOOL "Disble SDL static library bulid" FORCE)
set(SDL_SHARED ON CACHE BOOL "Build SDL as shared object" FORCE)

set(SDL_TEST OFF CACHE BOOL "Disable SDL tests build" FORCE)

# adding SDL libraries
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_mixer)
add_subdirectory(external/SDL_ttf)
add_subdirectory(external/SDL2_gfx)

add_subdirectory(external/box2d)
add_subdirectory(external/tileson)
add_subdirectory(external/yaml-cpp)

include_directories(
    external/box2d/include
    external/tileson/include
    external/yaml-cpp/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIRS}
)

# include library
add_subdirectory(src)

# include tests
add_subdirectory(tests)