// Bullet.h
#pragma once
#include "IRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "ConvexShape.h"
#include "Movement.h"
#include "CircleCollider.h"
#include "AsteroidController.h"
#include "BulletController.h"

class Bullet : public GameObject
{
private:
    std::weak_ptr<IRenderer> renderer_;
    float maxSpeed_ = 900;

public:
    Bullet(std::shared_ptr<IRenderer> renderer) : renderer_(renderer)
    {
        this->SetTag("bullet");
    }

    void Initialize() override
    {
        auto transform = AddComponent<Transform>();
        auto movement = AddComponent<Movement>();
        movement->SetMass(1.0f);
        movement->SetMaxSpeed(maxSpeed_);
        movement->SetDrag(0.0f);

        auto collider = AddComponent<CircleCollider>();
        collider->SetRadius(10.0f);
        collider->SetIsTrigger(true);

        std::vector<Vector2> points = {
            Vector2(0, -6),
            Vector2(4, 4),
            Vector2(0, 2),
            Vector2(-4, 4)};

        auto convex = Convex(points);
        convex.fillColor = Colors::kTransparent;
        convex.outlinecolor = Colors::kRedCatp;

        auto shape = AddComponent<ConvexShape>(convex, renderer_);

        AddComponent<BulletController>();
    }

    void OnTriggerEnter(std::shared_ptr<GameObject> obj) override
    {
        if (obj->Tag() == "asteroid")
        {
            auto asteroidController = obj->GetComponent<AsteroidController>();
            if (asteroidController)
            {
                asteroidController->TakeDamage();
                this->Disable();
            }
        }
    }

    void OnEnable() override
    {
        auto controller = GetComponent<BulletController>();
        if (controller)
        {
            controller->OnEnable();
        }
    }
};