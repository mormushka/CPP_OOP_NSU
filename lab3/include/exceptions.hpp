#pragma once
#include <iostream>

namespace Exceptions
{
    class Exception : std::exception
    {
    private:
        std::string error_message_;

    public:
        explicit Exception(const std::string &message) : error_message_(message) {}
        const char *what() const noexcept
        {
            return error_message_.c_str();
        }
    };

    class FileOpenException : public Exception
    {
    public:
        explicit FileOpenException(const std::string &filename)
            : Exception("Cannot open file: " + filename) {}
    };

    class InvalidFormatException : public Exception
    {
    public:
        InvalidFormatException()
            : Exception("Unsupported WAV format. Expected: PCM, mono, 16-bit, 44100 Hz") {}
    };

    class ReadException : public Exception
    {
    public:
        ReadException() : Exception("Error reading WAV file") {}
    };
}