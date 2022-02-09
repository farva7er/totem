cmake_minimum_required(VERSION 3.10)

add_subdirectory(Dependencies/glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Dependencies/glfw-3.3.6)

add_subdirectory(Dependencies/stb)

project(totem)

add_compile_definitions(PROJECT_NAME=totem)

add_library(totem STATIC   App.h
                           Log.h
                           Platform/Linux/LinuxWindow.h
                           Platform/Linux/LinuxWindow.cpp
                           Totem.cpp
                           App.cpp
                           Log.cpp
                           Window.cpp
                           Window.h
                           Events.h
                           Assert.h
                           Renderer/Renderer.h
                           Renderer/Renderer.cpp
                           Renderer/Texture.h
                           Renderer/Texture.cpp
                           Math/Mat.h
                           Math/Mat.cpp
                           Math/Vec.h
                           ResourceManager.h
                           ResourceManager.cpp)


target_include_directories(totem PUBLIC ./)

target_link_libraries(totem PRIVATE glad)
target_link_libraries(totem PRIVATE glfw)
target_link_libraries(totem PRIVATE stb)
