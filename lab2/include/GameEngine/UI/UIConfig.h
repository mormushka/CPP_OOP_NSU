// UIConfig.h
#pragma once
#include "IRenderer.h"

// Общие настройки UI
namespace UIConfig
{
    // Пути к ресурсам
    constexpr const char *DEFAULT_FONT_PATH = "resources/fonts/8bit.ttf";

    // Размеры и шрифты
    constexpr unsigned int DEFAULT_BUTTON_FONT_SIZE = 24;
    constexpr unsigned int DEFAULT_LABEL_FONT_SIZE = 24;
    constexpr unsigned int DEFAULT_SLIDER_FONT_SIZE = 16;

    // Global UI
    constexpr float THUMB_BORDER_RADIUS = 2.0f;

    // Цвета элементов
    inline const Color DEFAULT_BACKGROUND_COLOR(200, 200, 255, 255);
    inline const Color DEFAULT_TEXT_COLOR(30, 30, 90, 255);

    inline const Color HOVER_COLOR(255, 180, 180, 255);
    inline const Color PRESSED_COLOR(150, 255, 150, 255);

    // Цвета слайдера
    inline const Color SLIDER_TRACK_COLOR(100, 100, 100, 255);
    inline const Color SLIDER_FILL_COLOR(70, 130, 180, 255);
    inline const Color SLIDER_THUMB_COLOR(220, 220, 220, 255);
    inline const Color SLIDER_THUMB_HOVER_COLOR(200, 200, 240, 255);
    inline const Color SLIDER_THUMB_PRESSED_COLOR(180, 180, 220, 255);
    inline const Color SLIDER_TEXT_COLOR(220, 220, 220, 255);

    constexpr float SLIDER_TRACK_HEIGHT_RATIO = 0.2f;
    constexpr float SLIDER_THUMB_BORDER_RADIUS = 2.0f;
    constexpr float SLIDER_TEXT_OFFSET = 10.0f;
    constexpr float SLIDER_DEFAULT_WIDTH = 20.0f;

    // Прозрачные цвета
    inline const Color TRANSPARENT(0, 0, 0, 0);
}
