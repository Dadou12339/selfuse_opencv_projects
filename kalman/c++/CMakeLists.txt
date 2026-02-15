cmake_minimum_required(VERSION 4.2.2)
project(kalman)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)
add_compile_options(-finput-charset=utf-8 -fexec-charset=gbk)

include(${CMAKE_SOURCE_DIR}/build/Release/generators/conan_toolchain.cmake)

find_package(Eigen3 REQUIRED)

add_executable(app src/main.cpp)

target_link_libraries(
    app PRIVATE
    Eigen3::Eigen
)