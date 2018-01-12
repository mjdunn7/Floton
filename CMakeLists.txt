cmake_minimum_required(VERSION 3.8)
project(Floton)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O3)

include_directories(Eigen)

file(GLOB SOURCE_FILES src/*.cpp src/*.h)

add_executable(Floton.sh ${SOURCE_FILES})
