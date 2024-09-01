file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_library(Core STATIC ${SOURCES})

target_link_libraries(Core PRIVATE
    Box2D
    yaml-cpp
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)

target_include_directories(Core PRIVATE 
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)