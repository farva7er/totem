cmake_minimum_required(VERSION 3.10)

project(totem)

add_library(totem STATIC
                        src/App.h
                        src/Assert.h
                        src/Events.h
                        src/Log.h
                        src/Log.cpp
                        src/Timer.h
                        src/Timer.cpp
                        src/Totem.cpp
                        src/Window.h
                        src/Window.cpp
                        src/Animations/BasicAnimations/InterpAnimations.h
                        src/Animations/BasicAnimations/SeqAnimation.h
                        src/Animations/BasicAnimations/SeqAnimation.cpp
                        src/Animations/Animator.h
                        src/Animations/Animator.cpp
                        src/Animations/Animation.h
                        src/Animations/Animation.cpp
                        src/Core/Resource.h
                        src/Core/Resource.cpp
                        src/Core/ResourceManager.h
                        src/Core/ResourceManager.cpp
                        src/Math/Mat.h
                        src/Math/Mat.cpp
                        src/Math/Vec.h
                        src/Math/
$<$<STREQUAL:${CMAKE_SYSTEM_NAME},Linux>:
                        src/Platform/Linux/LinuxWindow.h
                        src/Platform/Linux/LinuxWindow.cpp
                        src/Platform/Linux/Filesystem/LinuxFileSystem.h
                        src/Platform/Linux/Filesystem/LinuxFileSystem.cpp
>
$<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:
                        src/Platform/Linux/LinuxWindow.h # Temporary
                        src/Platform/Linux/LinuxWindow.cpp # Temporary
                        src/Platform/Windows/Filesystem/WindowsFileSystem.h
                        src/Platform/Windows/Filesystem/WindowsFileSystem.cpp
>
                        src/Rendering/Font.h
                        src/Rendering/Font.cpp
                        src/Rendering/Renderer.h
                        src/Rendering/Renderer.cpp
                        src/Rendering/Primitives/Rect.h
                        src/Rendering/Primitives/Rect.cpp
                        src/Rendering/Shader.h
                        src/Rendering/Shader.cpp
                        src/Rendering/Texture.h
                        src/Rendering/Texture.cpp
                        src/UI/Clickable.h
                        src/UI/Clickable.cpp
                        src/UI/Element.h
                        src/UI/TextBox.h
                        src/UI/TextBox.cpp
                        src/Filesystem/FileSystem.h
                        src/Filesystem/FileSystem.cpp
                        src/Internationalization/Text.h
                        src/Internationalization/Text.cpp
                        src/Novel/DialogBox.h
                        src/Novel/DialogBox.cpp
                        src/Novel/NovelApp.h
                        src/Novel/NovelApp.cpp
                        src/Novel/Script.h
                        src/Novel/Script.cpp
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


target_compile_options(totem PRIVATE
   $<$<CXX_COMPILER_ID:MSVC>:/W4>
   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
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

set(FT_DISABLE_ZLIB TRUE CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 TRUE CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG TRUE CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ TRUE CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI TRUE CACHE BOOL "" FORCE)
add_subdirectory(Dependencies/freetype-2.11.1)

target_link_libraries(totem PRIVATE glad)
target_link_libraries(totem PRIVATE glfw)
target_link_libraries(totem PRIVATE stb)
target_link_Libraries(totem PRIVATE freetype)


#################################################
#################################################
