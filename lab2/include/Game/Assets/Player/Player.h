// Player.h
#pragma once
#include "IRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "ConvexShape.h"
#include "Movement.h"
#include "PlayerController.h"
#include "CircleCollider.h"

class Player : public GameObject
{
private:
    std::weak_ptr<IRenderer> renderer_;

public:
    Player(std::weak_ptr<IRenderer> renderer) : renderer_(renderer)
    {
        this->SetTag("player");
    }

    void Initialize() override
    {
        auto transform = AddComponent<Transform>();
        transform->SetLocalPosition(Vector2(400, 400));

        auto movement = AddComponent<Movement>();
        movement->SetMass(10);
        movement->SetMaxSpeed(400);

        AddComponent<PlayerController>();

        auto collider = AddComponent<CircleCollider>();
        collider->SetRadius(15.0f);
        collider->SetIsTrigger(false);

        std::vector<Vector2> points = {
            Vector2(0, -15),
            Vector2(10, 10),
            Vector2(0, 5),
            Vector2(-10, 10)};

        auto convex = Convex(points);
        auto color = Colors::kYellowCatp;
        convex.outlinecolor = color;
        color.a = 100;
        convex.fillColor = color;

        AddComponent<ConvexShape>(convex, renderer_);
    }

    void OnCollisionEnter(std::shared_ptr<GameObject> obj) override
    {
        if (obj->Tag() == "asteroid")
        {
            this->GetComponent<PlayerController>()->TakeDamage();
        }
    }
};