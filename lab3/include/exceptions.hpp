#pragma once

#include <iostream>

namespace Exceptions
{
    enum Code
    {
        kEror = 1,
        kFileOpenException,
        kWavInvalidFormatException,
        kWavReadException,
        kInvalidArgument
    };

    class Exception : std::exception
    {
    private:
        std::string error_message_;

    public:
        explicit Exception(const std::string &message) : error_message_(message) {}

        const char *what() const noexcept { return error_message_.c_str(); }

        virtual int Code() const noexcept { return Code::kEror; }
    };

    class FileOpenException : public Exception
    {
    public:
        explicit FileOpenException(const std::string &filename)
            : Exception("Cannot open file: " + filename) {}
        int Code() const noexcept override { return Code::kFileOpenException; }
    };

    class WavInvalidFormatException : public Exception
    {
    public:
        WavInvalidFormatException()
            : Exception("Unsupported WAV format. Expected: PCM, mono, 16-bit, 44100 Hz") {}
        int Code() const noexcept override { return Code::kWavInvalidFormatException; }
    };

    class WavReadException : public Exception
    {
    public:
        WavReadException() : Exception("Error reading WAV file") {}
        int Code() const noexcept override { return Code::kWavReadException; }
    };

    class InvalidArgument : public Exception
    {
    public:
        InvalidArgument(const std::string &message) : Exception(message) {}
        int Code() const noexcept override { return Code::kInvalidArgument; }
    };
}