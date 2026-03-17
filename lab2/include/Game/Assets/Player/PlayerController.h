// PlayerController.h
#pragma once
#include "BehaviourComponent.h"
#include "IInputEvent.h"
#include "ConvexShape.h"
#include "Movement.h"
#include <memory>

enum MoveState
{
    kNone = 0,
    kRotateLeft = 1 << 0,
    kRotateRight = 1 << 1,
    kMoveForward = 1 << 2,
    kMoveBackward = 1 << 3
};

class PlayerController : public BehaviourComponent
{
private:
    int state_ = 0;
    int moveState = 0;
    int lives_ = 3;
    bool shooting = false;
    float rotationSpeed = 360.0f;
    float accelerationForce = 8000.0f;
    float brakeForce = 8000.0f;
    float stunTimer_ = 0.0f;
    const float kStunDuration_ = 4.0f;
    float animTimer_ = 0.0f;
    const float kAnimDuration_ = 0.2f;
    std::shared_ptr<Movement> movement;
    std::shared_ptr<Transform> transform;
    std::shared_ptr<ConvexShape> convexShape;

    float shootCooldown_ = 0.0f;
    const float kShootCooldown_ = 0.3f;

public:
    enum PlayerState
    {
        kNormal,
        kStunned,
        kDestroyed
    };

    PlayerController() = default;

    void Awake() override
    {
        if (!Owner().lock())
            return;

        movement = Owner().lock()->GetComponent<Movement>();
        transform = Owner().lock()->GetComponent<Transform>();
        convexShape = Owner().lock()->GetComponent<ConvexShape>();
    }

    void Start() override {}

    void OnNotify(const Event &event) override
    {
        if (!Owner().lock())
            return;

        if (auto *inputEvent = dynamic_cast<const IInputEvent::IEvent *>(&event))
        {
            if (inputEvent->type == IInputEvent::IEvent::Type::KeyPressed)
            {
                if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Left)
                {
                    moveState |= kRotateLeft;
                }
                else if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Right)
                {
                    moveState |= kRotateRight;
                }
                else if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Up)
                {
                    moveState |= kMoveForward;
                }
                else if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Down)
                {
                    moveState |= kMoveBackward;
                }
                else if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Space)
                {
                    shooting = true;
                }
            }
            else if (inputEvent->type == IInputEvent::IEvent::Type::KeyReleased)
            {
                if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Left)
                {
                    moveState &= ~kRotateLeft;
                }
                else if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Right)
                {
                    moveState &= ~kRotateRight;
                }
                else if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Up)
                {
                    moveState &= ~kMoveForward;
                }
                else if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Down)
                {
                    moveState &= ~kMoveBackward;
                }
                else if (inputEvent->keyCode == IInputEvent::IEvent::KeyCode::Space)
                {
                    shooting = false;
                }
            }
        }
    }

    void Update(float deltaTime) override
    {
        if (!transform || !movement)
            return;

        if (state_ == kDestroyed)
        {
            return;
        }

        if (state_ == kStunned)
        {
            stunTimer_ += deltaTime;
            animTimer_ += deltaTime;
            Color c = convexShape->OutlineColor();
            if (animTimer_ >= kAnimDuration_)
            {
                animTimer_ = 0.0f;
                c.a = (c.a == 255) ? 0 : 255;
            }
            if (stunTimer_ >= kStunDuration_)
            {
                state_ = kNormal;
                stunTimer_ = 0.0f;
                c.a = 255;
            }
            convexShape->SetOutlineColor(c);
        }

        if (shootCooldown_ > 0.0f)
        {
            shootCooldown_ -= deltaTime;
        }

        if (shooting)
        {
            Shoot();
        }

        if (moveState & kRotateLeft)
        {
            transform->SetRotation(transform->Rotation() - rotationSpeed * deltaTime);
        }
        if (moveState & kRotateRight)
        {
            transform->SetRotation(transform->Rotation() + rotationSpeed * deltaTime);
        }

        if (moveState & kMoveForward)
        {
            float rotation = transform->Rotation() - 90.0f;
            float rad = rotation * 3.14159f / 180.0f;
            Vector2 direction(std::cos(rad), std::sin(rad));
            movement->AddForce(direction * accelerationForce);
        }

        if (moveState & kMoveBackward)
        {
            movement->Brake(brakeForce);
        }
    }

    void TakeDamage()
    {
        if (state_ != kStunned && state_ != kDestroyed)
        {
            state_ = kStunned;
            lives_ -= 1;
            if (lives_ <= 0)
            {
                state_ = kDestroyed;
                convexShape->SetFillColor(Color(Colors::kRed));
                GameEvents::GameOver e;
                Notify(e);
            }
        }
    }

    int Lives() { return lives_; }

    void SetRotationSpeed(float speed) { rotationSpeed = speed; }
    float RotationSpeed() const { return rotationSpeed; }

    void SetAccelerationForce(float force) { accelerationForce = force; }
    float AccelerationForce() const { return accelerationForce; }

    void SetBrakeForce(float force) { brakeForce = force; }
    float BrakeForce() const { return brakeForce; }

    int State() const
    {
        return state_;
    }

    void Shoot()
    {
        if (state_ != kDestroyed && shootCooldown_ <= 0.0f)
        {
            float rotation = transform->Rotation() - 90.f;
            float rad = rotation * 3.14159f / 180.0f;
            Vector2 direction(std::cos(rad), std::sin(rad));
            Vector2 spawnPosition = transform->GetLocalPosition() + direction * 20.0f;

            GameEvents::Shoot shootEvent;
            shootEvent.position = spawnPosition;
            shootEvent.direction = direction;
            shootEvent.rotation = transform->Rotation();

            Notify(shootEvent);

            shootCooldown_ = kShootCooldown_;
            
        }
    }
};