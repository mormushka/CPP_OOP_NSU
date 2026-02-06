cmake_minimum_required(VERSION 3.16)

project(Asteroids)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

set(SOURCE_FILES
    src/main.cpp
)

foreach(source_file ${SOURCE_FILES})
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${source_file})
        message(WARNING "Source file ${source_file} not found")
    endif()
endforeach()

add_executable(Asteroids ${SOURCE_FILES})

target_include_directories(Asteroids PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/GameEngine/Core
    ${CMAKE_SOURCE_DIR}/include/GameEngine/Core/SFML
    ${CMAKE_SOURCE_DIR}/include/GameEngine/Core/FAKE
    ${CMAKE_SOURCE_DIR}/include/GameEngine/Core/Utils
    ${CMAKE_SOURCE_DIR}/include/GameEngine/GameObjects
    ${CMAKE_SOURCE_DIR}/include/GameEngine/GameObjects/Components
    ${CMAKE_SOURCE_DIR}/include/GameEngine/GameObjects/BehaviourComponents
    ${CMAKE_SOURCE_DIR}/include/GameEngine/GameObjects/Base
    ${CMAKE_SOURCE_DIR}/include/GameEngine/UI
    ${CMAKE_SOURCE_DIR}/include/Game
    ${CMAKE_SOURCE_DIR}/include/Game/Scenes
    ${CMAKE_SOURCE_DIR}/include/Game/GameEvents
    ${CMAKE_SOURCE_DIR}/include/Game/Assets
    ${CMAKE_SOURCE_DIR}/include/Game/Assets/Scripts
    ${CMAKE_SOURCE_DIR}/include/Game/Assets/Player
    ${CMAKE_SOURCE_DIR}/include/Game/Assets/Asteroids
    ${CMAKE_SOURCE_DIR}/include/Game/Assets/Bullet
)

target_link_libraries(Asteroids 
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

add_custom_command(TARGET Asteroids POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:Asteroids>/resources"
    COMMENT "Copying resource directory..."
)