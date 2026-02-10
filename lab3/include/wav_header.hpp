#pragma once

#include <cstdint>
#include <string>
#include <fstream>

struct WavHeader {
    char riff[4];
    std::uint32_t chunkSize;
    char wave[4];
    
    char fmt[4];
    std::uint32_t subchunk1Size;
    std::uint16_t audioFormat;
    std::uint16_t numChannels;
    std::uint32_t sampleRate;
    std::uint32_t byteRate;
    std::uint16_t blockAlign;
    std::uint16_t bitsPerSample;
    
    char data[4];
    std::uint32_t subchunk2Size;
    
    bool IsSupportedFormat() const {
        return (std::string(riff, 4) == "RIFF" &&
                std::string(wave, 4) == "WAVE" &&
                std::string(fmt, 4) == "fmt " &&
                audioFormat == 1 &&
                numChannels == 1 &&
                sampleRate == 44100 &&
                bitsPerSample == 16 &&
                subchunk1Size == 16);
    }
    
    std::uint32_t GetNumSamples() const {
        return subchunk2Size / (bitsPerSample / 8);
    }
};