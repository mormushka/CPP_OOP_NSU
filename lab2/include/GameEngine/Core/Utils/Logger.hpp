#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <ctime>
#include <iomanip>

#define LOG_ON

enum class LogLevel
{
    Debug,
    Info,
    Warning,
    Error
};

class Logger
{
public:
    static Logger &Instance()
    {
        static Logger instance;
        return instance;
    }

    void SetLogFile(const std::string &filename)
    {
        fileStream_.open(filename, std::ios::out | std::ios::app);
    }

    void SetLevel(LogLevel level)
    {
        logLevel_ = level;
    }

    void Write(LogLevel level, const std::string &message)
    {
        if (level >= logLevel_)
        {
            std::ostringstream oss;
            oss << "[" << CurrentTime() << "] "
                << LevelToString(level) << " "
                << message << "\n";

            std::string out = oss.str();
            std::cout << out;
            if (fileStream_.is_open())
            {
                fileStream_ << out;
                fileStream_.flush();
            }
        }
    }

    class LogStream
    {
    public:
        LogStream(Logger &logger, LogLevel level) 
            : logger_(logger), level_(level) 
        {
        }
        
        ~LogStream()
        {
            if (!buffer_.str().empty())
            {
                logger_.Write(level_, buffer_.str());
            }
        }
        
        template <typename T>
        LogStream &operator<<(const T &msg)
        {
            buffer_ << msg;
            return *this;
        }
        
        LogStream &operator<<(std::ostream &(*manip)(std::ostream &))
        {
            buffer_ << manip;
            return *this;
        }
        
    private:
        Logger &logger_;
        LogLevel level_;
        std::ostringstream buffer_;
    };
    
    LogStream Stream(LogLevel level)
    {
        return LogStream(*this, level);
    }

private:
    Logger() : logLevel_(LogLevel::Debug) {}
    ~Logger()
    {
        if (fileStream_.is_open())
            fileStream_.close();
    }

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    std::string CurrentTime()
    {
        auto now = std::time(nullptr);
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &now);
#else
        localtime_r(&now, &tm);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string LevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug:
            return "[DEBUG]";
        case LogLevel::Info:
            return "[INFO]";
        case LogLevel::Warning:
            return "[WARN]";
        case LogLevel::Error:
            return "[ERROR]";
        }
        return "[UNKNOWN]";
    }

    LogLevel logLevel_;
    std::ofstream fileStream_;
};

#ifdef LOG_ON
#define LOG_DEBUG Logger::Instance().Stream(LogLevel::Debug)
#define LOG_INFO Logger::Instance().Stream(LogLevel::Info)
#define LOG_WARN Logger::Instance().Stream(LogLevel::Warning)
#define LOG_ERROR Logger::Instance().Stream(LogLevel::Error)
#else
// Пустые макросы, когда логирование отключено
class NullStream
{
public:
    template <typename T>
    NullStream &operator<<(const T &) { return *this; }
    
    NullStream &operator<<(std::ostream &(*)(std::ostream &)) { return *this; }
};

#define LOG_DEBUG NullStream()
#define LOG_INFO NullStream()
#define LOG_WARN NullStream()
#define LOG_ERROR NullStream()
#endif