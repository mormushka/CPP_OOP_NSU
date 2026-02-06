// Transform.h
#pragma once
#include "Vectors.h"
#include "Component.h"

class Transform : public Component
{
private:
    Vector2 position_ = Vector2::zero();
    Vector2 scale_ = Vector2::one();
    float rotation_ = 0.0f;

public:
    Transform() = default;
    Transform(const Vector2 &position) : position_(position) {}
    Transform(const Vector2 &position, float rotation)
        : position_(position), rotation_(rotation) {}

    Vector2 GetLocalPosition() const { return position_; }
    void SetLocalPosition(const Vector2 &position) { position_ = position; }

    Vector2 WorldPosition() const
    {
        if (!Owner().lock() || !Owner().lock()->Parent().lock())
            return position_;

        if (auto parentTransform = Owner().lock()->Parent().lock()->GetComponent<Transform>())
        {
            return parentTransform->TransformPoint(position_);
        }

        return position_;
    }

    void SetWorldPosition(const Vector2 &worldPosition)
    {
        if (!Owner().lock() || !Owner().lock()->Parent().lock())
        {
            SetLocalPosition(worldPosition);
            return;
        }

        if (auto parentTransform = Owner().lock()->Parent().lock()->GetComponent<Transform>())
        {
            Vector2 localPos = parentTransform->InverseTransformPoint(worldPosition);
            SetLocalPosition(localPos);
        }
        else
        {
            SetLocalPosition(worldPosition);
        }
    }

    void Translate(const Vector2 &translation) { position_ += translation; }
    void Translate(float x, float y) { position_ += Vector2(x, y); }

    void SetScale(const Vector2 &scale) { scale_ = scale; }
    void SetScale(float x, float y) { scale_ = Vector2(x, y); }
    void SetScale(float uniformScale) { scale_ = Vector2(uniformScale, uniformScale); }
    Vector2 Scale() const { return scale_; }

    Vector2 WorldScale() const
    {
        if (!Owner().lock() || !Owner().lock()->Parent().lock())
            return scale_;

        Vector2 worldScale = scale_;
        auto currentParent = Owner().lock()->Parent().lock();

        while (currentParent)
        {
            if (auto parentTransform = currentParent->GetComponent<Transform>())
            {
                Vector2 parentScale = parentTransform->Scale();
                worldScale.x *= parentScale.x;
                worldScale.y *= parentScale.y;
                currentParent = currentParent->Parent().lock();
            }
            else
            {
                break;
            }
        }

        return worldScale;
    }

    void SetRotation(float rotation) { rotation_ = rotation; }
    float Rotation() const { return rotation_; }
    void Rotate(float angle) { rotation_ += angle; }

    float WorldRotation() const
    {
        if (!Owner().lock() || !Owner().lock()->Parent().lock())
            return rotation_;

        float worldRotation = rotation_;
        auto currentParent = Owner().lock()->Parent().lock();

        while (currentParent)
        {
            auto parentTransform = currentParent->GetComponent<Transform>();
            if (!parentTransform)
                break;
            worldRotation += parentTransform->Rotation();
            currentParent = currentParent->Parent().lock();
        }

        return worldRotation;
    }

    // Матричные преобразования (используют только локальные трансформации)
    Vector2 TransformPoint(const Vector2 &point) const
    {
        Vector2 result = point;

        // Масштаб
        result.x *= scale_.x;
        result.y *= scale_.y;

        // Поворот
        if (rotation_ != 0.0f)
        {
            float cosA = cos(rotation_);
            float sinA = sin(rotation_);
            float x = result.x * cosA - result.y * sinA;
            float y = result.x * sinA + result.y * cosA;
            result = Vector2(x, y);
        }

        // Перенос
        result += position_;

        return result;
    }

    Vector2 InverseTransformPoint(const Vector2 &point) const
    {
        Vector2 result = point;

        // Обратный перенос
        result -= position_;

        // Обратный поворот
        if (rotation_ != 0.0f)
        {
            float cosA = cos(-rotation_);
            float sinA = sin(-rotation_);
            float x = result.x * cosA - result.y * sinA;
            float y = result.x * sinA + result.y * cosA;
            result = Vector2(x, y);
        }

        // Обратный масштаб
        if (scale_.x != 0.0f && scale_.y != 0.0f)
            result = Vector2(result.x / scale_.x, result.y / scale_.y);

        return result;
    }

    // Преобразование с учетом всей иерархии
    Vector2 TransformPointWorld(const Vector2 &point) const
    {
        Vector2 result = TransformPoint(point);

        // Применяем трансформации всех родителей
        auto currentParent = Owner().lock() ? Owner().lock()->Parent().lock() : nullptr;
        while (currentParent)
        {
            auto parentTransform = currentParent->GetComponent<Transform>();
            if (!parentTransform)
                break;
            result = parentTransform->TransformPoint(result);
            currentParent = currentParent->Parent().lock();
        }

        return result;
    }
};