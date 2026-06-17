cmake_minimum_required(VERSION 3.27.3)

project(SGE)

set(SOURCE_FILES
    # Components
    src/components/CAnchorBungee3.cpp
    src/components/CAnchorSpring3.cpp
    src/components/CBoundingBox.cpp
    src/components/CBungee3.cpp
    src/components/CBuoyancy3.cpp
    src/components/CCollision.cpp
    src/components/CDrag3.cpp
    src/components/CFakeStiffSpring3.cpp
    src/components/CGravity3.cpp
    src/components/CInput.cpp
    src/components/CLifespan.cpp
    src/components/CRigidBody3.cpp
    src/components/CScore.cpp
    src/components/CShape.cpp
    src/components/CSpring3.cpp
    src/components/CTransform.cpp
    src/components/CTransform3.cpp

    # Core
    src/core/GameEngine.cpp
    src/core/InputSnapshot.cpp
    src/core/Scene.cpp
    src/core/World.cpp

    # Entities
    src/entities/EntityManager.cpp

    # Systems
    # src/systems/physics/Cable3.cpp
    # src/systems/physics/Contact3.cpp
    # src/systems/physics/ContactResolver3.cpp
    # src/systems/physics/Link3.cpp
    # src/systems/physics/Rod3.cpp
    src/systems/physics/PhysicsWorld3.cpp

    # Utility
    src/util/Action.cpp
    src/util/Bitmask.cpp
    )

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} raylib)

# Adding the custom Math Library
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/sm/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/sm)
target_link_libraries(${PROJECT_NAME} SM)

target_include_directories(${PROJECT_NAME} PUBLIC include)