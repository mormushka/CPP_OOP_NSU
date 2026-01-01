// Ship.hpp
#pragma once
#include "IGameObject.hpp"
#include "IMovable.hpp"
#include "IDrawable.hpp"

class Ship : public IGameObject, public IMovable, public IDrawable
{
private:
    sf::ConvexShape shape;
    float acceleration;
    float maxSpeed;

public:
    Ship();

    void move() override;
    void update() override;
    void draw(sf::RenderWindow &win) override;
    sf::FloatRect getBounds() const override { return shape.getGlobalBounds(); }

    // Позиция
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    void setPosition(const sf::Vector2f &pos) { shape.setPosition(pos); }

    // Угол поворота
    float getRotation() const { return shape.getRotation(); }
    void setRotation(float angle) { shape.setRotation(angle); }

    void accelerate(const sf::Vector2f &dir); // Добавляем deltaTime
};