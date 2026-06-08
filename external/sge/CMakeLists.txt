cmake_minimum_required(VERSION 3.27.3)

project(SGE)

set(SOURCE_FILES
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
    src/entities/EntityManager.cpp

    # Systems
    src/systems/physics/AnchorBungee3.cpp
    src/systems/physics/AnchorSpring3.cpp
    src/systems/physics/BungeeSpring3.cpp
    src/systems/physics/BuoyancyForce3.cpp
    src/systems/physics/DragForce3.cpp
    src/systems/physics/FakeStiffSpring3.cpp
    include/SGE/systems/physics/ForceGenerator3.h
    src/systems/physics/ForceRegistry3.cpp
    src/systems/physics/GravityForce3.cpp
    src/systems/physics/SpringForce3.cpp

    # Utility
    src/util/Action.cpp
    include/SGE/util/ActionType.h
    src/util/Bitmask.cpp
    src/util/GameEngine.cpp
    src/util/InputSnapshot.cpp
    src/util/Scene.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} raylib)

# Adding the custom Math Library
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/sm/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/sm)
target_link_libraries(${PROJECT_NAME} SM)

target_include_directories(${PROJECT_NAME} PUBLIC include)