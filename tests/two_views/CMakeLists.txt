file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(EntixTest_TwoViews ${SOURCES})

target_include_directories(EntixTest_TwoViews PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    external/yaml-cpp/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIRS}
)

target_link_libraries(EntixTest_TwoViews PRIVATE 
    entix
    SDL2::SDL2 
    SDL2_image 
    SDL2_gfx
)