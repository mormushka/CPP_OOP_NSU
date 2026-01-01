// IGameObjectFactory.hpp
#pragma once
#include "Logger.hpp"
#include <memory>
#include "IGameObject.hpp"

class IGameObjectFactory
{
public:
    virtual std::unique_ptr<IGameObject> createShip() = 0;
    virtual std::unique_ptr<IGameObject> createAsteroid(float radius,
                                                        const sf::Vector2f &pos,
                                                        const sf::Vector2f &vel) = 0;
    virtual std::unique_ptr<IGameObject> createBullet(const sf::Vector2f &pos,
                                                      const sf::Vector2f &startVel,
                                                      const sf::Vector2f &dir) = 0;

    virtual ~IGameObjectFactory() = default;
};
