cmake_minimum_required(VERSION "3.16.3")

project(opengl-project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
endif() 

include_directories(includes)
include_directories(configuration)
if(WIN32)
link_directories(lib)
endif()

add_executable(
    opengl-project

    src/Application.cpp
    src/glad.c
    src/stb_image.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/FileSystem.cpp
    src/Utils.cpp
    src/Primitive.cpp
    src/FileReader.cpp

    includes/Shader.h
    includes/Camera.h
    includes/Mesh.h
    includes/Model.h
    includes/FileSystem.h
    includes/Utils.h
    includes/Primitive.h
    includes/FileReader.h
)

target_link_libraries(opengl-project ${CMAKE_DL_LIBS})
if(WIN32)
target_link_libraries(
        opengl-project
        ${glfw_LIBRARIES}
)
target_link_libraries(opengl-project glfw3)
target_link_libraries(opengl-project assimp)
else()
target_link_libraries(opengl-project glfw)
target_link_libraries(opengl-project assimp)
endif()

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

if(WIN32)
add_custom_command(TARGET opengl-project POST_BUILD   
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/dlls/assimp-vc142-mtd.dll"
        $<TARGET_FILE_DIR:opengl-project>)    
endif()