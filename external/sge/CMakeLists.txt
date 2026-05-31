cmake_minimum_required(VERSION 3.27.3)

project(SGE)

set(SOURCE_FILES
    # Utility
    src/util/Action.cpp
    include/SGE/util/ActionType.h
    src/util/Bitmask.cpp
    src/util/GameEngine.cpp
    src/util/InputSnapshot.cpp
    src/util/Scene.cpp

    # Components
    src/components/CBoundingBox.cpp
    src/components/CCollision.cpp
    src/components/CInput.cpp
    src/components/CLifespan.cpp
    src/components/Component.cpp
    src/components/CRigidBody3.cpp
    src/components/CScore.cpp
    src/components/CShape.cpp
    src/components/CTransform.cpp
    src/components/CTransform3.cpp
    
    # Entities
    src/entities/Entity.cpp
    src/entities/EntityManager.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} raylib)

# Adding the custom Math Library
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/sm/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/sm)
target_link_libraries(${PROJECT_NAME} SM)

target_include_directories(${PROJECT_NAME} PUBLIC include)