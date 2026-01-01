// Controller.cpp
#include "Controller.hpp"
#include <SFML/Window.hpp>

void Controller::handleInput(Ship &ship,
                             sf::RenderWindow &window,
                             std::vector<std::unique_ptr<IGameObject>> &objects)
{
    sf::Vector2f windowCenter = sf::Vector2f(window.getSize()) / 2.0f;
    sf::Vector2f direction = sf::Vector2f(sf::Mouse::getPosition(window)) - windowCenter;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0.1f)
    {
        direction /= length;

        // поворот корабля к мыши
        ship.setRotation(std::atan2(direction.y, direction.x) * 180.f / 3.14159f + 90.f);

        // ЛКМ — ускорение
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            ship.accelerate(direction);
        }

        // ПКМ — стрельба
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            if (fireClock.getElapsedTime().asSeconds() >= GC::FIRE_DELAY)
            {
                objects.push_back(std::make_unique<Bullet>(
                    ship.getPosition(),
                    ship.getVelocity(),
                    direction));
                fireClock.restart();
            }
        }
    }
}