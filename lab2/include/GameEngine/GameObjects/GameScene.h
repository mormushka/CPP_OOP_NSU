// GameScene.h
#pragma once
#include "GameEvents.h"
#include "BehaviourComponent.h"
#include "IRenderer.h"
#include "IAudioManager.h"
#include "IInputEvent.h"
#include "UIManager.h"
#include "CollisionManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "ConvexShape.h"

class GameScene : public BehaviourComponent
{
protected:
    std::string sceneName_;
    std::shared_ptr<IRenderer> renderer_;
    std::shared_ptr<IAudioManager> audio_;
    std::shared_ptr<UIManager> ui_;
    std::shared_ptr<CollisionManager> collisions_;

    std::vector<std::shared_ptr<GameObject>> objects_;

public:
    GameScene(std::string &sceneName,
              std::shared_ptr<IRenderer> r,
              std::shared_ptr<IAudioManager> a,
              std::shared_ptr<UIManager> u,
              std::shared_ptr<CollisionManager> p)
        : sceneName_(sceneName),
          renderer_(r),
          audio_(a),
          ui_(u),
          collisions_(p) {}

    virtual ~GameScene() = default;

    void Awake() override {};
    void Start() override {};
    void Update(float deltaTime) override {};
    virtual void Render() {};
    virtual void HandleEvent(const IInputEvent::IEvent &event) {}

    void AwakeObjects()
    {
        for (auto &obj : objects_)
        {
            obj->Awake();
        }
    };

    void StartObjects()
    {
        for (auto &obj : objects_)
        {
            obj->Start();
        }
    };

    void UpdateObjects(float deltaTime)
    {
        for (auto &obj : objects_)
        {
            obj->Update(deltaTime);
        }
    };

    void RenderObjects()
    {
        for (auto &obj : objects_)
        {
            obj->Render();
        }
    };
};

// Макрос для Label
#define UI_LABEL(id, posX, posY, sizeX, sizeY, text) \
    ui_->CreateElement<Label>(sceneName_, id, {posX, posY}, {sizeX, sizeY}, text)

// Макрос для Button
#define UI_BUTTON(id, posX, posY, sizeX, sizeY, text) \
    ui_->CreateElement<Button>(sceneName_, id, {posX, posY}, {sizeX, sizeY}, text)

// Макрос для Slider
#define UI_SLIDER(id, posX, posY, sizeX, sizeY, text, initVal) \
    ui_->CreateSlider(sceneName_, id, {posX, posY}, {sizeX, sizeY}, text, initVal)
