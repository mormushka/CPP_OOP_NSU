// IClock.h
#pragma once

class IClock {
public:
    virtual ~IClock() = default;
    virtual float GetDeltaTime() = 0;
    virtual void Reset() = 0;
};
