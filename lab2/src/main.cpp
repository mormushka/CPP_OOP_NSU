// main.cpp
#include "GameEngine.hpp"
#include "Logger.hpp"

int main() {
    try {
        LOG_INFO("========================================");
        LOG_INFO("     Asteroids Game - Starting Up       ");
        LOG_INFO("========================================");
        
        // Инициализация и запуск игрового движка
        GameEngine gameEngine;
        gameEngine.run();
        
        LOG_INFO("========================================");
        LOG_INFO("      Asteroids Game - Shutting Down    ");
        LOG_INFO("========================================");
        
        return 0;
    } catch (const std::exception& e) {
        LOG_ERROR("Fatal error in main: " + std::string(e.what()));
        std::cerr << "Fatal error: " << e.what() << std::endl;
        
        #ifdef _WIN32
            MessageBoxA(nullptr, e.what(), "Asteroids - Fatal Error", MB_ICONERROR | MB_OK);
        #endif
        
        return 1;
    }
}