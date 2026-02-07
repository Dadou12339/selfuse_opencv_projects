cmake_minimum_required(VERSION 4.2.2)
project(project1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)
add_compile_options(-finput-charset=utf-8 -fexec-charset=gbk)

set(OpenCV_DIR "../lib/opencv/install/x64/mingw/lib")
find_package(OpenCV REQUIRED)

add_executable(app src/main.cpp)

target_include_directories(
    app PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
    app PRIVATE
    ${OpenCV_LIBS}
)