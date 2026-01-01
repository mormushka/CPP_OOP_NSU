// Bullet.cpp
#include "Bullet.hpp"
#include "Constants.hpp"

Bullet::Bullet(const sf::Vector2f &pos, const sf::Vector2f &startVel, const sf::Vector2f &dir)
    : IMovable(startVel + (dir * GC::BULLET_SPEED_MULTIPLIER))
{
    shape.setRadius(GC::BULLET_RADIUS);
    shape.setFillColor(GC::BULLET_COLOR);
    shape.setOrigin(GC::BULLET_RADIUS, GC::BULLET_RADIUS);
    shape.setPosition(pos);
    // LOG_INFO("BULLET POS: " + std::to_string(shape.getPosition().x) + " | " + std::to_string(shape.getPosition().y));
}
void Bullet::move()
{
    shape.move(velocity * deltaTimeFactor);
}

void Bullet::update()
{
    move();
}

void Bullet::draw(sf::RenderWindow &win)
{
    win.draw(shape);
}

bool Bullet::isOffScreen(const sf::RenderWindow &win) const
{
    sf::Vector2f pos = shape.getPosition();
    return (pos.x < 0 || pos.x > win.getSize().x ||
            pos.y < 0 || pos.y > win.getSize().y);
}