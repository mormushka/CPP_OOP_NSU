// SFMLAudioManager.h
#pragma once
#include "IAudioManager.h"
#include "ObjectPool.h"
#include "Logger.hpp"
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>

struct SoundInfo
{
    std::string name;
    std::string filename;
    SoundType type;
    float volume;
    bool loop;

    SoundInfo(const std::string& n = "", const std::string& f = "", 
              SoundType t = SoundType::EFFECT, float v = 100.0f, bool l = false)
        : name(n), filename(f), type(t), volume(v), loop(l) {}
};

class SFMLMusicPlayer
{
private:
    std::unique_ptr<sf::Music> currentMusic;
    std::unique_ptr<sf::Music> nextMusic;
    std::string currentTrack;
    std::string nextTrack;

    float baseVolume = 100.0f;
    float fadeDuration = 2.0f;
    float fadeTimer = 0.0f;
    bool isCrossfading = false;

public:
    SFMLMusicPlayer()
    {
        currentMusic = std::make_unique<sf::Music>();
        nextMusic = std::make_unique<sf::Music>();
    }

    void Play(const SoundInfo& info, float masterVolume, float musicVolume)
    {
        if (!currentMusic->openFromFile(info.filename))
        {
            LOG_ERROR << "Failed to load music: " << info.filename;
            return;
        }
        baseVolume = info.volume;
        currentTrack = info.name;

        float calculatedVolume = baseVolume * (musicVolume / 100.0f) * (masterVolume / 100.0f);
        currentMusic->setVolume(calculatedVolume);
        currentMusic->setLoop(info.loop);
        currentMusic->play();
    }

    void CrossfadePlay(const SoundInfo& info, float masterVolume, float musicVolume, float duration = 2.0f)
    {
        if (!nextMusic->openFromFile(info.filename))
        {
            LOG_ERROR << "Failed to load next music: " << info.filename;
            return;
        }

        if(info.name == currentTrack) 
        {
            return;
        }

        nextTrack = info.name;
        baseVolume = info.volume;
        fadeDuration = duration;
        fadeTimer = 0.0f;
        isCrossfading = true;

        nextMusic->setVolume(0.0f);
        nextMusic->setLoop(info.loop);
        nextMusic->play();
    }

    void Update(float dt, float masterVolume, float musicVolume)
    {
        if (isCrossfading)
        {
            fadeTimer += dt;
            float t = std::min(fadeTimer / fadeDuration, 1.0f);

            float targetVolume = baseVolume * (musicVolume / 100.0f) * (masterVolume / 100.0f);

            currentMusic->setVolume((1.0f - t) * targetVolume);
            nextMusic->setVolume(t * targetVolume);

            if (t >= 1.0f)
            {
                currentMusic->stop();
                std::swap(currentMusic, nextMusic);
                currentTrack = nextTrack;
                isCrossfading = false;
            }
        }
        else
        {
            float calculatedVolume = baseVolume * (musicVolume / 100.0f) * (masterVolume / 100.0f);
            currentMusic->setVolume(calculatedVolume);
        }
    }

    void Stop() { currentMusic->stop(); }
    void Pause() { currentMusic->pause(); }
    void Resume() { currentMusic->play(); }
    bool IsPlaying() const { return currentMusic->getStatus() == sf::Music::Playing; }
    const std::string& GetCurrentTrack() const { return currentTrack; }
};

struct ActiveEffect
{
    sf::Sound* sound;
    float baseVolume;
};

class SFMLEffectPlayer
{
private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    ObjectPool<sf::Sound> soundPool;
    std::vector<ActiveEffect> activeEffects;

public:
    SFMLEffectPlayer(size_t initialSize = 32, size_t maxSize = 128)
        : soundPool(initialSize, maxSize) {}

    void Play(const SoundInfo& info, float masterVolume, float effectsVolume, float pitch = 1.0f)
    {
        if (buffers.find(info.name) == buffers.end())
        {
            sf::SoundBuffer buffer;
            if (!buffer.loadFromFile(info.filename))
            {
                LOG_ERROR << "Failed to load effect: " << info.filename;
                return;
            }
            buffers[info.name] = buffer;
        }

        sf::Sound* sound = soundPool.Acquire([&](sf::Sound& s)
                                             {
            s.setBuffer(buffers[info.name]);
            s.setPitch(pitch);
            float calculatedVolume = info.volume * (effectsVolume / 100.f) * (masterVolume / 100.f);
            s.setVolume(calculatedVolume);
            s.setLoop(info.loop);
            s.play(); });

        if (!sound)
        {
            LOG_WARN << "Sound pool limit reached!";
        }
        else
        {
            activeEffects.push_back({sound, info.volume});
            LOG_DEBUG << "Playing effect: " << info.name;
        }
    }

    void Update(float masterVolume, float effectsVolume)
    {
        soundPool.Update([](sf::Sound* s)
                         { return s->getStatus() == sf::Sound::Stopped; });

        activeEffects.erase(
            std::remove_if(activeEffects.begin(), activeEffects.end(),
                           [](const ActiveEffect& ae)
                           { return ae.sound->getStatus() == sf::Sound::Stopped; }),
            activeEffects.end());

        for (auto& ae : activeEffects)
        {
            float calculatedVolume = ae.baseVolume * (effectsVolume / 100.f) * (masterVolume / 100.f);
            ae.sound->setVolume(calculatedVolume);
        }
    }

    void StopAll()
    {
        for (auto* s : soundPool.GetActive())
            s->stop();
        soundPool.Update([](sf::Sound* s) { return true; });
    }

    void Stop(const std::string& name)
    {
        for (auto* s : soundPool.GetActive())
        {
            if (s->getBuffer() == &buffers[name])
            {
                s->stop();
            }
        }
    }
};

class SFMLAudioManager : public IAudioManager
{
private:
    SFMLMusicPlayer musicPlayer;
    SFMLEffectPlayer effectPlayer;
    std::unordered_map<std::string, SoundInfo> registry;

    float masterVolume = 100.0f;
    float musicVolume = 100.0f;
    float effectsVolume = 100.0f;

public:
    SFMLAudioManager() = default;

    void RegisterSound(const std::string& name, const std::string& filename,
                       SoundType type, float volume = 100.0f, bool loop = false) override
    {
        registry[name] = SoundInfo(name, filename, type, volume, loop);
    }

    void Play(const std::string& name, float pitch = 1.0f) override
    {
        auto it = registry.find(name);
        if (it == registry.end())
        {
            LOG_WARN << "Sound not registered: " << name;
            return;
        }

        const SoundInfo& info = it->second;
        if (info.type == SoundType::MUSIC)
            musicPlayer.Play(info, masterVolume, musicVolume);
        else
            effectPlayer.Play(info, masterVolume, effectsVolume, pitch);
    }

    void Crossfade(const std::string& name, float duration = 2.0f) override
    {
        auto it = registry.find(name);
        if (it == registry.end())
        {
            LOG_WARN << "Sound not registered: " << name;
            return;
        }

        const SoundInfo& info = it->second;
        if (info.type != SoundType::MUSIC)
        {
            LOG_WARN << "Crossfade only works for music tracks!";
            return;
        }

        musicPlayer.CrossfadePlay(info, masterVolume, musicVolume, duration);
    }

    void StopAll() override
    {
        musicPlayer.Stop();
        effectPlayer.StopAll();
    }

    void Update(float dt) override
    {
        effectPlayer.Update(masterVolume, effectsVolume);
        musicPlayer.Update(dt, masterVolume, musicVolume);
    }

    // Volume controls
    float GetMasterVolume() const override { return masterVolume; }
    float GetMusicVolume() const override { return musicVolume; }
    float GetEffectsVolume() const override { return effectsVolume; }

    void SetMasterVolume(float v) override { masterVolume = std::clamp(v, 0.0f, 100.0f); }
    void SetMusicVolume(float v) override { musicVolume = std::clamp(v, 0.0f, 100.0f); }
    void SetEffectsVolume(float v) override { effectsVolume = std::clamp(v, 0.0f, 100.0f); }

    void PauseAll() override
    {
        musicPlayer.Pause();
        // Эффекты в SFML...
    }

    void ResumeAll() override
    {
        musicPlayer.Resume();
        // Эффекты в SFML...
    }

    void Stop(const std::string& name) override
    {
        auto it = registry.find(name);
        if (it == registry.end()) return;

        if (it->second.type == SoundType::MUSIC && 
            musicPlayer.GetCurrentTrack() == name)
        {
            musicPlayer.Stop();
        }
        else if (it->second.type == SoundType::EFFECT)
        {
            effectPlayer.Stop(name);
        }
    }
};