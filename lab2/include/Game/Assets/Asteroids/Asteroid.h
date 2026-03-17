// Asteroid.h
#pragma once
#include "IRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "ConvexShape.h"
#include "Movement.h"
#include "CircleCollider.h"
#include "AsteroidController.h"

class Asteroid : public GameObject
{
private:
    int index_;
    int size_; // 1-3
    int baseRadius_ = 30;
    int baseNumPoints = 10;
    std::shared_ptr<IRenderer> renderer_;

public:
    Asteroid(std::shared_ptr<IRenderer> renderer,
             const int &size,
             const int &index)
        : renderer_(renderer),
          index_(index)
    {
        this->SetTag("asteroid");
        size_ = std::clamp(size, 1, 3);
    }

    int Index() { return index_; }

    void Initialize() override
    {
        auto transform = AddComponent<Transform>();
        transform->SetScale(Vector2(0.5f * size_, 0.5f * size_));

        auto movement = AddComponent<Movement>();
        movement->SetMass(2.5f * size_);
        movement->SetMaxSpeed(300);

        std::vector<Vector2> points;
        int numPoints = baseNumPoints + rand() % (size_ * 3);
        float angleStep = 360.0f / numPoints;
        for (int i = 0; i < numPoints; i++)
        {
            float angle = i * angleStep;
            float rad = angle * 3.14159f / 180.0f;
            float distance = baseRadius_ - (rand() % (int)(baseRadius_ / 2));
            points.push_back(Vector2(
                std::cos(rad) * distance,
                std::sin(rad) * distance));
        }

        auto convex = Convex(points);
        auto color = Colors::kGreenCatp;
        convex.outlinecolor = color;
        color.a = 20;
        convex.fillColor = color;

        AddComponent<ConvexShape>(convex, renderer_);

        AddComponent<AsteroidController>(size_, index_);

        auto collider = AddComponent<CircleCollider>();
        collider->SetRadius(baseRadius_ - baseRadius_ / 4);
    }

    void OnEnable() override
    {
        
    }

    void OnCollisionEnter(std::shared_ptr<GameObject> obj) override
    {
    }
};