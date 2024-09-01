cmake_minimum_required(VERSION 3.10)

project(ECS)

include(FetchContent)

FetchContent_Declare(
    Box2D
    GIT_REPOSITORY https://github.com/erincatto/box2d
    GIT_TAG v2.4.2
)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
    GIT_TAG 0.8.0
)

FetchContent_MakeAvailable(Box2D)
FetchContent_MakeAvailable(yaml-cpp)

# adding SDL libraries
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_mixer)
add_subdirectory(external/SDL_ttf)

add_subdirectory(core)

add_executable(TestApplication test.cpp)

target_link_libraries(TestApplication PRIVATE Core Box2D yaml-cpp SDL2::SDL2 SDL2_mixer SDL2_image SDL2_ttf)