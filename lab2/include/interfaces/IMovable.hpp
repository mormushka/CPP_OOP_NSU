// IMovable.hpp
#pragma once
#include <SFML/Graphics.hpp>

class IMovable
{
protected:
    sf::Vector2f velocity;
    float deltaTimeFactor = 1.0f; // Добавляем множитель для deltaTime

public:
    IMovable(const sf::Vector2f &vel = sf::Vector2f(0, 0)) : velocity(vel) {}
    IMovable(float velX, float velY) : velocity(velX, velY) {}
    virtual void move() = 0;
    virtual ~IMovable() = default;
    
    void setDeltaTimeFactor(float factor) { deltaTimeFactor = factor; }
    float getDeltaTimeFactor() const { return deltaTimeFactor; }
    
    sf::Vector2f getVelocity() const { return velocity; }
    void setVelocity(const sf::Vector2f &v) { velocity = v; }
};