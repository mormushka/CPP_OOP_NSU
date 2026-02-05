// UIManager.h
#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IRenderer.h"
#include "IInputEvent.h"
#include "Observer.hpp"
#include "Logger.hpp"
#include "UIConfig.h"
#include "UIEvents.h"
#include "UIElement.h"
#include "Button.h"
#include "Label.h"
#include "Slider.h"

class UIManager : public Subject<Event>
{
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<UIElement>>> sceneElements_;
    std::string activeScene_;
    std::string defaultFontPath_ = UIConfig::DEFAULT_FONT_PATH;
    UIElement *hoveredElement_ = nullptr;
    UIElement *pressedElement_ = nullptr;

public:
    UIManager() = default;

    void SetActiveScene(const std::string &sceneName)
    {
        LOG_INFO << "SetActiveUIScene: " + sceneName;
        activeScene_ = sceneName;
    }

    bool LoadFont(const std::string &fontPath)
    {
        defaultFontPath_ = fontPath;
        return true;
    }

    template <typename T>
    T *CreateElement(const std::string &sceneName,
                     const std::string &id,
                     const Vector2 &pos,
                     const Vector2 &size,
                     const std::string &text = "")
    {
        auto element = std::make_unique<T>(id, pos, size, text);
        element->SetFont(defaultFontPath_);
        T *ptr = element.get();
        sceneElements_[sceneName][id] = std::move(element);
        LOG_INFO << "CREATE UI ELEMENT on " + sceneName + ", id: " + id;
        return ptr;
    }

    Slider *CreateSlider(const std::string &sceneName,
                         const std::string &id,
                         const Vector2 &pos,
                         const Vector2 &size,
                         const std::string &text,
                         float initVal)
    {
        auto element = std::make_unique<Slider>(id, pos, size, initVal);
        element->SetFont(defaultFontPath_);
        element->SetText(text);
        auto *ptr = element.get();
        sceneElements_[sceneName][id] = std::move(element);
        LOG_INFO << "CREATE UI ELEMENT on " + sceneName + ", id: " + id;
        return ptr;
    }

    void UpdateLayout(IRenderer &renderer)
    {
        int w, h;
        renderer.GetWindowSize(w, h);
        for (auto &[id, element] : sceneElements_[activeScene_])
        {
            element->CalculateAbsoluteCoords(w, h);
        }
    }

    void Update(float deltaTime)
    {
        // Обновление анимаций или других эффектов
    }

    void Render(IRenderer &renderer)
    {
        if (activeScene_.empty())
        {
            return;
        }
        for (auto &[id, element] : sceneElements_[activeScene_])
        {
            element->Render(renderer);
        }
    }

    void HandleEvent(const IInputEvent::IEvent &event)
    {
        switch (event.type)
        {
        case IInputEvent::IEvent::Type::MouseMoved:
            HandleMouseMove(event.mousePos);
            break;

        case IInputEvent::IEvent::Type::MouseButtonPressed:
            if (event.mouseButton == 0)
            {
                HandleMousePress(event.mousePos);
            }
            break;

        case IInputEvent::IEvent::Type::MouseButtonReleased:
            if (event.mouseButton == 0)
            {
                HandleMouseRelease(event.mousePos);
            }
            break;

        default:
            break;
        }
    }



private:
    void HandleMouseMove(const Vector2 &mousePos)
    {
        Slider *draggingSlider = GetDraggingSlider();
        if (draggingSlider && draggingSlider->IsDragging())
        {
            float oldValue = draggingSlider->GetValue();
            draggingSlider->UpdateValueFromMousePos(mousePos);

            float newValue = draggingSlider->GetValue();
            if (oldValue != newValue)
            {
                Notify(UIEvents::SliderValueChanged(draggingSlider->GetId(), newValue));
            }
            return;
        }

        UIElement *newHovered = nullptr;
        for (auto &[id, element] : sceneElements_[activeScene_])
        {
            if (element->IsVisible() && element->IsPointInside(mousePos))
            {
                newHovered = element.get();
                break;
            }
        }

        if (hoveredElement_ && hoveredElement_ != newHovered)
        {
            hoveredElement_->OnUnhover();
            Notify(UIEvents::ElementUnhovered(hoveredElement_->GetId()));
        }

        if (newHovered && newHovered != hoveredElement_)
        {
            newHovered->OnHover();
            Notify(UIEvents::ElementHovered(newHovered->GetId()));
        }

        hoveredElement_ = newHovered;
    }

    void HandleMousePress(const Vector2 &p)
    {
        if (hoveredElement_)
        {
            hoveredElement_->OnPress();
            pressedElement_ = hoveredElement_;

            if (auto *slider = dynamic_cast<Slider *>(hoveredElement_))
            {
                float oldValue = slider->GetValue();
                slider->UpdateValueFromMousePos(p);

                float newValue = slider->GetValue();
                if (oldValue != newValue)
                {
                    Notify(UIEvents::SliderValueChanged(slider->GetId(), newValue));
                }
            }
        }
    }

    void HandleMouseRelease(const Vector2 &p)
    {
        if (pressedElement_)
        {
            pressedElement_->OnRelease();

            if (pressedElement_ == hoveredElement_)
            {
                pressedElement_->OnClick();
                Notify(UIEvents::ElementClicked(pressedElement_->GetId()));
            }

            pressedElement_ = nullptr;
        }
    }

    Slider *GetDraggingSlider()
    {
        if (pressedElement_)
        {
            return dynamic_cast<Slider *>(pressedElement_);
        }
        return nullptr;
    }
};