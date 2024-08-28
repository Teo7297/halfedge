cmake_minimum_required(VERSION 3.5.0)
project(HalfEdge VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB SOURCES "src/*.cpp")

add_executable(HalfEdge ${SOURCES})

target_include_directories(HalfEdge PUBLIC include)


