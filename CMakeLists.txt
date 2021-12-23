cmake_minimum_required(VERSION 3.10)

project(Mandelbrot VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(libs)
add_subdirectory(src)