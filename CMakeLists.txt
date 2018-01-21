cmake_minimum_required(VERSION 3.8)
project(Floton)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O3)

include_directories(Eigen)

file(GLOB SOURCE_FILES src/*.cpp src/*.h)

add_executable(Floton ${SOURCE_FILES} src/RayTraceData.cpp src/RayTraceData.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Floton Threads::Threads)

set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}..)
install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}/..)
