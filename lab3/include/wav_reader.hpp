// wav_reader.h
#pragma once

#include <vector>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "wav_header.hpp"
#include "exceptions.hpp"

class WavReader {
private:
    WavHeader header_;
    std::vector<int16_t> samples_;
    std::string filename_;

public:
    explicit WavReader(const std::string& filename);
    
    void Read();

    const WavHeader& GetHeader() const { return header_; }
    
    const std::vector<int16_t>& GetSamples() const { return samples_; }
    
    float GetDuration() const {
        return static_cast<float>(samples_.size()) / header_.sampleRate;
    }
    
    const std::string& GetFilename() const { return filename_; }
};



WavReader::WavReader(const std::string& filename) 
    : filename_(filename) {}

void WavReader::Read() {
    std::ifstream file(filename_, std::ios::binary);
    
    if (!file.is_open()) {
        throw Exceptions::FileOpenException(filename_);
    }
    
    file.read(reinterpret_cast<char*>(&header_), sizeof(WavHeader));
    
    if (!file) {
        throw Exceptions::ReadException();
    }
    
    if (!header_.IsSupportedFormat()) {
        throw Exceptions::InvalidFormatException();
    }
    
    if (std::string(header_.data, 4) != "data") {
        char chunkId[4];
        std::uint32_t chunkSize;
        
        std::string currentChunk(header_.data, 4);
        std::uint32_t currentChunkSize = header_.subchunk2Size;
        
        while (currentChunk != "data") {
            file.seekg(currentChunkSize, std::ios::cur);
            
            file.read(chunkId, 4);
            file.read(reinterpret_cast<char*>(&chunkSize), 4);
            
            if (!file) {
                throw Exceptions::ReadException();
            }
            
            currentChunk = std::string(chunkId, 4);
            currentChunkSize = chunkSize;
        }
        
        header_.subchunk2Size = currentChunkSize;
    }
    
    std::uint32_t numSamples = header_.GetNumSamples();
    samples_.resize(numSamples);
    
    for (std::uint32_t i = 0; i < numSamples; ++i) {
        int16_t sample;
        file.read(reinterpret_cast<char*>(&sample), sizeof(sample));
        
        if (!file) {
            samples_.resize(i);
            std::cerr << "Warning" << std::endl;
            break;
        }
        
        samples_[i] = sample;
    }
    
    file.close();
}