cmake_minimum_required(VERSION 3.20)

project(OpenGLPong VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Automatically find source files
file(GLOB_RECURSE FRAMEWORK_SOURCES
    framework/src/*.cpp
)

file(GLOB_RECURSE GAME_SOURCES
    game/*.cpp
)

set(GLAD_SOURCE
    external/glad/src/glad.c
)

set(STB_SOURCE
    external/stb_image/stb_image.cpp
)

add_executable(OpenGLPong
    ${FRAMEWORK_SOURCES}
    ${GAME_SOURCES}
    ${GLAD_SOURCE}
    ${STB_SOURCE}
)


target_include_directories(OpenGLPong PRIVATE
     ${PROJECT_SOURCE_DIR}/framework/include/
     ${PROJECT_SOURCE_DIR}/external/glad/include
     ${PROJECT_SOURCE_DIR}/external/stb_image
     ${PROJECT_SOURCE_DIR}/external/glm
     ${PROJECT_SOURCE_DIR}/framework/include/core
)

target_link_libraries(OpenGLPong PRIVATE
    glfw
    OpenGL::GL
)

add_custom_command(TARGET OpenGLPong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/
        $<TARGET_FILE_DIR:OpenGLPong>/assets/
)