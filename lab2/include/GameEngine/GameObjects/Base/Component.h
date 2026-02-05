// Component.h
#pragma once
#include <string>

class GameObject;

class Component
{
protected:
    std::weak_ptr<GameObject> owner_;

public:
    virtual ~Component() = default;

    void SetOwner(std::shared_ptr<GameObject> owner) { owner_ = owner; }
    std::weak_ptr<GameObject> Owner() const { return owner_; }
};