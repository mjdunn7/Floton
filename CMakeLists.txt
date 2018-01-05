cmake_minimum_required(VERSION 3.8)
project(cs410)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O3)
#set(CMAKE_CXX_FLAGS -O3)
#set(CMAKE_CXX_FLAGS -pthread)
#set(CMAKE_CXX_FLAGS -march=native)

include_directories(Eigen)

set(SOURCE_FILES main.cpp Object3D.cpp Object3D.h Transformation.cpp Transformation.h Triangle.cpp Triangle.h DriverParser.cpp DriverParser.h CameraSpecifications.h CameraPictureTaker.cpp CameraPictureTaker.h Ray.cpp Ray.h Sphere.h Light.h Lighting.h Material.cpp Material.h)
add_executable(cs410 ${SOURCE_FILES})