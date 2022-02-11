cmake_minimum_required(VERSION 3.10)

project(sandbox)

add_executable(sandbox src/Sandbox.cpp)

target_compile_definitions(
   sandbox
   PRIVATE
   PROJECT_NAME=sandbox
)

target_link_libraries(sandbox PRIVATE totem)

add_custom_command(
   TARGET sandbox 
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   "${PROJECT_SOURCE_DIR}/resources"
   "$<TARGET_FILE_DIR:sandbox>/resources"
)
