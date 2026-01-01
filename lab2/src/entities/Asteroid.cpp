// Asteroid.cpp
#include "Asteroid.hpp"
#include "Constants.hpp"

Asteroid::Asteroid(float radius, const sf::Vector2f& pos, const sf::Vector2f& vel) 
    : IMovable(vel) 
{
    shape.setRadius(radius);
    shape.setFillColor(GC::ASTEROID_COLOR);
    shape.setOrigin(radius, radius);
    shape.setPosition(pos);
}

void Asteroid::move() {
    shape.move(velocity * deltaTimeFactor);  // Используем deltaTimeFactor
}

void Asteroid::update() {
    move();
}

void Asteroid::draw(sf::RenderWindow& win) {
    win.draw(shape);
}

bool Asteroid::isOffScreen(const sf::RenderWindow& win) const {
    sf::Vector2f pos = shape.getPosition();
    return (pos.x < -50 || pos.x > win.getSize().x + 50 ||
            pos.y < -50 || pos.y > win.getSize().y + 50);
}