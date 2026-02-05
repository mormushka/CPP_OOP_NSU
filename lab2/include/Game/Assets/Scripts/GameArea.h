// GameArea.h
#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Player.h"
#include "Asteroid.h"

class GameArea
{
private:
    const float playerBorder_ = 15.f;
    const float asteroidBorder_ = 60.f;

    Vector2 gameZone_;
    Vector2 playerZone_;
    Vector2 asteroidZone_;
    std::shared_ptr<Player> player_;
    std::vector<std::shared_ptr<Asteroid>> asteroids_;

public:
    GameArea(Vector2 gameZoneSize,
                std::shared_ptr<Player> player,
                const std::vector<std::shared_ptr<GameObject>> &objs)
        : gameZone_(gameZoneSize),
          player_(player)
    {
        for (const auto &obj : objs)
        {
            auto asteroid = std::dynamic_pointer_cast<Asteroid>(obj);
            if (asteroid)
            {
                asteroids_.push_back(asteroid);
            }
        }
        playerZone_ = gameZone_ + playerBorder_;
        asteroidZone_ = gameZone_ + asteroidBorder_;
    }

    void Check(float deltaTime)
    {
        Transform *t;
        Vector2 newPos;
        for (const auto &a : asteroids_)
        {
            t = a->GetComponent<Transform>();
            newPos = t->LocalPosition();
            if (t->LocalPosition().x > asteroidZone_.x)
            {
                newPos.x = -asteroidBorder_;
            }
            else if (t->LocalPosition().x < -asteroidBorder_)
            {
                newPos.x = asteroidZone_.x;
            }
            if (t->LocalPosition().y > asteroidZone_.y)
            {
                newPos.y = -asteroidBorder_;
            }
            else if (t->LocalPosition().y < -asteroidBorder_)
            {
                newPos.y = asteroidZone_.y;
            }

            t->SetLocalPosition(newPos);
        }

        t = player_->GetComponent<Transform>();
        newPos = t->LocalPosition();
        if (t->LocalPosition().x > playerZone_.x)
        {
            newPos.x = -playerBorder_;
        }
        else if (t->LocalPosition().x < -playerBorder_)
        {
            newPos.x = playerZone_.x;
        }
        if (t->LocalPosition().y > playerZone_.y)
        {
            newPos.y = -playerBorder_;
        }
        else if (t->LocalPosition().y < -playerBorder_)
        {
            newPos.y = playerZone_.y;
        }
        t->SetLocalPosition(newPos);
    }
};