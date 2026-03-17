#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "exceptions.hpp"
#include "consts.hpp"

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

    class File
    {
    private:
        std::unique_ptr<Header> header_;
        std::string filename_;
        std::ifstream file_;
        std::streampos dataStartPos_;

    public:
        ~File() { CloseFile(); }

        void OpenFile(const std::string &filename)
        {
            filename_ = filename;
            file_.open(filename_, std::ios::in | std::ios::binary);

            if (!file_.is_open())
            {
                throw Exceptions::FileOpenException(filename);
            }
        }

        std::ifstream &GetFileStream()
        {
            return file_;
        }

        void CloseFile()
        {
            if (file_.is_open())
            {
                file_.close();
            }
        }

        void SetHeader(std::unique_ptr<Header> &header)
        {
            header_ = std::move(header);
            if (!IsSupportedFormat())
            {
                throw Exceptions::WavInvalidFormatException();
            }
        }

        void SetDataStartPos(const std::streampos &dataStartPos)
        {
            dataStartPos_ = dataStartPos;
        }

        const std::streampos &GetDataStartPos() const
        {
            return dataStartPos_;
        }

        std::size_t GetCurrentPosition()
        {
            return static_cast<std::size_t>(file_.tellg() - dataStartPos_) / sizeof(int16_t);
        }

        bool IsSupportedFormat() const
        {
            return (std::string(header_->riff, 4) == kRiff &&
                    std::string(header_->wave, 4) == kWave &&
                    std::string(header_->fmt, 4) == kFmt &&
                    header_->audioFormat == kAudioFormat &&
                    header_->numChannels == kNumChannels &&
                    header_->sampleRate == kSampleRate &&
                    header_->bitsPerSample == kBitsPerSample &&
                    header_->fmtChunkSize == kFmtChunkSize);
        }

        const std::unique_ptr<Header>& GetHeader() const
        {
            return header_;
        }

        std::uint32_t GetNumSamples() const
        {
            return header_->dataChunkSize / (kBitsPerSample / 8);
        }

        float GetDuration() const
        {
            return static_cast<float>(GetNumSamples()) / kSampleRate;
        }

        const std::string &GetFilename() const
        {
            return filename_;
        }
    };
}
