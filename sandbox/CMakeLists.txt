cmake_minimum_required(VERSION 3.10)

project(sandbox)

add_executable(sandbox src/Sandbox.cpp)

target_compile_definitions(
   sandbox
   PRIVATE
   PROJECT_NAME=sandbox
)

target_link_libraries(sandbox PRIVATE totem)

