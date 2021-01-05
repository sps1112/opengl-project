cmake_minimum_required(VERSION "3.16.3")

project(opengl-project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
include_directories(includes)


add_executable(
    opengl-project

    src/Application.cpp
    src/glad.c
    src/stb_image.cpp
)

target_link_libraries(opengl-project ${CMAKE_DL_LIBS})
target_link_libraries(opengl-project glfw)