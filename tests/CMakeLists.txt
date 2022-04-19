cmake_minimum_required(VERSION 3.10)

project(tests)

add_executable(tests Tests.h
                     Tests.cpp
                     TextTests.h
                     TextTests.cpp)

target_compile_definitions(
   tests
   PRIVATE
   PROJECT_NAME=tests
)

target_link_libraries(tests PRIVATE totem)
