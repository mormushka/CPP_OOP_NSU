// SFMLClock.h
#pragma once
#include "IClock.h"
#include <SFML/System.hpp>

class SFMLClock : public IClock
{
private:
    sf::Clock clock;

public:
    float GetDeltaTime() override
    {
        return clock.restart().asSeconds();
    }

    void Reset() override
    {
        clock.restart();
    }
};
