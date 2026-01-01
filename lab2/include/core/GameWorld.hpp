// GameWorld.hpp
#pragma once
#include "Logger.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "IGameObject.hpp"
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "Bullet.hpp"
#include "Controller.hpp"
#include "Constants.hpp"
#include "SFMLGameObjectFactory.hpp"

class GameWorld {
private:
    bool isGameOver;      // флаг конца игры
    sf::Text gameOverText; // текст Game Over
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<IGameObject>> objects;
    Controller controller;
    SFMLGameObjectFactory factory;

    int score;                // счёт игрока
    sf::Font font;            // шрифт для текста
    sf::Text scoreText;       // текст для отображения счёта

public:
    GameWorld(sf::RenderWindow& win);

    void setDeltaTimeForAllObjects(float deltaTime);
    void update();
    void draw();
    void handleCollisions();
    void spawnAsteroidsIfNeeded();
    void reset();
    bool isObjectOffScreen(const IGameObject* obj) const;
    const sf::Font& getFont() const { return font; }

    Ship* getShip();
};

