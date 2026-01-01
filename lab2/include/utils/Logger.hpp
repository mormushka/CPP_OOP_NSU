#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#define NOMINMAX
#include <windows.h>

class Logger
{
private:
    std::ofstream file;

    Logger(const std::string &filename = "debug.log")
    {
        openConsole();
        file.open(filename, std::ios::out | std::ios::app);
        if (!file)
        {
            std::cerr << "Failed to open log file!" << std::endl;
        }
    }

    ~Logger()
    {
        if (file.is_open())
            file.close();
    }

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void openConsole()
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        std::cout << "Debug console ready!" << std::endl;
    }

    std::string timestamp()
    {
        std::time_t t = std::time(nullptr);
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        return buf;
    }

    void log(const std::string &level, const std::string &msg, std::ostream &out)
    {
        std::string line = "[" + level + "] " + timestamp() + " - " + msg;
        out << line << std::endl;
        if (file.is_open())
            file << line << std::endl;
    }

public:
    static Logger &getInstance()
    {
        static Logger instance; // создаётся один раз
        return instance;
    }

    void info(const std::string &msg) { log("INFO", msg, std::cout); }
    void warn(const std::string &msg) { log("WARN", msg, std::cout); }
    void error(const std::string &msg) { log("ERROR", msg, std::cerr); }
};

// Макросы для удобного использования
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_VEC(vec) Logger::getInstance().info("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")")
#define LOG_WARN(msg) Logger::getInstance().warn(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)
