cmake_minimum_required(VERSION 3.20)

project(SGE)

set(SOURCE_FILES
    # core/ecs/components
    src/core/ecs/components/CCamera3.cpp
    src/core/ecs/components/CLifespan.cpp
    src/core/ecs/components/CScore.cpp
    src/core/ecs/components/CShape.cpp
    src/core/ecs/components/CTransform.cpp
    src/core/ecs/components/CTransform3.cpp

    # core/globalContexts/contexts
    src/core/globalContexts/contexts/InputMappingContext.cpp

    # core
    src/core/GameEngine.cpp
    src/core/Scene.cpp
    src/core/World.cpp

    # managers
    src/managers/EntityManager.cpp

    # physics/constraints
    src/physics/constraints/ConstraintManager.cpp

    # physics/forceComponents
    src/physics/forceComponents/CRigidBody3.cpp


    # systems/physics
    src/systems/physics/AnchorBungeeSystem3.cpp
    src/systems/physics/AnchorSpringSystem3.cpp
    src/systems/physics/BungeeSpringSystem3.cpp
    src/systems/physics/BuoyancySystem3.cpp
    src/systems/physics/ClearAccumulatorsSystem3.cpp
    src/systems/physics/DragSystem3.cpp
    src/systems/physics/GravitySystem3.cpp
    src/systems/physics/IntegrationSystem3.cpp
    src/systems/physics/SpringSystem3.cpp

    # systems
    src/systems/LifespanSystem.cpp
    src/systems/PollInputSystem.cpp
    src/systems/RenderingSystem3.cpp
    src/systems/SystemManager.cpp

    )

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} raylib)

# Adding the custom Math Library
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/sm/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/sm)
target_link_libraries(${PROJECT_NAME} SM)

target_include_directories(${PROJECT_NAME} PUBLIC include)