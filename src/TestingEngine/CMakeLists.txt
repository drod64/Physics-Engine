cmake_minimum_required(VERSION 3.27.3)

project(TestingEngine)

set(SOURCE_FILES
    src/TestingEngine.cpp
    src/Scene_Play.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

if (SFML_FOUND)
target_include_directories(${PROJECT_NAME} 
PUBLIC ${SFML_INCLUDE_DIR}
PUBLIC include)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif ()

target_link_libraries(${PROJECT_NAME} SGE)

target_include_directories(${PROJECT_NAME} PUBLIC include)