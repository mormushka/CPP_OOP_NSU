#pragma once

#include <vector>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "wav_file.hpp"
#include "exceptions.hpp"

class WavReader
{
public:
    std::shared_ptr<WavFile::File> Read(const std::string &filename);
};

std::shared_ptr<WavFile::File> WavReader::Read(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open())
    {
        throw Exceptions::FileOpenException(filename);
    }

    auto header = std::make_unique<WavFile::Header>();
    file.read(reinterpret_cast<char *>(header.get()), sizeof(WavFile::Header));

    if (!file)
    {
        throw Exceptions::ReadException();
    }

    while (std::string(header->data, 4) != "data")
    {
        file.seekg(header->dataChunkSize, std::ios::cur);

        file.read(header->data, 4);
        file.read(reinterpret_cast<char *>(&header->dataChunkSize), 4);

        if (!file)
        {
            throw Exceptions::ReadException();
        }
    }

    auto wavFile = std::make_shared<WavFile::File>(filename, header);

    if (!wavFile->IsSupportedFormat())
        throw Exceptions::InvalidFormatException();

    std::uint32_t numSamples = wavFile->GetNumSamples();
    wavFile->samples_.resize(numSamples);

    for (std::uint32_t i = 0; i < numSamples; ++i)
    {
        int16_t sample;
        file.read(reinterpret_cast<char *>(&sample), sizeof(sample));

        if (!file)
        {
            wavFile->samples_.resize(i);
            std::cerr << "Warning" << std::endl;
            break;
        }

        wavFile->samples_[i] = sample;
    }

    file.close();

    return wavFile;
}