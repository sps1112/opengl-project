cmake_minimum_required(VERSION "3.16.3")

project(opengl-project)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
endif() 

include_directories(includes)
include_directories(includes/external)
include_directories(configuration)
if(WIN32)
link_directories(lib)
endif()

add_executable(
    opengl-project

    src/Application.cpp
    src/external/glad.c
    src/external/stb_image.cpp
    src/rendering/Shader.cpp
    src/rendering/Camera.cpp
    src/rendering/Renderer.cpp
    src/rendering/Texture.cpp
    src/rendering/Light.cpp
    src/object/Mesh.cpp
    src/object/Model.cpp
    src/object/Primitive.cpp
    src/object/Scene.cpp
    src/utility/FileSystem.cpp
    src/utility/Utils.cpp
    src/utility/FileReader.cpp
    src/utility/CustomMath.cpp
    src/gui/GUI.cpp
    src/gui/ExampleGUI.cpp
    src/gui/ObjectUI.cpp

    includes/external/imgui/imgui.cpp
    includes/external/imgui/imgui_draw.cpp
    includes/external/imgui/imgui_demo.cpp
    includes/external/imgui/imgui_widgets.cpp
    includes/external/imgui/imgui_tables.cpp
    includes/external/imgui/imgui_impl_glfw.cpp
    includes/external/imgui/imgui_impl_opengl3.cpp
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