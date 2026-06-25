cmake_minimum_required(VERSION 3.20)

# SFML-MATH
project(SM) 

set(SOURCE_FILES
    include/SM/Precision.h
    src/Vec2.cpp
    src/Vec3.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)