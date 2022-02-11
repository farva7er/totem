add_library(glad STATIC src/glad.c
                        include/glad/glad.h
                        include/KHR/khrplatform.h)

target_include_directories(glad PUBLIC include/)
#target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
