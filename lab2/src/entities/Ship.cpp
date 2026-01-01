#include "Ship.hpp"
#include <cmath>
#include "Constants.hpp"

Ship::Ship() : IMovable(0.f, 0.f), acceleration(GC::SHIP_ACCELERATION), maxSpeed(GC::SHIP_MAX_SPEED)
{
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(0.f, -GC::SHIP_SIZE));
    shape.setPoint(1, sf::Vector2f(-GC::SHIP_SIZE/2, GC::SHIP_SIZE/2));
    shape.setPoint(2, sf::Vector2f(GC::SHIP_SIZE/2, GC::SHIP_SIZE/2));
    shape.setFillColor(GC::SHIP_COLOR);
    shape.setPosition(GC::SHIP_START_POSITION);
}

void Ship::move() {
    // Умножаем скорость на deltaTimeFactor
    shape.move(velocity * deltaTimeFactor);
}

void Ship::update()
{
    move();
}

void Ship::draw(sf::RenderWindow &win)
{
    win.draw(shape);
}

void Ship::accelerate(const sf::Vector2f &dir)
{
    velocity += dir * acceleration * deltaTimeFactor;
    float vLen = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (vLen > maxSpeed)
    {
        velocity = (velocity / vLen) * maxSpeed;
    }
}