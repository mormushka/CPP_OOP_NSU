// GameObject.h
#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Vectors.h"
#include "BehaviourComponent.h"
#include "Logger.hpp"

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
    std::string tag_;
    bool isActive_ = true;
    bool isDestroyed_ = false;
    bool isStarted_ = false;

    std::unordered_map<std::type_index, std::shared_ptr<Component>> components_;
    std::vector<BehaviourComponent *> behaviourComponents_;
    std::vector<std::shared_ptr<GameObject>> childrens_;
    std::weak_ptr<GameObject> parent_;

public:
    virtual ~GameObject() = default;
    GameObject(const std::string &tag = "untagged") : tag_(tag) {}

    virtual void Initialize() {};

    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        auto typeIdx = std::type_index(typeid(T));
        if (components_.find(typeIdx) != components_.end())
        {
            return static_cast<T *>(components_[typeIdx].get());
        }

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T *rawPtr = component.get();
        components_[typeIdx] = std::move(component);

        if (auto self = weak_from_this().lock())
        {
            rawPtr->SetOwner(self);
        }

        if (auto behaviourComponent = dynamic_cast<BehaviourComponent *>(rawPtr))
            behaviourComponents_.push_back(behaviourComponent);

        return rawPtr;
    }

    template <typename T>
    T *GetComponent()
    {
        auto typeIdx = std::type_index(typeid(T));
        auto it = components_.find(typeIdx);
        return (it != components_.end()) ? static_cast<T *>(it->second.get()) : nullptr;
    }

    template <typename T>
    bool HasComponent() const
    {
        return components_.find(std::type_index(typeid(T))) != components_.end();
    }

    template <typename T>
    void RemoveComponent()
    {
        auto typeIdx = std::type_index(typeid(T));
        auto it = components_.find(typeIdx);

        if (it != components_.end())
        {
            if (auto behaviourComponent = dynamic_cast<BehaviourComponent *>(it->second.get()))
            {
                behaviourComponents_.erase(
                    std::remove(
                        behaviourComponents_.begin(),
                        behaviourComponents_.end(),
                        behaviourComponent),
                    behaviourComponents_.end());
            }

            components_.erase(it);
        }
    }

    void RemoveAllComponents()
    {
        behaviourComponents_.clear();
        components_.clear();
    }

    void Destroy()
    {
        isDestroyed_ = true;
        isActive_ = false;

        for (auto &child : childrens_)
        {
            child->Destroy();
        }
        childrens_.clear();
        RemoveAllComponents();
    }

    void Disable()
    {
        isActive_ = false;
        for (auto child : childrens_)
        {
            child->Disable();
        }
        OnDisable();
    }

    void Enable()
    {
        isActive_ = true;
        if (!isStarted_)
        {
            Start();
        }
        for (auto child : childrens_)
        {
            child->Enable();
            child->OnEnable();
        }
        OnEnable();
    }

    virtual void OnDisable() {}
    virtual void OnEnable() {}

    bool IsActive() const { return isActive_ && !isDestroyed_; }
    bool IsDestroyed() const { return isDestroyed_; }
    const std::string &Tag() const { return tag_; }
    void SetTag(const std::string &tag) { tag_ = tag; }

    void AddChild(std::shared_ptr<GameObject> child)
    {
        childrens_.push_back(child);
        child->parent_ = shared_from_this();
    }

    void RemoveChild(GameObject *child)
    {
        auto it = std::find_if(childrens_.begin(), childrens_.end(),
                               [child](const std::shared_ptr<GameObject> &obj)
                               {
                                   return obj.get() == child;
                               });
        if (it != childrens_.end())
        {
            (*it)->parent_.reset();
            childrens_.erase(it);
        }
    }

    std::weak_ptr<GameObject> Parent() const { return parent_; }
    const std::vector<std::shared_ptr<GameObject>> &Children() const { return childrens_; }

    virtual void OnCollisionEnter(std::shared_ptr<GameObject> obj) {}
    virtual void OnTriggerEnter(std::shared_ptr<GameObject> obj) {}

    void Awake()
    {
        for (auto &c : behaviourComponents_)
        {
            c->Awake();
        }
        for (auto child : childrens_)
        {
            child->Awake();
        }
    }

    void Start()
    {
        if (!isActive_)
            return;
        isStarted_ = true;
        for (auto &c : behaviourComponents_)
        {
            c->Start();
        }
        for (auto child : childrens_)
        {
            child->Start();
        }
    }

    void Update(float deltaTime)
    {
        if (!isActive_)
            return;
        for (auto &c : behaviourComponents_)
        {
            c->Update(deltaTime);
        }
        for (auto child : childrens_)
        {
            child->Update(deltaTime);
        }
    }

    void Render()
    {
        if (!isActive_)
            return;
        for (auto &c : behaviourComponents_)
        {
            c->Render();
        }
        for (auto child : childrens_)
        {
            child->Render();
        }
    }
};