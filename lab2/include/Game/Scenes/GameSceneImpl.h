// GameSceneImpl.h
#pragma once
#include "GameScene.h"
#include "Player.h"
#include "Asteroid.h"
#include "GameArea.h"
#include "AsteroidsManager.h"
#include "BulletManager.h"

enum GameState
{
    kPlay,
    kPayse,
    kGameOver,
    kShutdown
};

class GameSceneImpl : public GameScene
{
private:
    std::shared_ptr<Player> player_;
    std::shared_ptr<GameArea> gameArea_;
    std::shared_ptr<AsteroidsManager> asteroidsManager_;
    std::shared_ptr<BulletManager> bulletManager_;
    int numAsteroids_ = 10;

    int score_ = 0;

    int gameState_;
    float gameOverCooldown_;
    const float kGameOverCooldown_ = 3.f;

    std::shared_ptr<Label> labelScore_;
    std::shared_ptr<Label> labelLives_;

public:
    GameSceneImpl(std::string sceneName,
                  std::shared_ptr<IRenderer> renderer,
                  std::shared_ptr<IAudioManager> audio,
                  std::shared_ptr<UIManager> ui,
                  std::shared_ptr<CollisionManager> c)
        : GameScene(sceneName, renderer, audio, ui, c)
    {
    }

    int GetState_() { return gameState_; }

    void OnNotify(const Event &event) override
    {
        if (event.tag == GameEvents::kGameOver)
        {
            OnGameOver();
        }
        else if (event.tag == GameEvents::kGameShutdown)
        {
            OnGameShutdown();
        }
        else if (event.tag == GameEvents::kAsteroidDestroy)
        {
            ScoreUp();
        }
        Notify(event);
    }

    void HandleEvent(const IInputEvent::IEvent &event) override
    {
        if (gameState_ != kPlay)
        {
            ui_->HandleEvent(event);
            Notify(event);
            return;
        }
        if (event.type == IInputEvent::IEvent::Type::KeyPressed)
        {
            if (event.keyCode == IInputEvent::IEvent::KeyCode::P)
            {
                GameEvents::SceneChange ev("Pause");
                Notify(ev);
            }
            else if (event.keyCode == IInputEvent::IEvent::KeyCode::R)
            {
                Start();
            }
            else if (event.keyCode == IInputEvent::IEvent::KeyCode::O)
            {
                OnGameOver();
                // GameEvents::SceneChange ev("GameOver");
                // Notify(ev);
            }
            else if (event.keyCode == IInputEvent::IEvent::KeyCode::S)
            {
                collisions_->setActive(!collisions_->getActive());
            }
        }
        ui_->HandleEvent(event);
        Notify(event);
    }

    void Awake() override
    {
        ui_->AddObserver(weak_from_this());
        labelScore_ = UI_LABEL("score_txt", 0.02f, 0.02f, 0.1f, 0.1f, "score");
        labelScore_->SetTextColor(Colors::kWhiteCatp);
        labelScore_->SetTextSize(30);

        labelLives_ = UI_LABEL("lives_txt", 0.02f, 0.08f, 0.1f, 0.1f, "^ ^ ^");
        labelLives_->SetTextColor(Colors::kRedCatp);
        labelLives_->SetTextSize(50);
    }

    void Start() override
    {
        gameOverCooldown_ = 0;
        player_ = std::make_shared<Player>(renderer_);
        player_->Initialize();
        objects_.push_back(player_);

        auto playerController = player_->GetComponent<PlayerController>();
        AddObserver(playerController->getWeakPtr());
        playerController->AddObserver(weak_from_this());

        asteroidsManager_ = std::make_shared<AsteroidsManager>(numAsteroids_);
        asteroidsManager_->InitAsteroids(renderer_);
        asteroidsManager_->AddObserver(weak_from_this());

        for (auto &a : asteroidsManager_->GetAsteroids())
        {
            objects_.push_back(a);
        }

        bulletManager_ = std::make_shared<BulletManager>(renderer_);
        playerController->AddObserver(bulletManager_->getWeakPtr());

        for (auto &b : bulletManager_->GetBullets())
        {
            LOG_DEBUG << "PUSH BULLET";
            objects_.push_back(b);
        }

        gameArea_ = std::make_shared<GameArea>(Vector2(800, 800), player_, objects_);

        AwakeObjects();
        collisions_->AddObjects(objects_);
        gameState_ = kPlay;
        StartObjects();
    }

    void Update(float deltaTime) override
    {
        int l = player_->GetComponent<PlayerController>()->Lives();
        if (l == 0)
        {
            labelLives_->SetText(" x_x ");
        }
        else if (l == 1)
        {
            labelLives_->SetText("^");
        }
        else if (l == 2)
        {
            labelLives_->SetText("^ ^");
        }
        else if (l == 3)
        {
            labelLives_->SetText("^ ^ ^");
        }

        labelScore_->SetText(std::to_string(score_));

        UpdateObjects(deltaTime);
        gameArea_->Check(deltaTime);
        bulletManager_->Update(deltaTime);

        if (gameState_ == kGameOver)
        {
            gameOverCooldown_ += deltaTime;
            if (gameOverCooldown_ >= kGameOverCooldown_)
            {
                OnGameShutdown();
                GameEvents::SceneChange ev("GameOver");
                Notify(ev);
            }
        }
    }

    void Render() override
    {
        RenderObjects();
    }

    void ScoreUp()
    {
        score_ += 50;
    }

    void OnGameOver()
    {
        gameState_ = kGameOver;
        LOG_INFO << "ON GAME OVER";
    }

    void OnGameShutdown()
    {
        gameState_ = kShutdown;
        score_ = 0;
        for (auto &obj : objects_)
        {
            obj->Destroy();
        }
        asteroidsManager_->Clear();
        if (bulletManager_)
        {
            bulletManager_->Clear();
        }
        objects_.clear();
    }
};