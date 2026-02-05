// SFMLInputEvent.h
#pragma once
#include "IInputEvent.h"
#include <SFML/Window.hpp>
#include "Logger.hpp"

class SFMLInputEvent : public IInputEvent
{
private:
    sf::Window &window;

    static IEvent::KeyCode MapKey(sf::Keyboard::Key sfKey)
    {
        switch (sfKey)
        {
        case sf::Keyboard::Left:
            return IEvent::KeyCode::Left;
        case sf::Keyboard::Right:
            return IEvent::KeyCode::Right;
        case sf::Keyboard::Up:
            return IEvent::KeyCode::Up;
        case sf::Keyboard::Down:
            return IEvent::KeyCode::Down;
        case sf::Keyboard::Escape:
            return IEvent::KeyCode::Escape;
        case sf::Keyboard::Space:
            return IEvent::KeyCode::Space;
        case sf::Keyboard::P:
            return IEvent::KeyCode::P;
        case sf::Keyboard::O:
            return IEvent::KeyCode::O;
        case sf::Keyboard::R:
            return IEvent::KeyCode::R;
        case sf::Keyboard::S:
            return IEvent::KeyCode::S;
        default:
            return IEvent::KeyCode::Unknown;
        }
    }

public:
    explicit SFMLInputEvent(sf::Window &win) : window(win) {}

    bool PollEvent(IEvent &e) override
    {
        sf::Event sfEvent;
        if (window.pollEvent(sfEvent))
        {
            switch (sfEvent.type)
            {
            case sf::Event::Closed:
                e.type = IEvent::Type::Closed;
                return true;
            case sf::Event::KeyPressed:
                e.type = IEvent::Type::KeyPressed;
                e.keyCode = MapKey(sfEvent.key.code);
                return true;
            case sf::Event::KeyReleased:
                e.type = IEvent::Type::KeyReleased;
                e.keyCode = MapKey(sfEvent.key.code);
                return true;
            case sf::Event::MouseMoved:
                e.type = IEvent::Type::MouseMoved;
                e.mousePos = {static_cast<float>(sfEvent.mouseMove.x),
                              static_cast<float>(sfEvent.mouseMove.y)};
                return true;
            case sf::Event::MouseButtonPressed:
                e.type = IEvent::Type::MouseButtonPressed;
                e.mouseButton = sfEvent.mouseButton.button;
                e.mousePos = {static_cast<float>(sfEvent.mouseButton.x),
                              static_cast<float>(sfEvent.mouseButton.y)};
                return true;
            case sf::Event::MouseButtonReleased:
                e.type = IEvent::Type::MouseButtonReleased;
                e.mouseButton = sfEvent.mouseButton.button;
                e.mousePos = {static_cast<float>(sfEvent.mouseButton.x),
                              static_cast<float>(sfEvent.mouseButton.y)};
                return true;
            default:
                break;
            }
        }
        return false;
    }
};
