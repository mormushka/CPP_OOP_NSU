#pragma once
#include "UIElement.h"

class Slider : public UIElement
{
private:
    std::string fontPath_;
    float minValue_ = 0.0f;
    float maxValue_ = 1.0f;
    float currentValue_;
    float sliderWidth_ = UIConfig::SLIDER_DEFAULT_WIDTH;
    bool isDragging_ = false;
    Vector2 dragStartPos_;
    float dragStartValue_;

    Color trackColor_ = UIConfig::SLIDER_TRACK_COLOR;
    Color fillColor_ = UIConfig::SLIDER_FILL_COLOR;
    Color thumbColor_ = UIConfig::SLIDER_THUMB_COLOR;
    Color thumbHoverColor_ = UIConfig::SLIDER_THUMB_HOVER_COLOR;
    Color thumbPressedColor_ = UIConfig::SLIDER_THUMB_PRESSED_COLOR;

public:
    Slider(const std::string &id, const Vector2 &pos, const Vector2 &size, float initialValue)
        : UIElement(id, pos, size, UIConfig::DEFAULT_SLIDER_FONT_SIZE),
          currentValue_(initialValue)
    {
        SetBackgroundColor(UIConfig::TRANSPARENT);
        SetTextColor(UIConfig::SLIDER_TEXT_COLOR);
    }

    void Render(IRenderer &renderer) override
    {
        if (!IsVisible())
            return;

        const Vector2 &pos = GetAbsolutePos();
        const Vector2 &size = GetAbsoluteSize();

        float trackHeight = size.y * 0.2f;
        Vector2 trackPos = {pos.x, pos.y + (size.y - trackHeight) / 2.0f};
        Vector2 trackSize = {size.x, trackHeight};

        renderer.DrawRectangle(trackPos, trackSize, trackColor_, 0.0f);

        float fillWidth = (currentValue_ - minValue_) / (maxValue_ - minValue_) * size.x;
        Vector2 fillSize = {fillWidth, trackHeight};

        if (fillWidth > 0)
        {
            renderer.DrawRectangle(trackPos, fillSize, fillColor_, 0.0f);
        }

        float thumbX = pos.x + fillWidth - sliderWidth_ / 2.0f;
        thumbX = std::max(pos.x, std::min(thumbX, pos.x + size.x - sliderWidth_));

        Vector2 thumbPos = {thumbX, pos.y};
        Vector2 thumbSize = {sliderWidth_, size.y};

        Color thumbColor = thumbColor_;
        if (isPressed_ || isDragging_)
            thumbColor = thumbPressedColor_;
        else if (isHovered_)
            thumbColor = thumbHoverColor_;

        renderer.DrawRectangle(thumbPos, thumbSize, thumbColor, 2.0f);

        if (!GetText().empty() && fontPath_ != "")
        {
            Vector2 textPos = {pos.x + size.x + 10.0f, pos.y + size.y / 2.0f};
            std::string valueText = GetText() + ": " + std::to_string(static_cast<int>(currentValue_ * 100.f));
            renderer.DrawText(valueText, fontPath_, textPos, fontSize_, GetTextColor(), false);
        }
    }

    void OnPress() override
    {
        isPressed_ = true;
        StartDragging();
    }

    void OnRelease() override
    {
        isPressed_ = false;
        isDragging_ = false;
    }

    void OnHover() override
    {
        isHovered_ = true;
    }

    void OnUnhover() override
    {
        isHovered_ = false;
        isPressed_ = false;
        isDragging_ = false;
    }

    void UpdateValueFromMousePos(const Vector2 &mousePos)
    {
        const Vector2 &pos = GetAbsolutePos();
        const Vector2 &size = GetAbsoluteSize();

        float relativeX = (mousePos.x - pos.x) / size.x;
        relativeX = std::max(0.0f, std::min(1.0f, relativeX));

        float newValue = minValue_ + relativeX * (maxValue_ - minValue_);
        SetValue(newValue);
    }

    void StartDragging()
    {
        isDragging_ = true;
        dragStartPos_ = GetAbsolutePos();
        dragStartValue_ = currentValue_;
    }

    void SetValue(float value)
    {
        float oldValue = currentValue_;
        currentValue_ = std::max(minValue_, std::min(maxValue_, value));
    }

    float GetValue() const { return currentValue_; }
    float GetMinValue() const { return minValue_; }
    float GetMaxValue() const { return maxValue_; }

    void SetRange(float minValue, float maxValue)
    {
        minValue_ = minValue;
        maxValue_ = maxValue;
        SetValue(currentValue_);
    }

    void SetColors(const Color &track, const Color &fill, const Color &thumb)
    {
        trackColor_ = track;
        fillColor_ = fill;
        thumbColor_ = thumb;
    }

    void SetThumbHoverColor(const Color &color) { thumbHoverColor_ = color; }
    void SetThumbPressedColor(const Color &color) { thumbPressedColor_ = color; }

    void SetFont(const std::string &fontPath)
    {
        fontPath_ = fontPath;
    }

    void SetSliderWidth(float width) { sliderWidth_ = width; }

    bool IsDragging() const { return isDragging_; }
};
