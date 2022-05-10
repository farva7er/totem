cmake_minimum_required(VERSION 3.10)

project(tgenerator)

add_executable(tgenerator  src/Generator.h
                           src/Generator.cpp
                           src/GeneratorApp.h
                           src/GeneratorApp.cpp
                           src/QuotedTextReader.h
                           src/QuotedTextReader.cpp
                           src/SimpleTextWriter.h
                           src/SimpleTextWriter.cpp
                           )

target_compile_definitions(
   tgenerator
   PRIVATE
   PROJECT_NAME=tgenerator
)

target_link_libraries(tgenerator PRIVATE totem)
