// CollisionManager.h
#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "CircleCollider.h"
#include "Movement.h"

struct CollisionInfo
{
    std::shared_ptr<GameObject> objectA;
    std::shared_ptr<GameObject> objectB;
    CircleCollider *colliderA;
    CircleCollider *colliderB;
    Vector2 normal;
    float depth = 0.0f;
    Vector2 contactPoint;
};

class CollisionManager
{
private:
    bool isActive_ = true;
    std::vector<std::shared_ptr<GameObject>> objects_;

public:
    void AddObject(std::shared_ptr<GameObject> obj)
    {
        if (obj && obj->HasComponent<CircleCollider>())
            objects_.push_back(obj);
    }

    void AddObjects(const std::vector<std::shared_ptr<GameObject>> &objects)
    {
        for (auto obj : objects)
            AddObject(obj);
    }

    void RemoveObject(std::shared_ptr<GameObject> obj)
    {
        auto it = std::find(objects_.begin(), objects_.end(), obj);
        if (it != objects_.end())
            objects_.erase(it);
    }

    void RemoveAllObjects() { objects_.clear(); }

    bool IsEmpty() const { return objects_.empty(); }

    void setActive(bool b) { isActive_ = b; }
    bool getActive() const { return isActive_; }

    void Update(float deltaTime)
    {
        if (!isActive_)
            return;

        for (size_t i = 0; i < objects_.size(); ++i)
        {
            if (!objects_[i]->IsActive())
                continue;
            for (size_t j = i + 1; j < objects_.size(); ++j)
            {
                if (!objects_[j]->IsActive())
                    continue;
                CheckCollision(objects_[i], objects_[j]);
            }
        }
    }

private:
    bool CheckCollision(std::shared_ptr<GameObject> objA, std::shared_ptr<GameObject> objB)
    {
        auto colliderA = objA->GetComponent<CircleCollider>();
        auto colliderB = objB->GetComponent<CircleCollider>();

        if (!colliderA || !colliderB)
            return false;

        Vector2 centerA = colliderA->GetWorldCenter();
        Vector2 centerB = colliderB->GetWorldCenter();
        float radiusA = colliderA->GetWorldRadius();
        float radiusB = colliderB->GetWorldRadius();

        Vector2 delta = centerB - centerA;
        float distanceSquared = delta.x * delta.x + delta.y * delta.y;
        float radiusSum = radiusA + radiusB;
        float radiusSumSquared = radiusSum * radiusSum;

        if (distanceSquared < radiusSumSquared)
        {
            float distance = std::sqrt(distanceSquared);

            CollisionInfo info;
            info.objectA = objA;
            info.objectB = objB;
            info.colliderA = colliderA;
            info.colliderB = colliderB;

            if (distance > 0.0f)
            {
                info.normal = delta / distance;
                info.depth = radiusSum - distance;
                info.contactPoint = centerA + info.normal * (radiusA - info.depth / 2.0f);
            }
            else
            {
                // Центры совпадают
                info.normal = Vector2(1.0f, 0.0f);
                info.depth = radiusSum;
                info.contactPoint = centerA;
            }

            ResolveCollision(info);
            return true;
        }

        return false;
    }

    void ResolveCollision(const CollisionInfo &info)
    {
        if (info.colliderA->IsTrigger() || info.colliderB->IsTrigger())
        {
            info.objectA->OnTriggerEnter(info.objectB);
            info.objectB->OnTriggerEnter(info.objectA);
            return;
        }

        auto movementA = info.objectA->GetComponent<Movement>();
        auto movementB = info.objectB->GetComponent<Movement>();
        auto transformA = info.objectA->GetComponent<Transform>();
        auto transformB = info.objectB->GetComponent<Transform>();

        if (!transformA || !transformB)
            return;

        // Разделяем объекты
        Vector2 separation = info.normal * (info.depth / 2.0f);
        transformA->Translate(separation * -1);
        transformB->Translate(separation);

        if (movementA && movementB)
        {
            ResolvePhysicsCollision(info, movementA, movementB);
        }
        else if (movementA)
        {
            Vector2 velocityA = movementA->Velocity();
            Vector2 reflection = velocityA - info.normal * 2.0f * velocityA.Dot(info.normal);
            movementA->SetVelocity(reflection * 0.8f);
        }
        else if (movementB)
        {
            Vector2 velocityB = movementB->Velocity();
            Vector2 reflection = velocityB - (info.normal * -1) * 2.0f * velocityB.Dot(info.normal * -1);
            movementB->SetVelocity(reflection * 0.8f);
        }

        info.objectA->OnCollisionEnter(info.objectB);
        info.objectB->OnCollisionEnter(info.objectA);
    }

    void ResolvePhysicsCollision(const CollisionInfo &info, Movement *movementA, Movement *movementB)
    {
        float restitution = 0.8f;

        Vector2 velocityA = movementA->Velocity();
        Vector2 velocityB = movementB->Velocity();

        Vector2 relativeVelocity = velocityB - velocityA;
        float velocityAlongNormal = relativeVelocity.Dot(info.normal);

        if (velocityAlongNormal > 0)
            return;

        float massA = movementA->Mass();
        float massB = movementB->Mass();
        float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
        impulseScalar /= (1.0f / massA + 1.0f / massB);

        Vector2 impulse = info.normal * impulseScalar;

        movementA->AddImpulse(impulse * -1);
        movementB->AddImpulse(impulse);
    }
};