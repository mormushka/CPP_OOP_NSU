// IDrawable.hpp
#pragma once
#include <SFML/Graphics.hpp>

class IDrawable
{
public:
    virtual void draw(sf::RenderWindow &win) = 0;
    virtual ~IDrawable() = default;
};
