// IGameObject.hpp
#pragma once
#include "Logger.hpp"
#include <SFML/Graphics.hpp>

class IGameObject
{
public:
    virtual ~IGameObject() = default;
    virtual void update() = 0;
    virtual sf::FloatRect getBounds() const = 0;
};