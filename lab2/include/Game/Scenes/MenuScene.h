// MenuScene.h
#pragma once
#include "GameScene.h"

class MenuScene : public GameScene
{
private:
public:
    MenuScene(std::string sceneName,
              std::shared_ptr<IRenderer> renderer,
              std::shared_ptr<IAudioManager> audio,
              std::shared_ptr<UIManager> ui,
              std::shared_ptr<CollisionManager> c)
        : GameScene(sceneName, renderer, audio, ui, c) {}

    void OnNotify(const Event &event) override
    {
        if (auto *clickEvent = dynamic_cast<const UIEvents::ElementClicked *>(&event))
        {
            if (clickEvent->elementId == "btn_start")
            {
                GameEvents::SceneChange sceneEvent("Game");
                Notify(sceneEvent);
            }
            else if (clickEvent->elementId == "btn_exit")
            {
                renderer_->Close();
            }
            else if (clickEvent->elementId == "btn_sett")
            {
                GameEvents::SceneChange sceneEvent("Settings");
                Notify(sceneEvent);
            }
        }
    }

    void HandleEvent(const IInputEvent::IEvent &event) override
    {
        ui_->HandleEvent(event);

        if (event.type == IInputEvent::IEvent::Type::KeyPressed &&
            event.keyCode == IInputEvent::IEvent::KeyCode::Escape)
        {
            renderer_->Close();
        }
    }

    void Awake() override
    {
        ui_->AddObserver(weak_from_this());
    }

    void Start() override
    {
        auto label = UI_LABEL("main_txt", 0.2f, 0.1f, 0.1f, 0.1f, "ASTEROIDS");
        label->SetTextColor(Colors::kWhiteCatp);
        label->SetTextSize(80);

        UI_BUTTON("btn_start", 0.3f, 0.3f, 0.4f, 0.1f, "START GAME");
        UI_BUTTON("btn_exit", 0.3f, 0.45f, 0.4f, 0.1f, "EXIT");
        UI_BUTTON("btn_sett", 0.3f, 0.6f, 0.4f, 0.1f, "Settings");
    }

    void Update(float deltaTime) override
    {
    }

    void Render() override
    {
    }
};
