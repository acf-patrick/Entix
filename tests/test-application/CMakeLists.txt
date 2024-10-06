file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(TestApplication ${SOURCES})

target_include_directories(TestApplication PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    external/box2d/include
    external/tileson/include
    external/yaml-cpp/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIRS}
)

target_link_libraries(TestApplication PRIVATE 
    entix
    box2d
    SDL2::SDL2 
    SDL2_mixer 
    SDL2_image 
    SDL2_ttf
    SDL2_gfx
)