cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

project(Mandelbrot VERSION 1.0)

add_subdirectory(libs)
add_subdirectory(src)