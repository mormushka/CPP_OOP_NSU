// Movement.h - исправленная версия с физически корректным движением
#pragma once
#include "BehaviourComponent.h"
#include "Vectors.h"
#include "Transform.h"
#include <cmath>

class Movement : public BehaviourComponent
{
protected:
    Vector2 velocity_ = {0.f, 0.f};
    Vector2 acceleration_ = {0.f, 0.f};
    float maxSpeed_ = 300.f;
    float drag_ = 0.f; // Сопротивление среды
    float mass_ = 1.f;

    // Гравитациz
    bool gravityEnabled_ = false;
    Vector2 gravity_ = {0.f, 9.8f * 100.f};

    bool isGrounded_ = false;

    bool collisionEnabled_ = true;
    Vector2 lastSafePosition_;

public:
    Movement() = default;

    void Awake() override {}
    void Start() override {}

    void Update(float deltaTime) override
    {
        if (!Owner().lock() || deltaTime <= 0.0f)
            return;

        if (gravityEnabled_ && !isGrounded_)
        {
            AddForce(gravity_ * mass_);
        }

        velocity_ += acceleration_ * deltaTime; // Применяем ускорение к скорости
        acceleration_ = Vector2::zero();       // Сбрасываем ускорение каждый кадр

        // Применяем сопротивление
        if (velocity_.LengthSquared() > 0.0001f)
        {
            Vector2 dragForce = velocity_.Normalized() * -1 * drag_ * deltaTime;
            velocity_ += dragForce;

            if (velocity_.LengthSquared() < 0.01f)
            {
                velocity_ = Vector2::zero();
            }
        }

        float currentSpeed = velocity_.Length();
        if (currentSpeed > maxSpeed_)
        {
            velocity_ = velocity_.Normalized() * maxSpeed_;
        }

        // Сохраняем текущую позицию как безопасную
        auto transform = Owner().lock()->GetComponent<Transform>();
        if (transform && collisionEnabled_)
        {
            lastSafePosition_ = transform->LocalPosition();
        }
        if (transform)
        {
            Vector2 newPos = transform->LocalPosition() + velocity_ * deltaTime;
            transform->SetLocalPosition(newPos);
        }
    }

    void SetAcceleration(const Vector2 &a) { acceleration_ = a; }

    void SetGravityEnabled(bool enabled) { gravityEnabled_ = enabled; }
    bool IsGravityEnabled() const { return gravityEnabled_; }

    void SetGravity(const Vector2 &newGravity) { gravity_ = newGravity; }
    Vector2 Gravity() const { return gravity_; }

    void SetGravityStrength(float strength)
    {
        gravity_.y = strength;
        gravity_.x = 0; // только вниз
    }
    float GravityStrength() const { return gravity_.y; }

    void EnableDownwardGravity(float strength = 9.8f * 100.f)
    {
        gravityEnabled_ = true;
        gravity_ = Vector2(0, strength);
    }

    void EnableDirectionalGravity(const Vector2 &direction, float strength)
    {
        gravityEnabled_ = true;
        gravity_ = direction.Normalized() * strength;
    }

    void DisableGravity()
    {
        gravityEnabled_ = false;
        isGrounded_ = false;
    }

    void SetVelocity(const Vector2 &vel) { velocity_ = vel; }
    Vector2 Velocity() const { return velocity_; }

    void SetMaxSpeed(float speed)
    {
        if (speed >= 0.0f)
            maxSpeed_ = speed;
    }
    float MaxSpeed() const { return maxSpeed_; }

    void SetDrag(float newDrag) { drag_ = std::max(0.0f, newDrag); }
    float Drag() const { return drag_; }

    void SetMass(float newMass)
    {
        if (newMass > 0.0f)
            mass_ = newMass;
    }
    float Mass() const { return mass_; }

    // Добавляет силу (учитывается масса)
    void AddForce(const Vector2 &force) { acceleration_ += force / mass_; }

    // Добавляет мгновенный импульс (не зависит от времени кадра)
    void AddImpulse(const Vector2 &impulse) { velocity_ += impulse / mass_; }

    // Добавляет силу в указанном направлении
    void AddForceInDirection(float forceAmount, float angleDegrees)
    {
        float rad = angleDegrees * 3.14159f / 180.0f;
        Vector2 force(
            std::cos(rad) * forceAmount,
            std::sin(rad) * forceAmount);
        AddForce(force);
    }

    // Добавляет импульс в указанном направлении
    void AddImpulseInDirection(float impulseAmount, float angleDegrees)
    {
        float rad = angleDegrees * 3.14159f / 180.0f;
        Vector2 impulse(
            std::cos(rad) * impulseAmount,
            std::sin(rad) * impulseAmount);
        AddImpulse(impulse);
    }

    void Stop()
    {
        velocity_ = Vector2::zero();
        acceleration_ = Vector2::zero();
    }

    void Brake(float brakeForce)
    {
        if (velocity_.LengthSquared() > 0.0001f)
        {
            Vector2 brakeDir = velocity_.Normalized() * -1;
            AddForce(brakeDir * brakeForce);
        }
    }

    Vector2 MovementDirection() const
    {
        if (velocity_.LengthSquared() > 0.0001f)
            return velocity_.Normalized();
        return Vector2::zero();
    }

    float Speed() const { return velocity_.Length(); }
    bool IsMoving() const { return velocity_.LengthSquared() > 0.0001f; }

    void EnableCollisions(bool enabled) { collisionEnabled_ = enabled; }
    bool IsCollisionsEnabled() const { return collisionEnabled_; }

    void SetLastSafePosition(const Vector2 &pos) { lastSafePosition_ = pos; }
    Vector2 LastSafePosition() const { return lastSafePosition_; }

    void RevertToLastSafePosition()
    {
        auto transform = Owner().lock()->GetComponent<Transform>();
        if (transform)
        {
            transform->SetLocalPosition(lastSafePosition_);
            velocity_ = Vector2::zero();
        }
    }
};