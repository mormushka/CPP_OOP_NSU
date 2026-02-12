#pragma once

#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <memory>
#include <string>

#include "converter.hpp"
#include "exceptions.hpp"

namespace ConfigParser
{
    class Parser
    {
    private:
        std::vector<std::shared_ptr<WavFile::File>> loadedFiles_;

        std::vector<std::string> SplitLine(const std::string &line)
        {
            std::vector<std::string> tokens;
            std::istringstream iss(line);
            std::string token;

            while (iss >> token)
            {
                if (token.empty() || token[0] == '#')
                    break;
                tokens.push_back(token);
            }

            return tokens;
        }

        bool IsCommentOrEmpty(const std::string &line)
        {
            return line.empty() || line[0] == '#';
        }

        std::string ExpandReference(const std::string &param)
        {
            if (param.empty() || param[0] != '$')
                return param;

            try
            {
                int stream_num = std::stoi(param.substr(1));
                if (stream_num < 1)
                    throw Exceptions::InvalidArgument("Stream number must be positive");

                if (static_cast<size_t>(stream_num - 1) >= loadedFiles_.size())
                    throw Exceptions::InvalidArgument("Stream reference $" + std::to_string(stream_num) +
                                                      " exceeds available input files (" +
                                                      std::to_string(loadedFiles_.size()) + ")");

                return std::to_string(stream_num - 1);
            }
            catch (const std::exception &e)
            {
                throw Exceptions::InvalidArgument("Invalid stream reference: " + param);
            }
        }

    public:
        explicit Parser(const std::vector<std::shared_ptr<WavFile::File>> &loadedFiles)
            : loadedFiles_(loadedFiles) {}

        std::vector<std::unique_ptr<Converters::IConverter>> ParseConfig(const std::string &configPath)
        {
            std::vector<std::unique_ptr<Converters::IConverter>> converters;
            std::ifstream configFile(configPath);

            if (!configFile.is_open())
                throw Exceptions::FileOpenException(configPath);

            std::string line;
            int line_num = 0;
            auto &factory = Converters::Factory::Instance();

            while (std::getline(configFile, line))
            {
                ++line_num;

                if (IsCommentOrEmpty(line))
                    continue;

                auto tokens = SplitLine(line);
                if (tokens.empty())
                    continue;

                std::string converter_name = tokens[0];
                auto converter = factory.CreateConverter(converter_name);

                if (!converter)
                    throw  Exceptions::InvalidArgument("Unknown converter '" + converter_name +
                                                "' at line " + std::to_string(line_num));

                std::vector<std::string> params;
                for (size_t i = 1; i < tokens.size(); ++i)
                {
                    try
                    {
                        params.push_back(ExpandReference(tokens[i]));
                    }
                    catch (const Exceptions::InvalidArgument &e)
                    {
                        throw  Exceptions::InvalidArgument("Error at line " + std::to_string(line_num) +
                                                    ": " + e.what());
                    }
                }

                try
                {
                    converter->SetParameters(params);
                }
                catch (const Exceptions::Exception &e)
                {
                    std::cerr << "Error at line " + std::to_string(line_num) << std::endl;
                    throw;
                }

                converters.push_back(std::move(converter));
            }

            if (converters.empty())
                throw Exceptions::Exception("No converters specified in config file");

            return converters;
        }
    };

    inline std::vector<std::unique_ptr<Converters::IConverter>> ParseConfigFile(
        const std::string &configPath,
        const std::vector<std::shared_ptr<WavFile::File>> &loadedFiles)
    {
        Parser parser(loadedFiles);
        return parser.ParseConfig(configPath);
    }
}