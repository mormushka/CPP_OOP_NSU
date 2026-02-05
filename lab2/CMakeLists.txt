cmake_minimum_required(VERSION 3.16)

project(Asteroids)

set(CMAKE_CXX_STANDARD 17)

# Для Linux используем pkg-config или обычный find_package
# Убедитесь, что SFML установлен через пакетный менеджер:
# sudo pacman -S sfml

# Ищем SFML через find_package (стандартный способ для Linux)
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

# Список исходных файлов по новой структуре
set(SOURCE_FILES
    src/main.cpp
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

# Подключаем SFML библиотеки
target_link_libraries(Asteroids 
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# В Linux обычно не нужно копировать DLL, так как библиотеки находятся в системных путях
# Но если вы хотите использовать динамические библиотеки из нестандартного пути,
# можно оставить копирование .so файлов (но обычно в этом нет необходимости)

# --- автокопирование ресурсов (остается таким же для Linux) ---
# Копируем всю структуру ресурсов рекурсивно
add_custom_command(TARGET Asteroids POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:Asteroids>/resources"
    COMMENT "Copying resource directory..."
)