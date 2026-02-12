#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "converter.hpp"

namespace DescriptionGen
{
    namespace
    {
        std::string GenerateConvertersDescription()
        {
            std::stringstream ss;
            ss << "\nAvailable converters:\n";

            auto &factory = Converters::Factory::Instance();
            auto converter_names = factory.GetAvailableConverters();

            for (const auto &name : converter_names)
            {
                auto converter = factory.CreateConverter(name);
                if (converter)
                {
                    ss << "  " << converter->GetDescription() << "\n";
                }
            }

            return ss.str();
        }
    }

    std::string Description()
    {
        return R"(
Sound Processor is a command-line tool for applying various audio effects to WAV files.
Supported formats: PCM, mono, 16-bit, 44100 Hz sampling rate.)" +
               GenerateConvertersDescription() + R"(
Configuration file format:
  - Each line represents one converter in order of application
  - Lines starting with '#' are comments
  - Stream references: $1, $2, etc. correspond to input files in order

Example config.txt:
  mute 38 48        # Silence from 38s to 48s
  mix $1 15         # Mix first input file starting at 15s
  raise 0.3         # Reduce volume to 30%)";
    }

    std::string Footer()
    {
        return R"(
Exit codes:
  0 - Success
  1 - General error
  2 - File open error
  3 - Invalid WAV format
  4 - WAV read error
  5 - Invalid argument)";
    }
}
