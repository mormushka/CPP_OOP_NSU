// MenuScene.h
#pragma once
#include "GameScene.h"

class SettingsScene : public GameScene
{
private:
public:
    SettingsScene(std::string sceneName,
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
            if (clickEvent->elementId == "btn_exit_menu")
            {
                GameEvents::SceneChange sceneEvent("Menu");
                Notify(sceneEvent);
            }
        }
        else if (auto *sliderEvent = dynamic_cast<const UIEvents::SliderValueChanged *>(&event))
        {
            ChangeVolume(*sliderEvent);
        }
        Notify(event);
    }

    void HandleEvent(const IInputEvent::IEvent &event) override
    {
        ui_->HandleEvent(event);
    }

    void Awake() override
    {
        ui_->AddObserver(weak_from_this());
    }

    void Start() override
    {
        auto label = UI_LABEL("settings_txt", 0.2f, 0.1f, 0.1f, 0.1f, "Settings");
        label->SetTextColor(Colors::kWhiteCatp);
        label->SetTextSize(80);

        UI_SLIDER("master_volume", 0.3f, 0.3f, 0.3f, 0.05f, "Master volume", (audio_->GetMasterVolume() / 100.f));
        UI_SLIDER("music_volume", 0.3f, 0.4f, 0.3f, 0.05f, "Music volume", (audio_->GetMusicVolume() / 100.f));
        UI_SLIDER("effects_volume", 0.3f, 0.5f, 0.3f, 0.05f, "Effects volume", (audio_->GetEffectsVolume() / 100.f));

        UI_BUTTON("btn_exit_menu", 0.3f, 0.8f, 0.4f, 0.1f, "EXIT");
    }

    void ChangeVolume(const UIEvents::SliderValueChanged &sliderEvent)
    {
        if (sliderEvent.elementId == "master_volume")
            audio_->SetMasterVolume(sliderEvent.value * 100.f);
        if (sliderEvent.elementId == "music_volume")
            audio_->SetMusicVolume(sliderEvent.value * 100.f);
        if (sliderEvent.elementId == "effects_volume")
            audio_->SetEffectsVolume(sliderEvent.value * 100.f);
    }
};