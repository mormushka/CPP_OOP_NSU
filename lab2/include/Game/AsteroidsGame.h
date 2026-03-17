// AsteroidsGame.h
#pragma once
#include "GameSceneManager.h"
#include "BehaviourComponent.h"
#include "Logger.hpp"
#include "AudioInitializer.h"

#include "IRenderer.h"
#include "IInputEvent.h"
#include "IClock.h"
#include "IAudioManager.h"
#include "UIManager.h"
#include "CollisionManager.h"

#include "MenuScene.h"
#include "PauseScene.h"
#include "GameSceneImpl.h"
#include "SettingsScene.h"
#include "GameOverScene.h"

class AsteroidsGame : public BehaviourComponent
{
private:
    std::shared_ptr<IRenderer> renderer_;
    std::shared_ptr<IInputEvent> inputEvent_;
    std::shared_ptr<IClock> clock_;
    std::shared_ptr<IAudioManager> audio_;
    std::shared_ptr<GameSceneManager> gameSceneManager_;
    std::shared_ptr<UIManager> ui_;
    std::shared_ptr<CollisionManager> collisions_;
    float deltaTime_ = 0.0f;
    bool isRunning_ = true;

    std::shared_ptr<GameSceneImpl> game_;

public:
    AsteroidsGame(std::shared_ptr<IRenderer> r,
                  std::shared_ptr<IInputEvent> e,
                  std::shared_ptr<IClock> c,
                  std::shared_ptr<IAudioManager> a)
        : renderer_(r),
          inputEvent_(e),
          clock_(c),
          audio_(a),
          gameSceneManager_(std::make_shared<GameSceneManager>()),
          ui_(std::make_shared<UIManager>()),
          collisions_(std::make_shared<CollisionManager>())
    {
    }

    void run()
    {
        Logger::Instance().SetLogFile("game.log");
        Logger::Instance().SetLevel(LogLevel::Debug);
        LOG_INFO << "Game run...";

        Awake();
        Start();

        while (isRunning_ && renderer_->IsOpen())
        {
            ProcessEvents();
            Update(deltaTime_);
            Render();
        }

        audio_->StopAll();
        LOG_INFO << "Game shutdown";
    }

    void Awake() override
    {
        LOG_INFO << "Game Awake...";
        AudioInitializer::Initialize(*audio_);
        LOG_INFO << "Game Awaked!";
    }

    void Start() override
    {
        LOG_INFO << "Game Start...";

        InitializeScenes();

        gameSceneManager_->Awake();
        gameSceneManager_->Start();
        gameSceneManager_->SetInitialScene("Menu");
        ui_->SetActiveScene("Menu");
        audio_->Play("menu_music");
        LOG_INFO << "Game Started!";
    }

    void InitializeScenes()
    {
        LOG_INFO << "Start Initialize Scenes...";

        auto menu = std::make_shared<MenuScene>("Menu", renderer_, audio_, ui_, collisions_);
        game_ = std::make_shared<GameSceneImpl>("Game", renderer_, audio_, ui_, collisions_);
        auto pause = std::make_shared<PauseScene>("Pause", renderer_, audio_, ui_, collisions_);
        auto gameOver = std::make_shared<GameOverScene>("GameOver", renderer_, audio_, ui_, collisions_);
        auto settings = std::make_shared<SettingsScene>("Settings", renderer_, audio_, ui_, collisions_);

        gameSceneManager_->AddScene("Menu", menu);
        gameSceneManager_->AddScene("Game", game_);
        gameSceneManager_->AddScene("Pause", pause);
        gameSceneManager_->AddScene("GameOver", gameOver);
        gameSceneManager_->AddScene("Settings", settings);

        LOG_INFO << "Initialized " << gameSceneManager_->GetScenes().size() << " scenes";

        pause->AddObserver(game_->getWeakPtr());

        for (auto &[name, scene] : gameSceneManager_->GetScenes())
        {
            scene->AddObserver(shared_from_this());
        }
    }

    void ProcessEvents()
    {
        IInputEvent::IEvent event;
        while (inputEvent_->PollEvent(event))
        {
            if (event.type == IInputEvent::IEvent::Type::Closed)
            {
                renderer_->Close();
                isRunning_ = false;
            }
            if (gameSceneManager_->GetCurrentScene())
            {
                gameSceneManager_->GetCurrentScene()->HandleEvent(event);
            }
        }
    }

    void Update(float deltaTime) override
    {
        collisions_->Update(deltaTime);
        deltaTime_ = clock_->GetDeltaTime();
        gameSceneManager_->Update(deltaTime_);
        ui_->Update(deltaTime);
        audio_->Update(deltaTime_);
    }

    void Render()
    {
        renderer_->Clear(Colors::kBaseCatp);
        gameSceneManager_->Render();
        ui_->UpdateLayout(*renderer_);
        ui_->Render(*renderer_);
        renderer_->Display();
    }

    void OnNotify(const Event &event) override
    {
        if (event.tag == GameEvents::kSceneChange)
        {
            HandleSceneChange(static_cast<const GameEvents::SceneChange *>(&event));
        }
        Notify(event);
    }

    void HandleSceneChange(const GameEvents::SceneChange *sceneEvent)
    {
        if (!sceneEvent)
            return;

        std::string sceneName = sceneEvent->sceneName;
        LOG_INFO << "Changing scene to: " << sceneName;

        gameSceneManager_->ChangeScene(sceneName);
        ui_->SetActiveScene(sceneName);

        if (sceneName == "Menu")
        {
            audio_->Crossfade("menu_music", 3.0f);
        }
        else if (sceneName == "Game")
        {
            audio_->Crossfade("game_music", 3.0f);
            if (game_->GetState_() >= 2)
            {
                game_->Start();
            }
        }
        else if (sceneName == "Pause")
        {
            audio_->Crossfade("game_music", 3.0f);
        }
        else if (sceneName == "GameOver")
        {
            audio_->Crossfade("gameover_music", 0.5f);
        }
    }
};