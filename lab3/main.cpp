#include <iostream>
#include <string>

#include <CLI11.hpp>

#include "wav_file.hpp"
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
        WavReader reader;
        auto wavFile = reader.ReadHeader(inputs[0]);

        const auto header = wavFile->GetHeaderTEST();

        std::cout << "File: " << wavFile->GetFilename() << std::endl
                  << "Duration: " << wavFile->GetDuration() << " seconds" << std::endl
                  << "Sample rate: " << header->sampleRate << " Hz" << std::endl
                  << "Channels: " << header->numChannels << std::endl
                  << "Bits per sample: " << header->bitsPerSample << std::endl
                  << "Number of samples: " << wavFile->GetNumSamples() << std::endl
                  << "Data size: " << header->dataChunkSize << " bytes" << std::endl;

        std::cout << "\nSamples:" << std::endl;
        size_t c = 0;
        for (size_t i = 0; i < wavFile->GetDuration(); i++)
        {
            std::cout << "---SECOND--- " << i << std::endl;
            for (auto &s : reader.ReadNextSeconds(wavFile, 1))
            {
                std::cout << "Sample " << ++c << ": " << s << std::endl;
            }
            std::cout << "Press Enter to continue...";
            std::string dumb;
            std::getline(std::cin, dumb);
        }
    }
    catch (const Exceptions::Exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}