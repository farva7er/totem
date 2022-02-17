cmake_minimum_required(VERSION 3.10)

project(totem)

add_library(totem STATIC   src/App.h
                           src/App.cpp
                           src/Assert.h
                           src/Events.h
                           src/Log.h
                           src/Log.cpp
                           src/Log.cpp
                           src/ResourceManager.h
                           src/ResourceManager.cpp
                           src/Timer.h
                           src/Timer.cpp
                           src/Totem.cpp
                           src/Window.h
                           src/Window.cpp
                           src/Animations/Animator.h
                           src/Animations/Animator.cpp
                           src/Animations/BasicAnimations.h
                           src/Math/Mat.h
                           src/Math/Mat.cpp
                           src/Math/Vec.h
                           src/Math/
$<$<STREQUAL:${CMAKE_SYSTEM_NAME},Linux>:
                           src/Platform/Linux/LinuxWindow.h
                           src/Platform/Linux/LinuxWindow.cpp
                           src/Platform/Linux/Filesystem/LinuxUtils.h
                           src/Platform/Linux/Filesystem/LinuxUtils.cpp
>
$<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:
                           src/Platform/Linux/LinuxWindow.h # Temporary
                           src/Platform/Linux/LinuxWindow.cpp # Temporary
                           src/Platform/Windows/Filesystem/WindowsUtils.h
                           src/Platform/Windows/Filesystem/WindowsUtils.cpp
>
                           src/Renderer/Renderer.h
                           src/Renderer/Renderer.cpp
                           src/Renderer/Shader.h
                           src/Renderer/Shader.cpp
                           src/Renderer/Texture.h
                           src/Renderer/Texture.cpp
                           src/Filesystem/Utils.h
                           src/Filesystem/Utils.cpp
                           )


target_include_directories(totem PUBLIC src)

target_compile_definitions( 
   totem
   PRIVATE
   PROJECT_NAME=totem
   PUBLIC
   # If the debug configuration pass the DEBUG define to the compiler 
   $<$<CONFIG:Debug>:TOTEM_DEBUG>
   #$<$<STREQUAL:${CMAKE_SYSTEM_NAME}, Linux>:TOTEM_PLATFORM_LINUX>
   $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Linux>:TOTEM_PLATFORM_LINUX>
   $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:TOTEM_PLATFORM_WINDOWS>
)

target_compile_options(
   totem
   PUBLIC
   $<$<CONFIG:Debug>:-Wall -g>
)

###############################################
## DEPENDENCIES ###############################
###############################################

add_subdirectory(Dependencies/glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Dependencies/glfw-3.3.6)

add_subdirectory(Dependencies/stb)

target_link_libraries(totem PRIVATE glad)
target_link_libraries(totem PRIVATE glfw)
target_link_libraries(totem PRIVATE stb)

target_compile_options(
   glad
   PRIVATE
   $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:/w>
)
target_compile_options(
   glfw
   PRIVATE
   $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:/w>
)
target_compile_options(
   stb
   PRIVATE
   $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:/w>
)
#################################################
#################################################
