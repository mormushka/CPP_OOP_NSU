// GameEngine.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "GameWorld.hpp"
#include "Logger.hpp"
#include "Constants.hpp"

class GameEngine
{
private:
    // Окно и мир
    sf::RenderWindow window;
    std::unique_ptr<GameWorld> world;

    // Камеры и виды
    float cameraZoom = 1.0f;
    sf::View camera;
    sf::View miniMapView;

    // Графика
    sf::VertexArray grid;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::RectangleShape miniMapBorder;

    // UI элементы
    sf::Text fpsText;
    sf::Text statsText;
    sf::Font uiFont;

    // Время и статистика
    sf::Clock gameClock;
    sf::Clock fpsClock;
    sf::Clock warningClock;
    int frameCount = 0;
    float fps = 0;
    float totalPlayTime = 0.0f;

    // Состояние
    bool isPaused = false;
    bool showDebugInfo = false;
    bool showMiniMap = true;

    // Инициализация
    void initializeWindow();
    void initializeViews();
    void initializeGrid();
    void initializeBackground();
    void initializeUI();
    void initializeTextures();

    // Обновление
    void update(float deltaTime);
    void updateCamera();
    void updateBackground();
    void updateFPS(float deltaTime);
    void updateStats();
    void checkDeadZone();

    // Зум
    void zoomCamera(float factor);
    void zoomCameraRelative(float delta);
    float getCameraZoom() const { return cameraZoom; }
    void resetCameraZoom() { cameraZoom = 1.0f; }

    // Отрисовка
    void render();
    void renderMainView();
    void renderMiniMap();
    void renderUI();

    // Обработка событий
    void processEvents();
    void handleKeyPressed(sf::Keyboard::Key key);

    // Утилиты
    sf::Color getRandomStarColor() const;
    void generateStarBackground();

    void GameEngine::handleMouseWheelScrolled(sf::Event::MouseWheelScrollEvent &event);

public:
    GameEngine();
    ~GameEngine() = default;

    // Запрет копирования
    GameEngine(const GameEngine &) = delete;
    GameEngine &operator=(const GameEngine &) = delete;

    // Основные методы
    void run();
    void togglePause();
    void toggleDebugInfo();
    void toggleMiniMap();

    // Геттеры
    bool isRunning() const { return window.isOpen(); }
    float getFPS() const { return fps; }
    float getPlayTime() const { return totalPlayTime; }
    bool getPaused() const { return isPaused; }
};