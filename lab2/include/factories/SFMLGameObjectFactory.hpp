// SFMLGameObjectFactory.hpp
#pragma once
#include "IGameObjectFactory.hpp"
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "Bullet.hpp"

class SFMLGameObjectFactory : public IGameObjectFactory
{
public:
    std::unique_ptr<IGameObject> createShip() override
    {
        return std::make_unique<Ship>();
    }

    std::unique_ptr<IGameObject> createAsteroid(float radius,
                                                const sf::Vector2f &pos,
                                                const sf::Vector2f &vel) override
    {
        return std::make_unique<Asteroid>(radius, pos, vel);
    }

    std::unique_ptr<IGameObject> createBullet(const sf::Vector2f &pos,
                                              const sf::Vector2f &startVel,
                                              const sf::Vector2f &dir) override
    {
        return std::make_unique<Bullet>(pos, startVel, dir);
    }
};
