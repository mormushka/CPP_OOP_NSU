// IAudioManager.h
#pragma once
#include <string>

enum class SoundType
{
    EFFECT,
    MUSIC
};

class IAudioManager
{
public:
    virtual ~IAudioManager() = default;
    
    // Регистрация звуков
    virtual void RegisterSound(const std::string& name, const std::string& filename,
                              SoundType type, float volume = 100.0f, bool loop = false) = 0;
    
    // Воспроизведение
    virtual void Play(const std::string& name, float pitch = 1.0f) = 0;
    virtual void Crossfade(const std::string& name, float duration = 2.0f) = 0;
    virtual void StopAll() = 0;
    
    // Обновление состояния
    virtual void Update(float dt) = 0;
    
    // Управление громкостью
    virtual float GetMasterVolume() const = 0;
    virtual float GetMusicVolume() const = 0;
    virtual float GetEffectsVolume() const = 0;
    
    virtual void SetMasterVolume(float v) = 0;
    virtual void SetMusicVolume(float v) = 0;
    virtual void SetEffectsVolume(float v) = 0;
    
    // Дополнительные методы управления
    virtual void PauseAll() = 0;
    virtual void ResumeAll() = 0;
    virtual void Stop(const std::string& name) = 0;
};