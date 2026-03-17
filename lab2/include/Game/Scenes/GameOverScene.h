// GameOverScene.h
#pragma once
#include "GameScene.h"

class GameOverScene : public GameScene
{
private:
    int finalScore;

public:
    GameOverScene(std::string sceneName,
                  std::shared_ptr<IRenderer> renderer,
                  std::shared_ptr<IAudioManager> audio,
                  std::shared_ptr<UIManager> ui,
                  std::shared_ptr<CollisionManager> c)
        : GameScene(sceneName, renderer, audio, ui, c)
    {
    }

    void OnNotify(const Event &event) override
    {
        if (auto *clickEvent = dynamic_cast<const UIEvents::ElementClicked *>(&event))
        {
            if (clickEvent->elementId == "restart_btn")
            {
                GameEvents::SceneChange event("Game");
                Notify(event);
            }
            else if (clickEvent->elementId == "menu_btn")
            {
                GameEvents::SceneChange event("Menu");
                Notify(event);
            }
            else if (clickEvent->elementId == "exit_btn")
            {
                renderer_->Close();
            }
        }
    }

    void HandleEvent(const IInputEvent::IEvent &event) override
    {
        ui_->HandleEvent(event);

        if (event.type == IInputEvent::IEvent::Type::KeyPressed)
        {
            if (event.keyCode == IInputEvent::IEvent::KeyCode::Escape)
            {
                GameEvents::SceneChange evt("Menu");
                Notify(evt);
            }
            else if (event.keyCode == IInputEvent::IEvent::KeyCode::R)
            {
                GameEvents::SceneChange evt("Game");
                Notify(evt);
            }
        }
    }

    void Awake() override
    {
        ui_->AddObserver(weak_from_this());
    }
    
    void Start() override
    {
        auto label = UI_LABEL("game_over_txt", 0.2f, 0.1f, 0.1f, 0.1f, "GAME OVER");
        label->SetTextColor(Colors::kRedCatp);
        label->SetTextSize(80);

        UI_BUTTON("restart_btn", 0.35f, 0.45f, 0.3f, 0.12f, "PLAY AGAIN");
        UI_BUTTON("menu_btn", 0.35f, 0.6f, 0.3f, 0.12f, "MAIN MENU");
        UI_BUTTON("exit_btn", 0.35f, 0.75f, 0.3f, 0.12f, "QUIT GAME");
    }

    void SetFinalScore(int score)
    {
        finalScore = score;
    }

    void Update(float deltaTime) override
    {
    }

    void Render() override
    {
    }
};