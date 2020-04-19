cmake_minimum_required(VERSION 3.15)
project(opengl-examples)

set(CMAKE_CXX_STANDARD 20)

find_library(GLEW REQUIRED)
find_library(glfw REQUIRED)
find_library(OpenGL REQUIRED)

file(GLOB SOURCE_FILES ./src/*.cpp )

add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )
target_link_libraries( ${PROJECT_NAME} GLEW glfw OpenGL )

set(EXECUTABLE_OUTPUT_PATH "..")

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        SUFFIX ".o"
)