#pragma once
#include "UIElement.h"

class Label : public UIElement
{
private:
    std::string fontPath_;

public:
    Label(const std::string &id, const Vector2 &pos, const Vector2 &size, const std::string &text = "")
        : UIElement(id, pos, size, UIConfig::DEFAULT_LABEL_FONT_SIZE)
    {
        tag_ = kUILabel;
        SetBackgroundColor(UIConfig::TRANSPARENT);
        SetText(text);
    }

    void Render(IRenderer &renderer) override
    {
        if (!IsVisible() || GetText().empty())
            return;

        renderer.DrawText(GetText(), fontPath_, GetAbsolutePos(),
                          fontSize_, GetTextColor(), false);
    }

    void SetFont(const std::string &fontPath)
    {
        fontPath_ = fontPath;
    }
};
