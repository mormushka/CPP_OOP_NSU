// FakeAudioManager.h
#pragma once
#include "IAudioManager.h"
#include "Logger.hpp"
#include <unordered_map>

class FakeAudioManager : public IAudioManager
{
private:
    struct SoundInfo
    {
        std::string filename;
        SoundType type;
        float volume;
        bool loop;
    };

    std::unordered_map<std::string, SoundInfo> registeredSounds;
    float masterVolume = 100.0f;
    float musicVolume = 100.0f;
    float effectsVolume = 100.0f;

public:
    ~FakeAudioManager() override
    {
        LOG_DEBUG << "[FakeAudioManager] Destroyed";
    }

    void RegisterSound(const std::string &name, const std::string &filename,
                       SoundType type, float volume = 100.0f, bool loop = false) override
    {
        std::string typeStr = (type == SoundType::EFFECT) ? "EFFECT" : "MUSIC";
        LOG_INFO << "[FakeAudioManager] RegisterSound: " << name
                 << ", file: " << filename
                 << ", type: " << typeStr
                 << ", volume: " << volume
                 << ", loop: " << (loop ? "true" : "false");
        registeredSounds[name] = {filename, type, volume, loop};
    }

    void Play(const std::string &name, float pitch = 1.0f) override
    {
        if (registeredSounds.find(name) != registeredSounds.end())
        {
            LOG_INFO << "[FakeAudioManager] Playing: " << name
                     << ", pitch: " << pitch;
        }
        else
        {
            LOG_WARN << "[FakeAudioManager] Sound not found: " << name;
        }
    }

    void Crossfade(const std::string &name, float duration = 2.0f) override
    {
        LOG_INFO << "[FakeAudioManager] Crossfade to: " << name
                 << ", duration: " << duration << " seconds";
    }

    void StopAll() override
    {
        LOG_INFO << "[FakeAudioManager] Stopping all sounds";
    }

    void Update(float dt) override
    {
        
    }

    float GetMasterVolume() const override
    {
        LOG_DEBUG << "[FakeAudioManager] GetMasterVolume: " << masterVolume;
        return masterVolume;
    }

    float GetMusicVolume() const override
    {
        LOG_DEBUG << "[FakeAudioManager] GetMusicVolume: " << musicVolume;
        return musicVolume;
    }

    float GetEffectsVolume() const override
    {
        LOG_DEBUG << "[FakeAudioManager] GetEffectsVolume: " << effectsVolume;
        return effectsVolume;
    }

    void SetMasterVolume(float v) override
    {
        masterVolume = v;
        LOG_INFO << "[FakeAudioManager] Master volume set to: " << v;
    }

    void SetMusicVolume(float v) override
    {
        musicVolume = v;
        LOG_INFO << "[FakeAudioManager] Music volume set to: " << v;
    }

    void SetEffectsVolume(float v) override
    {
        effectsVolume = v;
        LOG_INFO << "[FakeAudioManager] Effects volume set to: " << v;
    }

    void PauseAll() override
    {
        LOG_INFO << "[FakeAudioManager] All sounds paused";
    }

    void ResumeAll() override
    {
        LOG_INFO << "[FakeAudioManager] All sounds resumed";
    }

    void Stop(const std::string &name) override
    {
        LOG_INFO << "[FakeAudioManager] Stopping sound: " << name;
    }

    // Доп
    size_t GetRegisteredSoundCount() const
    {
        return registeredSounds.size();
    }

    bool IsSoundRegistered(const std::string &name) const
    {
        return registeredSounds.find(name) != registeredSounds.end();
    }
};