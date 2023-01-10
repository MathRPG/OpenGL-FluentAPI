cmake_minimum_required(VERSION 3.24)
project(TransformLibrary C)

set(CMAKE_C_STANDARD 99)

add_executable(TransformLibrary main.c shape.c shape.h)

set(OpenGlLinkers -lglut -lGLU -lGL)
target_link_libraries(TransformLibrary ${OpenGlLinkers} -lm)