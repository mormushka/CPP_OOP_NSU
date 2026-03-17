// AsteroidController.h
#pragma once
#include "BehaviourComponent.h"
#include "IInputEvent.h"
#include "Transform.h"
#include <memory>

class Asteroid; 

class AsteroidController : public BehaviourComponent
{
private:
    int size_;
    int index_;
    float rotationSpeed_ = 20.f;
    int speedRandMut_ = 200;
    std::shared_ptr<Transform> transform_;

public:
    enum AsteroidState
    {
        kNormal,
        kDestroyed
    };

    AsteroidController(const int &size, const int &index) : size_(size), index_(index)
    {
        rotationSpeed_ = rotationSpeed_ / size + (rand() % (speedRandMut_ / size));
        rotationSpeed_ *= (rand() % 2) - 1 ? -1 : 1;
    }

    int Size() { return size_; }

    void Awake() override
    {
        if (!Owner().lock())
            return;

        transform_ = Owner().lock()->GetComponent<Transform>();
    }

    void Start() override {}

    void Update(float deltaTime) override
    {
        transform_->SetRotation(transform_->Rotation() + rotationSpeed_ * deltaTime);
    }

    void TakeDamage()
    {
        GameEvents::AsteroidDestroy ev(index_);
        Notify(ev);
    }
};