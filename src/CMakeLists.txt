cmake_minimum_required(VERSION 3.10)

project(totem-development)

add_subdirectory(totem)
add_subdirectory(sandbox)

target_compile_definitions( 
   totem
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

