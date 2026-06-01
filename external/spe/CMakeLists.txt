cmake_minimum_required(VERSION 3.27.3)

project(SPE)

set(SOURCE_FILES
    src/Particle3.cpp
    src/Particle3ForceRegistry.cpp
    )

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# Adding the custom Math Library
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/sm/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/sm)
target_link_libraries(${PROJECT_NAME} SM)

target_include_directories(${PROJECT_NAME} PUBLIC include)