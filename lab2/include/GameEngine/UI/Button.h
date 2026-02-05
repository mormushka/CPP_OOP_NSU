#pragma once
#include "UIElement.h"

class Button : public UIElement
{
private:
    std::string fontPath_;

public:
    Button(const std::string &id, const Vector2 &pos, const Vector2 &size, const std::string &text = "")
        : UIElement(id, pos, size, UIConfig::DEFAULT_BUTTON_FONT_SIZE)
    {
        SetText(text);
    }

    void Render(IRenderer &renderer) override
    {
        UIElement::Render(renderer);

        if (!IsVisible() || GetText().empty())
            return;

        Vector2 center = {
            GetAbsolutePos().x + GetAbsoluteSize().x / 2.0f,
            GetAbsolutePos().y + GetAbsoluteSize().y / 2.0f};

        renderer.DrawText(GetText(), fontPath_, center,
                          fontSize_, GetTextColor(), true);
    }

    void SetFont(const std::string &fontPath)
    {
        fontPath_ = fontPath;
    }
};
