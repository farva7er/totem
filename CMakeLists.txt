cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 11)

project(totem-development)

add_subdirectory(totem)
add_subdirectory(examplegame)
add_subdirectory(tools/tgenerator)
add_subdirectory(tests)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            PROPERTY VS_STARTUP_PROJECT sandbox)

