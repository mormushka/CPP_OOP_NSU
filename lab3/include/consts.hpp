#pragma once

#include <string>
#include <vector>
#include <set>

namespace WavFile
{
    enum Options
    {
        in,
        out,
        empty,
    };

    inline const std::string kRiff = "RIFF";

    inline const std::string kWave = "WAVE";

    inline const std::string kFmt = "fmt ";
    inline constexpr std::uint32_t kFmtChunkSize = 16;
    inline constexpr std::uint16_t kAudioFormat = 1;
    inline constexpr std::uint16_t kNumChannels = 1;
    inline constexpr std::uint32_t kSampleRate = 44100;
    inline constexpr std::uint16_t kBitsPerSample = 16;

    inline const std::string kData = "data";
}

namespace Converters
{
    
}

namespace Config
{
  
}
