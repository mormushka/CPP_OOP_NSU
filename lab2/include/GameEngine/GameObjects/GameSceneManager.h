// GameSceneManager.h
#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "BehaviourComponent.h"
#include "GameScene.h"

class GameSceneManager : public BehaviourComponent
{
private:
    std::unordered_map<std::string, std::shared_ptr<GameScene>> scenes_;
    GameScene *currentScene_ = nullptr;

public:
    void Awake() override
    {
        for (auto &scene : scenes_)
        {
            scene.second->Awake();
        }
    }

    void Start() override
    {
        for (auto &scene : scenes_)
        {
            scene.second->Start();
        }
    }

    void Update(float deltaTime) override
    {
        if (currentScene_)
            currentScene_->Update(deltaTime);
    }

    void Render()
    {
        if (currentScene_)
            currentScene_->Render();
    }

    void AddScene(const std::string &name, std::shared_ptr<GameScene> scene)
    {
        scenes_[name] = std::move(scene);
        LOG_INFO << "Added Scene: " << name;
    }

    GameScene *GetCurrentScene() const
    {
        return currentScene_;
    }

    const std::unordered_map<std::string, std::shared_ptr<GameScene>> &GetScenes() const
    {
        return scenes_;
    }
    
    void ChangeScene(const std::string &name)
    {
        auto it = scenes_.find(name);
        if (it != scenes_.end())
        {
            currentScene_ = it->second.get();
        }
    }

    void SetInitialScene(const std::string &name)
    {
        ChangeScene(name);
    }

    bool HasScene(const std::string &name) const
    {
        return scenes_.find(name) != scenes_.end();
    }
};