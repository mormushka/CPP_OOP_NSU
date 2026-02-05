// AudioInitializer.h
#pragma once
#include "IAudioManager.h"
#include "Logger.hpp"

namespace AudioInitializer
{
    void Initialize(IAudioManager &audio)
    {
        // Регистрация файловых звуков
        // Музыка для сцен
        audio.RegisterSound("menu_music", "resources/audio/music/game-music-7408.mp3",
                            SoundType::MUSIC, 70.0f, true);
        audio.RegisterSound("game_music", "resources/audio/music/8-bit-arcade-mode-158814.mp3",
                            SoundType::MUSIC, 60.0f, true);
        audio.RegisterSound("gameover_music", "resources/audio/music/game-over-retrowaver-synthwave-vaporwave-retro-80s-198365.mp3",
                            SoundType::MUSIC, 80.0f, false);

        // Звуковые эффекты
        audio.RegisterSound("button_click", "resources/audio/vine-boom.mp3",
                            SoundType::EFFECT, 80.0f);
        audio.RegisterSound("laser_shoot", "resources/audio/vine-boom.mp3",
                            SoundType::EFFECT, 60.0f);
        audio.RegisterSound("explosion", "resources/audio/vine-boom.mp3",
                            SoundType::EFFECT, 70.0f);
        audio.RegisterSound("asteroid_hit", "resources/audio/vine-boom.mp3",
                            SoundType::EFFECT, 65.0f);
        audio.RegisterSound("powerup", "resources/audio/vine-boom.mp3",
                            SoundType::EFFECT, 75.0f);
        audio.RegisterSound("game_over", "resources/audio/vine-boom.mp3",
                            SoundType::EFFECT, 85.0f);

        LOG_INFO << "Audio system initialized";
    }
}