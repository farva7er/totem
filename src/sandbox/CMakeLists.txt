cmake_minimum_required(VERSION 3.10)

project(sandbox)

add_executable(sandbox sandbox.cpp)

add_compile_definitions(PROJECT_NAME=sandbox)

target_link_libraries(sandbox PRIVATE totem)
