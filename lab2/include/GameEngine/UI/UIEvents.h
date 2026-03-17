#pragma once
#include "Observer.hpp"
#include <string>

namespace UIEvents
{
    struct ElementClicked : public Event
    {
        std::string elementId;
        ElementClicked(const std::string &id) : elementId(id) {}
    };

    struct ElementHovered : public Event
    {
        std::string elementId;
        ElementHovered(const std::string &id) : elementId(id) {}
    };

    struct ElementUnhovered : public Event
    {
        std::string elementId;
        ElementUnhovered(const std::string &id) : elementId(id) {}
    };

    struct SliderValueChanged : public Event
    {
        std::string elementId;
        float value;
        SliderValueChanged(const std::string &id, float val) : elementId(id), value(val) {}
    };
}