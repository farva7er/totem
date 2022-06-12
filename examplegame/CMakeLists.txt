cmake_minimum_required(VERSION 3.10)

project(examplegame)

add_executable(examplegame src/ExampleGame.cpp)
add_dependencies(examplegame copy_resources_to_build)

target_compile_definitions(
   examplegame
   PRIVATE
   PROJECT_NAME=examplegame
)

target_link_libraries(examplegame PRIVATE totem)

set_target_properties(examplegame PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
)
set_target_properties(examplegame PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(
   copy_resources_to_build ALL
   COMMAND
   ${CMAKE_COMMAND} -E copy_directory
   "${PROJECT_SOURCE_DIR}/resources"
   "${totem_SOURCE_DIR}/resources"
   "${CMAKE_CURRENT_BINARY_DIR}/resources"
   COMMENT "Copying resources from src to build..."
)
