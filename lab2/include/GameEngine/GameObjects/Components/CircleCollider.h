// CircleCollider.h
#pragma once
#include "Component.h"
#include "Transform.h"
#include "Vectors.h"

class CircleCollider : public Component
{
private:
    float radius_ = 1.0f;
    Vector2 offset_ = Vector2::zero();
    bool isTrigger_ = false;

public:
    CircleCollider() = default;
    CircleCollider(float radius)
    {
        radius_ = GetWorldRadius();
    }
    CircleCollider(float radius, const Vector2 &offset) : CircleCollider(radius)
    {
        offset_ = offset;
    }

    float GetRadius() const { return radius_; }
    void SetRadius(float radius) { radius_ = radius; }

    Vector2 GetOffset() const { return offset_; }
    void SetOffset(const Vector2 &offset) { offset_ = offset; }

    bool IsTrigger() const { return isTrigger_; }
    void SetIsTrigger(bool isTrigger) { isTrigger_ = isTrigger; }

    Vector2 GetWorldCenter() const
    {
        if (!Owner().lock())
            return offset_;

        auto transform = Owner().lock()->GetComponent <Transform>();
        if (!transform)
            return offset_;

        return transform->TransformPoint(offset_);
    }

    float GetWorldRadius() const
    {
        if (!Owner().lock())
            return radius_;

        auto transform = Owner().lock()->GetComponent<Transform>();
        if (!transform)
            return radius_;

        Vector2 scale = transform->Scale();
        float avgScale = (scale.x + scale.y) / 2.0f;
        return radius_ * avgScale;
    }
};