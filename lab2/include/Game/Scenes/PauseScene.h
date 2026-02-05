// MenuScene.h
#pragma once
#include "GameScene.h"

class PauseScene : public GameScene
{
private:
public:
    PauseScene(std::string sceneName,
               std::shared_ptr<IRenderer> renderer,
               std::shared_ptr<IAudioManager> audio,
               std::shared_ptr<UIManager> ui,
               std::shared_ptr<CollisionManager> c)
        : GameScene(sceneName, renderer, audio, ui, c)
    {
        ui_->AddObserver(weak_from_this());
    }

    void OnNotify(const Event &event) override
    {
        if (auto *clickEvent = dynamic_cast<const UIEvents::ElementClicked *>(&event))
        {
            if (clickEvent->elementId == "btn_return")
            {
                GameEvents::SceneChange sceneEvent("Game");
                Notify(sceneEvent);
            }
            else if (clickEvent->elementId == "btn_menu")
            {
                GameEvents::GameShutdown e;
                Notify(e);
                GameEvents::SceneChange sceneEvent("Menu");
                Notify(sceneEvent);
            }
        }
    }

    void HandleEvent(const IInputEvent::IEvent &event) override
    {
        ui_->HandleEvent(event);

        if (event.type == IInputEvent::IEvent::Type::KeyPressed)
        {
            if (event.keyCode == IInputEvent::IEvent::KeyCode::P)
            {
                GameEvents::SceneChange sceneEvent("Game");
                Notify(sceneEvent);
            }
        }
    }

    void Awake() override
    {
        ui_->AddObserver(weak_from_this());
    }

    void Start() override
    {
        auto label = UI_LABEL("pause_txt", 0.3f, 0.1f, 0.1f, 0.1f, "PAUSE");
        label->SetTextColor(Colors::kWhiteCatp);
        label->SetTextSize(80);

        UI_BUTTON("btn_menu", 0.3f, 0.45f, 0.4f, 0.1f, "Menu");
        UI_BUTTON("btn_return", 0.3f, 0.3f, 0.4f, 0.1f, "Return");
    }

    void Update(float deltaTime) override
    {
    }

    void Render() override
    {
    }
};
