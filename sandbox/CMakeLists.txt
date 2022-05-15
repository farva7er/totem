cmake_minimum_required(VERSION 3.10)

project(sandbox)

add_executable(sandbox src/Sandbox.cpp)
add_dependencies(sandbox copy_resources_to_build)

target_compile_definitions(
   sandbox
   PRIVATE
   PROJECT_NAME=sandbox
)

target_link_libraries(sandbox PRIVATE totem)

add_custom_target(
   copy_resources_to_build ALL
   COMMAND
   ${CMAKE_COMMAND} -E copy_directory
   "${PROJECT_SOURCE_DIR}/resources"
   "${totem_SOURCE_DIR}/resources"
   "${CMAKE_CURRENT_BINARY_DIR}/resources"
   COMMENT "Copying resources from src to build..."
)
