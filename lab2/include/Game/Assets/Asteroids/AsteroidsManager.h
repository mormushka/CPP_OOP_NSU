// AsteroidsManager.h
#pragma once
#include "IRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "ConvexShape.h"
#include "Movement.h"
#include "CircleCollider.h"
#include "Asteroid.h"
#include "AsteroidController.h"
#include "Observer.hpp"

class AsteroidsManager : public Observer<Event>, public Subject<Event>
{
private:
    std::vector<std::shared_ptr<Asteroid>> asteroids_;
    int numAsteroids_;
    int totalAsteroids_;

    int (*neighbour_)(int);

public:
    AsteroidsManager(const int &numAsteroids) : numAsteroids_(numAsteroids)
    {
        if (numAsteroids <= 0)
            numAsteroids_ = 1;
        else
            numAsteroids_ = numAsteroids;

        totalAsteroids_ = numAsteroids_ + numAsteroids_ * 2 + numAsteroids_ * 2 * 2;
        asteroids_.reserve(totalAsteroids_);

        if (numAsteroids_ % 2)
            neighbour_ = [](int x)
            {if(x % 2) return 1; else return -1; };
        else
            neighbour_ = [](int x)
            {if(x % 2) return -1; else return 1; };
    }

    virtual void OnNotify(const Event &event)
    {
        if (event.tag == GameEvents::kAsteroidDestroy)
        {
            DisableAsteroid(static_cast<const GameEvents::AsteroidDestroy *>(&event)->index);
            Notify(event);
        }
    }

    std::vector<std::shared_ptr<Asteroid>> &GetAsteroids()
    {
        return asteroids_;
    }

    void Clear()
    {
        asteroids_.clear();
    }

    void DisableAsteroid(const int &i)
    {
        asteroids_[i]->Disable();
        if (asteroids_[i]->GetComponent<AsteroidController>()->Size() == 1)
        {
            if (!asteroids_[i + neighbour_(i)]->IsActive())
            {
                int d = ((i - numAsteroids_) / 2);
                if (!asteroids_[d + neighbour_(d)]->IsActive())
                {
                    asteroids_[d - numAsteroids_]->Enable();
                    asteroids_[d - numAsteroids_]
                        ->GetComponent<Transform>()
                        ->SetLocalPosition(Vector2(-30, -30));
                    asteroids_[d - numAsteroids_]
                        ->GetComponent<Movement>()
                        ->SetVelocity(Vector2(rand() % 800 - 400,
                                              rand() % 800 - 400));
                }
            }
        }
        else
        {
            int a1 = i * 2 + numAsteroids_;
            int a2 = a1 + 1;
            asteroids_[a1]->Enable();
            asteroids_[a2]->Enable();

            asteroids_[a1]
                ->GetComponent<Transform>()
                ->SetLocalPosition(asteroids_[i]->GetComponent<Transform>()->LocalPosition());
            asteroids_[a2]
                ->GetComponent<Transform>()
                ->SetLocalPosition(asteroids_[i]->GetComponent<Transform>()->LocalPosition());
            asteroids_[a1]
                ->GetComponent<Movement>()
                ->SetVelocity(asteroids_[i]->GetComponent<Movement>()->Velocity() + Vector2(rand() % 400 - 200, rand() % 400 - 200));
            asteroids_[a2]
                ->GetComponent<Movement>()
                ->SetVelocity(asteroids_[i]->GetComponent<Movement>()->Velocity());
        }
    }

    void InitAsteroids(std::shared_ptr<IRenderer> renderer)
    {
        for (int i = 0; i < totalAsteroids_; ++i)
        {
            int size;
            bool active = true;
            if (i < numAsteroids_)
            {
                size = 3;
            }
            else if (i < numAsteroids_ + numAsteroids_ * 2)
            {
                size = 2;
                active = false;
            }
            else
            {
                size = 1;
                active = false;
            }

            std::shared_ptr<Asteroid> asteroid = std::make_shared<Asteroid>(renderer,
                                                                            size,
                                                                            i);
            asteroid->Initialize();
            asteroid->GetComponent<AsteroidController>()->AddObserver(weak_from_this());
            asteroids_.push_back(asteroid);
        }

        for (int i = 0; i < asteroids_.size(); ++i)
        {
            if (i < numAsteroids_)
            {
                asteroids_[i]->GetComponent<Movement>()->SetVelocity(Vector2(rand() % 400 - 200,
                                                                             rand() % 400 - 200));
                asteroids_[i]->GetComponent<Transform>()->SetLocalPosition(Vector2(-30, -30));
            }
            else
            {
                asteroids_[i]->Disable();
            }
        }
    }
};