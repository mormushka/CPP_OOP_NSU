// BehaviourComponent.h - Компонент с жизненным циклом
#pragma once
#include "Component.h"
#include "Observer.hpp"
#include "GameEvents.h"

class BehaviourComponent : public Component,
                           public Observer<Event>,
                           public Subject<Event>
{
public:
    virtual ~BehaviourComponent() = default;

    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}

    virtual void OnNotify(const Event &event) override {}
};