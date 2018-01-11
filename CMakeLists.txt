cmake_minimum_required(VERSION 3.8)
project(Floton.sh)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O3)

include_directories(Eigen)

set(SOURCE_FILES main.cpp Object3D.cpp Object3D.h Transformation.cpp Transformation.h Triangle.cpp Triangle.h DriverParser.cpp DriverParser.h CameraSpecifications.h CameraPictureTaker.cpp CameraPictureTaker.h Ray.cpp Ray.h Sphere.h Light.h Lighting.h Material.cpp Material.h)
add_executable(Floton.sh ${SOURCE_FILES})
