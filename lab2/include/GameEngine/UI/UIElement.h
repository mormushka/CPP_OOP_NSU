#pragma once
#include "IRenderer.h"
#include "UIConfig.h"

class UIElement
{
protected:
    std::string id_;
    Vector2 relativePosition_;
    Vector2 relativeSize_;
    Color backgroundColor_ = UIConfig::DEFAULT_BACKGROUND_COLOR;
    Color textColor_ = UIConfig::DEFAULT_TEXT_COLOR;
    std::string text_;
    bool isVisible_ = true;
    bool isHovered_ = false;
    bool isPressed_ = false;
    unsigned int fontSize_;

    Vector2 absolutePosition_;
    Vector2 absoluteSize_;

public:
    UIElement(const std::string &id, const Vector2 &pos, const Vector2 &size, const unsigned int &fontSize)
        : id_(id),
          relativePosition_(pos),
          relativeSize_(size),
          fontSize_(fontSize)
    {
    }

    virtual ~UIElement() = default;

    const std::string &GetId() const { return id_; }

    void CalculateAbsoluteCoords(const Vector2& screenSize)
    {
        absolutePosition_.x = relativePosition_.x * screenSize.x;
        absolutePosition_.y = relativePosition_.y * screenSize.y;
        absoluteSize_.x = relativeSize_.x * screenSize.x;
        absoluteSize_.y = relativeSize_.y * screenSize.y;
    }

    virtual void Render(IRenderer &renderer)
    {
        if (!isVisible_)
            return;
        Color bg = isPressed_ ? UIConfig::PRESSED_COLOR : isHovered_ ? UIConfig::HOVER_COLOR
                                                                            : backgroundColor_;
        renderer.DrawRectangle(absolutePosition_, absoluteSize_, bg, UIConfig::THUMB_BORDER_RADIUS);
    }

    virtual bool IsPointInside(const Vector2 &p) const
    {
        return p.x >= absolutePosition_.x && p.x <= absolutePosition_.x + absoluteSize_.x &&
               p.y >= absolutePosition_.y && p.y <= absolutePosition_.y + absoluteSize_.y;
    }

    virtual void OnClick() {}
    virtual void OnHover() { isHovered_ = true; }
    virtual void OnUnhover()
    {
        isHovered_ = false;
        isPressed_ = false;
    }
    virtual void OnPress() { isPressed_ = true; }
    virtual void OnRelease() { isPressed_ = false; }

    void SetBackgroundColor(const Color &color) { backgroundColor_ = color; }
    void SetTextColor(const Color &color) { textColor_ = color; }
    void SetTextSize(unsigned int fontSize) { fontSize_ = fontSize; }
    void SetText(const std::string &text) { text_ = text; }
    void SetVisible(bool visible) { isVisible_ = visible; }

    const Vector2 &GetAbsolutePos() const { return absolutePosition_; }
    const Vector2 &GetAbsoluteSize() const { return absoluteSize_; }
    const std::string &GetText() const { return text_; }
    const Color &GetTextColor() const { return textColor_; }
    bool IsVisible() const { return isVisible_; }
    bool IsHovered() const { return isHovered_; }
    bool IsPressed() const { return isPressed_; }
};