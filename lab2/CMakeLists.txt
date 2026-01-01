cmake_minimum_required(VERSION 3.16)

project(Asteroids)

set(CMAKE_CXX_STANDARD 17)

# Укажи путь к SFML
set(SFML_DIR "C:/Libs/SFML-2.6.2/lib/cmake/SFML")

# Не указываем версию, чтобы принять любую доступную
find_package(SFML COMPONENTS graphics window system REQUIRED)

# Список исходных файлов по новой структуре
set(SOURCE_FILES
    src/main.cpp
    
    # Core
    src/core/GameWorld.cpp
    src/core/Controller.cpp
    src/core/GameEngine.cpp
    
    # Entities
    src/entities/Ship.cpp
    src/entities/Asteroid.cpp
    src/entities/Bullet.cpp
)

# Проверяем существование файлов перед добавлением
foreach(source_file ${SOURCE_FILES})
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${source_file})
        message(WARNING "Source file ${source_file} not found")
    endif()
endforeach()

add_executable(Asteroids ${SOURCE_FILES})

# Включаем директории для заголовочных файлов
target_include_directories(Asteroids PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/entities
    ${CMAKE_SOURCE_DIR}/include/interfaces
    ${CMAKE_SOURCE_DIR}/include/utils
    ${CMAKE_SOURCE_DIR}/include/factories
)

target_link_libraries(Asteroids sfml-graphics sfml-window sfml-system)

# --- автокопирование DLL ---
# где лежат DLL
set(SFML_DLL_DIR "C:/Libs/SFML-2.6.2/bin")

file(GLOB SFML_DLLS "${SFML_DLL_DIR}/*.dll") 

add_custom_command(TARGET Asteroids POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    ${SFML_DLLS} 
    $<TARGET_FILE_DIR:Asteroids> 
    COMMENT "Copying all SFML DLLs..." 
)

# --- автокопирование ресурсов ---
# Копируем всю структуру ресурсов рекурсивно
add_custom_command(TARGET Asteroids POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:Asteroids>/resources"
    COMMENT "Copying resource directory..."
)
