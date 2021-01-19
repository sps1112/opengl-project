cmake_minimum_required(VERSION "3.16.3")

project(opengl-project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(ASSIMP REQUIRED)

include_directories(includes)
if(WIN32)
link_directories(lib)
endif()

add_executable(
    opengl-project

    src/Application.cpp
    src/glad.c
    src/stb_image.cpp

    includes/Shader.h
    includes/Camera.h
    includes/Mesh.h
    includes/Model.h
)

target_link_libraries(opengl-project ${CMAKE_DL_LIBS})
if(WIN32)
target_link_libraries(
        opengl-project
        ${glfw_LIBRARIES}
)
target_link_libraries(opengl-project glfw3)
else()
target_link_libraries(opengl-project glfw)
endif()