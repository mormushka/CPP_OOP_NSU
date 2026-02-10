#include <iostream>
#include <string>

#include <CLI11.hpp>

#include "wav_reader.hpp"
#include "exceptions.hpp"

int main(int argc, char *argv[])
{
    CLI::App app{"options"};

    std::string config;
    std::string output;
    std::vector<std::string> inputs;

    app.add_option("-c,--config", config, "Config file")->required();
    app.add_option("-o,--output", output, "Output file")->required();
    app.add_option("-i,--inputs", inputs, "Input files")->required();

    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e)
    {
        return app.exit(e);
    }

    try
    {
        WavReader reader(inputs[0]);
        reader.Read();

        const auto &header = reader.GetHeader();
        const auto &samples = reader.GetSamples();

        std::cout << "File: " << reader.GetFilename() << std::endl
                  << "Duration: " << reader.GetDuration() << " seconds" << std::endl
                  << "Sample rate: " << header.sampleRate << " Hz" << std::endl
                  << "Channels: " << header.numChannels << std::endl
                  << "Bits per sample: " << header.bitsPerSample << std::endl
                  << "Number of samples: " << samples.size() << std::endl
                  << "Data size: " << header.subchunk2Size << " bytes" << std::endl;

        std::cout << "\nFirst 10 samples:" << std::endl;
        for (size_t i = 0; i < std::min(samples.size(), size_t(10)); ++i)
        {
            std::cout << "Sample " << i << ": " << samples[i] << std::endl;
        }
    }
    catch (const Exceptions::Exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}