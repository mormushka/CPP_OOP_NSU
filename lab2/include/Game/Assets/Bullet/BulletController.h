// BulletController.h
#pragma once
#include "BehaviourComponent.h"
#include "Transform.h"
#include "Movement.h"
#include "CircleCollider.h"

class BulletController : public BehaviourComponent
{
private:
    float lifeTime_ = 0.0f;
    const float maxLifeTime_ = 1.0f;
    std::shared_ptr<Transform> transform_ = nullptr;
    std::shared_ptr<Movement> movement_ = nullptr;
    
public:
    BulletController() = default;
    
    void Awake() override
    {
        auto owner = Owner().lock();
        if (!owner) return;
        
        transform_ = owner->GetComponent<Transform>();
        movement_ = owner->GetComponent<Movement>();
    }
    
    void Start() override 
    {

    }
    
    void Update(float deltaTime) override
    {
        lifeTime_ += deltaTime;
        if (lifeTime_ >= maxLifeTime_)
        {
            auto owner = Owner().lock();
            if (owner)
            {
                owner->Disable();
            }
        }
    }
    
    void OnEnable()
    {
        lifeTime_ = 0.0f;
    }
    
    float GetLifeTime() const { return lifeTime_; }
    float GetMaxLifeTime() const { return maxLifeTime_; }
};