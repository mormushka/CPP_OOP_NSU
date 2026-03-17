// ConvexShape.h
#pragma once
#include "BehaviourComponent.h"
#include "Transform.h"

class ConvexShape : public BehaviourComponent
{
private:
    Convex convex_;
    Convex convexMutable_;
    std::weak_ptr<IRenderer> renderer_;
    std::shared_ptr<Transform> transform_;
    bool isVisible_ = true;

public:
    ConvexShape() = default;
    ConvexShape(Convex convex, std::weak_ptr<IRenderer> renderer)
        : convex_(convex), convexMutable_(convex), renderer_(renderer) {}

    void Awake() override
    {
    }

    void Start() override
    {
        if (renderer_.lock() && Owner().lock())
        {
            transform_ = Owner().lock()->GetComponent<Transform>();
        }
    }

    void Update(float deltaTime) override {}

    void Render() override
    {
        if (!isVisible_)
            return;

        if (!transform_)
            return;

        convexMutable_.position = transform_->WorldPosition();
        convexMutable_.scale = transform_->Scale();
        convexMutable_.rotation = transform_->Rotation();
        // LOG_DEBUG << std::to_string(convexMutable_.position.x) << " " << std::to_string(convexMutable_.position.y);
        // LOG_DEBUG << std::to_string(convexMutable_.scale.x) << " " << std::to_string(convexMutable_.scale.y);
        // LOG_DEBUG << std::to_string(convex_.scale.x) << " " << std::to_string(convex_.scale.y);
        // LOG_DEBUG << std::to_string(transform_->GetScale().x) << " " << std::to_string(transform_->GetScale().y);
        renderer_.lock()->DrawConvex(convexMutable_);
    }

    void SetOutlineColor(const Color &color) { convexMutable_.outlinecolor = color; }
    Color OutlineColor() const { return convexMutable_.outlinecolor; }

    void SetFillColor(const Color &color) { convexMutable_.fillColor = color; }
    Color FillColor() const { return convexMutable_.fillColor; }
};