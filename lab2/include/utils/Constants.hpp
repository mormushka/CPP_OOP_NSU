// Constants.hpp
#pragma once
#include <SFML/System/Vector2.hpp>

namespace GameConstants
{
    constexpr float DEFAULT_CAMERA_ZOOM = 1.0f;
    constexpr float MIN_CAMERA_ZOOM = 1.0f; // Максимальное приближение
    constexpr float MAX_CAMERA_ZOOM = 3.0f; // Максимальное отдаление
    constexpr float ZOOM_STEP = 0.2f;       // Шаг изменения зума

    constexpr float MAX_DELTA_TIME = 0.1f;  // Максимальный deltaTime для предотвращения спайков
    // Размеры карты
    constexpr float MAP_SIZE = 10000.0f;
    constexpr float HALF_MAP_SIZE = MAP_SIZE / 2.0f;
    constexpr float OFFSCREEN_MARGIN = 100.0f;
    constexpr float DEAD_ZONE_SIZE = MAP_SIZE / 2.0f; // Зона смерти (красная сетка)

    // Размеры окна
    constexpr unsigned int WINDOW_WIDTH = 800;
    constexpr unsigned int WINDOW_HEIGHT = 800;
    constexpr unsigned int FPS_LIMIT = 144;

    // Размеры мини-карты
    constexpr float MINIMAP_VIEWPORT_X = 0.75f;
    constexpr float MINIMAP_VIEWPORT_Y = 0.75f;
    constexpr float MINIMAP_VIEWPORT_WIDTH = 0.25f;
    constexpr float MINIMAP_VIEWPORT_HEIGHT = 0.25f;
    constexpr float MINIMAP_BORDER_SIZE = 200.0f;
    constexpr float MINIMAP_BORDER_THICKNESS = 2.0f;

    // Фон-сетка
    constexpr int GRID_CELL_SIZE = 300;
    const sf::Color GRID_COLOR = {50, 50, 100};         // Основной цвет сетки
    const sf::Color GRID_DEAD_ZONE_COLOR = {255, 0, 0}; // Цвет зоны смерти

    // Текстура фона
    constexpr unsigned int BACKGROUND_TEXTURE_SIZE = 3000;
    constexpr float BACKGROUND_PARALLAX_FACTOR = 2.2f; // Коэффициент параллакса

    // Корабль
    constexpr float SHIP_ACCELERATION = 600.0f;
    constexpr float SHIP_MAX_SPEED = 600.0f;
    constexpr float SHIP_SIZE = 40.0f; // Высота треугольника корабля
    const sf::Color SHIP_COLOR = sf::Color::Cyan;

    // Пуля
    constexpr float BULLET_RADIUS = 5.0f;
    constexpr float BULLET_SPEED_MULTIPLIER = 3000.0f; // Множитель скорости пули
    const sf::Color BULLET_COLOR = sf::Color::Cyan;

    // Астероиды
    constexpr float ASTEROID_LARGE_RADIUS = 40.0f;
    constexpr float ASTEROID_MEDIUM_RADIUS = 25.0f;
    constexpr float ASTEROID_SMALL_RADIUS = 20.0f;
    constexpr float ASTEROID_SPLIT_THRESHOLD = 30.0f; // Порог для разбиения
    const sf::Color ASTEROID_COLOR = sf::Color::White;

    // Игровая логика
    constexpr int MAX_ASTEROIDS = 2000;
    constexpr int ASTEROID_SPAWN_CHANCE = 3; // 3% каждый кадр
    constexpr int SCORE_INCREMENT = 10;
    constexpr float FIRE_DELAY = 0.1f; // Задержка между выстрелами

    // Начальные позиции объектов
    const sf::Vector2f SHIP_START_POSITION = {0.0f, 0.0f};
    const sf::Vector2f ASTEROID_1_POSITION = {-4000.0f, -4000.0f};
    const sf::Vector2f ASTEROID_1_VELOCITY = {400.0f, 400.0f};
    const sf::Vector2f ASTEROID_2_POSITION = {4000.0f, 400.0f};
    const sf::Vector2f ASTEROID_2_VELOCITY = {-200.0f, 120.0f};

    // Разлет после разбиения
    const float ASTEROID_SPLIT_PARAMETR = 100;

    // Текст
    constexpr unsigned int SCORE_TEXT_SIZE = 24;
    constexpr unsigned int GAME_OVER_TEXT_SIZE = 48;
    constexpr unsigned int FINAL_SCORE_TEXT_SIZE = 32;
    const sf::Color SCORE_TEXT_COLOR = sf::Color::White;
    const sf::Color GAME_OVER_TEXT_COLOR = sf::Color::Red;
    const sf::Color FINAL_SCORE_TEXT_COLOR = sf::Color::Yellow;

    const sf::Vector2f SCORE_TEXT_POSITION = {10.0f, 10.0f};
    const sf::Vector2f GAME_OVER_TEXT_POSITION = {200.0f, 250.0f};
    const sf::Vector2f FINAL_SCORE_TEXT_POSITION = {220.0f, 320.0f};

    // Шрифты
    constexpr const char *FONT_FILE = "resources/fonts/8bit.ttf";

    // Клавиши управления
    constexpr int RESET_KEY = sf::Keyboard::R;

    // Случайная генерация астероидов
    constexpr int ASTEROID_VELOCITY_MIN = -200;
    constexpr int ASTEROID_VELOCITY_MAX = 300;
    constexpr float ASTEROID_SPAWN_MARGIN = 100.0f;    // Отступ от края карты
    constexpr float ASTEROID_MIN_SPAWN_SPEED = 200.0f; // Минимальная скорость
    constexpr float ASTEROID_MAX_SPAWN_SPEED = 300.0f; // Максимальная скорость

    // Цвета для заднего фона (случайная генерация)
    const sf::Color BACKGROUND_COLOR_MIN = {0, 0, 0};
    const sf::Color BACKGROUND_COLOR_MAX = {50, 25, 50};
}

// Алиасы для удобства
namespace GC = GameConstants;