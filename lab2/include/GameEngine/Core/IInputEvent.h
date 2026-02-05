// IInputEvent.h
#pragma once
#include "Observer.hpp"
#include "Vectors.h"

class IInputEvent
{
public:
    struct IEvent : public Event
    {
        enum class Type
        {
            None,
            Closed,
            KeyPressed,
            KeyReleased,
            MouseMoved,
            MouseButtonPressed,
            MouseButtonReleased
        };

        enum class KeyCode
        {
            Unknown,
            Left,
            Right,
            Up,
            Down,
            Escape,
            Space,
            P,
            O,
            R,
            S
        };

        Type type;
        KeyCode keyCode;
        Vector2 mousePos;
        int mouseButton;
    };

    virtual ~IInputEvent() = default;
    virtual bool PollEvent(IEvent &e) = 0;
};
