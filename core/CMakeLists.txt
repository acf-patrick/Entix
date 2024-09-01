file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_library(Core STATIC ${SOURCES})

target_include_directories(Core PRIVATE 
    ${CMAKE_SOURCE_DIR}/external/tileson/include
    ${CMAKE_SOURCE_DIR}/external/yaml-cpp/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

target_link_libraries(Core PRIVATE
    yaml-cpp
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)
