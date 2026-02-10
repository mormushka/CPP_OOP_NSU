#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "exceptions.hpp"

namespace WavFile
{
    struct Header
    {
        char riff[4];
        std::uint32_t riffChunkSize;

        char wave[4];

        char fmt[4];
        std::uint32_t fmtChunkSize;
        std::uint16_t audioFormat;
        std::uint16_t numChannels;
        std::uint32_t sampleRate;
        std::uint32_t byteRate;
        std::uint16_t blockAlign;
        std::uint16_t bitsPerSample;

        char data[4];
        std::uint32_t dataChunkSize;
    };

    enum class Options
    {
        in,
        out,
        empty,
    };

    class File
    {
    private:
        std::unique_ptr<Header> header_;
        std::string filename_;
        std::ifstream file_;
        Options option_;

    public:
        std::vector<int16_t> samples_;

        File() {}

        void OpenFile(const std::string &filename, Options option)
        {
            filename_ = filename;
            option_ = option;
            if (option_ == Options::in)
            {
                file_.open(filename_, std::ios::in | std::ios::binary);
            }
            else
            {
                file_.open(filename_, std::ios::out | std::ios::binary);
            }

            if (!file_.is_open())
                throw Exceptions::FileOpenException(filename);
        }

        std::ifstream &GetFileStream()
        {
            return file_;
        }

        void CloseFile()
        {
            file_.close();
        }

        void SetHeader(std::unique_ptr<Header> &header)
        {
            header_ = std::move(header);

            if (!IsSupportedFormat())
                throw Exceptions::InvalidFormatException();
        }

        bool IsSupportedFormat() const
        {
            return (std::string(header_->riff, 4) == "RIFF" &&
                    std::string(header_->wave, 4) == "WAVE" &&
                    std::string(header_->fmt, 4) == "fmt " &&
                    header_->audioFormat == 1 &&
                    header_->numChannels == 1 &&
                    header_->sampleRate == 44100 &&
                    header_->bitsPerSample == 16 &&
                    header_->fmtChunkSize == 16);
        }

        Header *GetHeaderTEST()
        {
            return header_.get();
        }

        std::uint32_t GetNumSamples() const
        {
            return header_->dataChunkSize / (header_->bitsPerSample / 8);
        }

        const std::vector<int16_t> &GetSamples() const { return samples_; }

        float GetDuration() const
        {
            return static_cast<float>(samples_.size()) / header_->sampleRate;
        }

        const std::string &GetFilename() const { return filename_; }
    };
}
