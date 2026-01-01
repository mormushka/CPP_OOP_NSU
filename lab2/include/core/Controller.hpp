// Controller.hpp
#pragma once
#include "Logger.hpp"
#include "Ship.hpp"
#include "Bullet.hpp"
#include "Constants.hpp"
#include <vector>
#include <memory>
#include <cmath>

class Controller
{
private:
    sf::Clock fireClock;
public:
    void handleInput(Ship &ship,
                     sf::RenderWindow &window,
                     std::vector<std::unique_ptr<IGameObject>> &objects);
};