cmake_minimum_required(VERSION 3.10)
project(LineCounter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TARGET LineCounter)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set(SOURCE_FILES
    ${SOURCE_DIR}/FileReader.cpp
    ${SOURCE_DIR}/FileReader.h
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/TaskManager.cpp
    ${SOURCE_DIR}/TaskManager.h
)

add_executable(${TARGET} ${SOURCE_FILES})