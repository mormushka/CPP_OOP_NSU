// Asteroid.hpp
#pragma once
#include "IGameObject.hpp"
#include "IMovable.hpp"
#include "IDrawable.hpp"

class Asteroid : public IGameObject, public IMovable, public IDrawable
{
private:
    sf::CircleShape shape;

public:
    Asteroid(float radius, const sf::Vector2f &pos, const sf::Vector2f &vel);

    void move() override;
    void update() override;
    void draw(sf::RenderWindow &win) override;
    bool isOffScreen(const sf::RenderWindow &win) const;
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }
};
