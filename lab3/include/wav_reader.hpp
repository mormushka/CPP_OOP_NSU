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
    auto wavFile = std::make_shared<WavFile::File>();

    try
    {
        wavFile->OpenFile(filename, WavFile::Options::in);
    }
    catch (const Exceptions::Exception &e)
    {
        throw e;
    }

    auto header = std::make_unique<WavFile::Header>();
    wavFile->GetFileStream().read(reinterpret_cast<char *>(header.get()), sizeof(WavFile::Header));

    if (!wavFile->GetFileStream())
        throw Exceptions::ReadException();

    while (std::string(header->data, 4) != "data")
    {
        wavFile->GetFileStream().seekg(header->dataChunkSize, std::ios::cur);

        wavFile->GetFileStream().read(header->data, 4);
        wavFile->GetFileStream().read(reinterpret_cast<char *>(&header->dataChunkSize), 4);

        if (!wavFile->GetFileStream())
            throw Exceptions::ReadException();
    }
    
    try
    {
        wavFile->SetHeader(header);
    }
    catch (const Exceptions::Exception &e)
    {
        throw e;
    }

    std::uint32_t numSamples = wavFile->GetNumSamples();
    wavFile->samples_.resize(numSamples);

    for (std::uint32_t i = 0; i < numSamples; ++i)
    {
        int16_t sample;
        wavFile->GetFileStream().read(reinterpret_cast<char *>(&sample), sizeof(sample));

        if (!wavFile->GetFileStream())
        {
            wavFile->samples_.resize(i);
            std::cerr << "Warning" << std::endl;
            break;
        }

        wavFile->samples_[i] = sample;
    }

    return wavFile;
}