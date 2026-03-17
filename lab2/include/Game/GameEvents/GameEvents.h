#pragma once
#include "Observer.hpp"
#include <string>
#include "Vectors.h"

namespace GameEvents
{
    enum Tag
    {
        kNone,
        kSceneChange,
        kGameShutdown,
        kGameOver,
        kAsteroidDestroy,
        kShoot
    };

    struct SceneChange : public Event
    {
        std::string sceneName;
        SceneChange(const std::string &name,
                    const int &tag = kSceneChange) : Event(tag), sceneName(name) {}
    };

    struct GameShutdown : public Event
    {
        GameShutdown(const int &tag = kGameShutdown) : Event(tag) {}
    };

    struct GameOver : public Event
    {
        GameOver(const int &tag = kGameOver) : Event(tag) {}
    };

    struct AsteroidDestroy : public Event
    {
        int index;
        AsteroidDestroy(const int &i, const int &tag = kAsteroidDestroy) : Event(tag), index(i) {}
    };

    struct Shoot : public Event
    {
        Vector2 position;
        Vector2 direction;
        float rotation;

        Shoot(const int &tag = kShoot) : Event(tag) {}
        Shoot(Vector2 pos, Vector2 dir, float rot, const int &tag = kShoot)
            : Event(tag), position(pos), direction(dir), rotation(rot) {}
    };
}