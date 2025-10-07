cmake_minimum_required(VERSION 3.10)
project(FaceEnhancer)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} cpp_src/includes)

file(GLOB SOURCES cpp_src/*.cpp)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/backend)

add_executable(face_enhancer ${SOURCES})

target_link_libraries(face_enhancer ${OpenCV_LIBS})
