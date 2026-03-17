// BulletManager.h
#pragma once
#include "Bullet.h"
#include "IRenderer.h"
#include "Observer.hpp"
#include <vector>
#include <memory>
#include "GameEvents.h"

class BulletManager : public Observer<Event>
{
private:
    std::vector<std::shared_ptr<Bullet>> bullets_;
    std::vector<std::shared_ptr<Bullet>> bulletPool_;
    const int maxPoolSize_ = 40;

public:
    BulletManager(std::shared_ptr<IRenderer> renderer)
    {
        for (int i = 0; i < maxPoolSize_; ++i)
        {
            auto bullet = std::make_shared<Bullet>(renderer);
            bullet->Initialize();
            bullet->Disable();
            bulletPool_.push_back(bullet);
        }
    }

    virtual void OnNotify(const Event &event) override
    {
        if (event.tag == GameEvents::kShoot)
        {
            const auto &shootEvent = static_cast<const GameEvents::Shoot &>(event);
            Shoot(shootEvent.position, shootEvent.direction, shootEvent.rotation);
        }
    }

    std::vector<std::shared_ptr<Bullet>> &GetBullets() { return bulletPool_; }

    void Shoot(Vector2 position, Vector2 direction, float rotation)
    {
        std::shared_ptr<Bullet> bullet = nullptr;

        for (auto &b : bulletPool_)
        {
            if (!b->IsActive())
            {
                bullet = b;
                break;
            }
        }

        if (!bullet)
        {
            LOG_WARN << "No available bullets in pool";
            return;
        }

        auto transform = bullet->GetComponent<Transform>();
        auto movement = bullet->GetComponent<Movement>();

        if (transform && movement)
        {
            transform->SetLocalPosition(position);
            transform->SetRotation(rotation);

            movement->SetVelocity(direction * movement->MaxSpeed());

            bullet->Enable();
            if (std::find(bullets_.begin(), bullets_.end(), bullet) == bullets_.end())
            {
                bullets_.push_back(bullet);
            }
        }
    }

    void Update(float deltaTime)
    {
        bullets_.erase(
            std::remove_if(bullets_.begin(), bullets_.end(),
                           [](const std::shared_ptr<Bullet> &bullet)
                           {
                               return !bullet->IsActive();
                           }),
            bullets_.end());
    }

    void Clear()
    {
        for (auto &bullet : bulletPool_)
        {
            bullet->Disable();
        }
        bullets_.clear();
    }
};