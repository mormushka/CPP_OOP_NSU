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
    std::shared_ptr<WavFile::File> ReadHeader(const std::string &filename);

    std::vector<int16_t> ReadSamplesChunk(
        std::shared_ptr<WavFile::File> const f,
        std::size_t chunk_size);

    std::vector<int16_t> ReadNextSeconds(
        std::shared_ptr<WavFile::File> const f,
        std::size_t seconds);
};

std::vector<int16_t> WavReader::ReadNextSeconds(
    std::shared_ptr<WavFile::File> const f,
    std::size_t seconds)
{
    return ReadSamplesChunk(f, seconds * f->SampleRate());
}

std::vector<int16_t> WavReader::ReadSamplesChunk(
    std::shared_ptr<WavFile::File> const f,
    std::size_t chunk_size)
{
    std::vector<int16_t> chunk;

    if (f->GetFileStream().eof())
        return chunk;

    std::size_t samples_left = f->GetNumSamples() - f->GetCurrentPosition();
    std::size_t samples_to_read = std::min(chunk_size, samples_left);

    if (samples_to_read == 0)
        return chunk;

    chunk.resize(samples_to_read);

    f->GetFileStream().read(reinterpret_cast<char *>(chunk.data()),
                            samples_to_read * sizeof(int16_t));

    if (!f->GetFileStream())
        throw Exceptions::ReadException();

    return chunk;
}

std::shared_ptr<WavFile::File> WavReader::ReadHeader(const std::string &filename)
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

    wavFile->SetDataStartPos(wavFile->GetFileStream().tellg());

    try
    {
        wavFile->SetHeader(header);
    }
    catch (const Exceptions::Exception &e)
    {
        throw e;
    }

    return wavFile;
}