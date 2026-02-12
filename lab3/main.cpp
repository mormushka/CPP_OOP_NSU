#include <iostream>
#include <string>

#include <CLI11.hpp>

#include "wav_file.hpp"
#include "wav_reader.hpp"
#include "exceptions.hpp"
#include "converter.hpp"
#include "config_parser.hpp"
#include "description_gen.hpp"

int main(int argc, char *argv[])
{
    CLI::App app{"sound processor"};

    app.description(DescriptionGen::Description());

    app.footer(DescriptionGen::Footer());

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
        std::vector<std::shared_ptr<WavFile::File>> loadedFiles;

        for (size_t i = 0; i < inputs.size(); ++i)
            loadedFiles.push_back(reader.ReadHeader(inputs[i]));

        std::ofstream outFile(output, std::ios::binary);
        if (!outFile.is_open())
            throw Exceptions::FileOpenException(output);

        outFile.write(reinterpret_cast<char *>(loadedFiles[0]->GetHeaderRAW()), sizeof(WavFile::Header));

        auto converters = ConfigParser::ParseConfigFile(config, loadedFiles);

        size_t numFiles = loadedFiles.size();
        auto samples = std::make_shared<std::vector<std::vector<int16_t>>>(numFiles);
        for (auto &in_v : *samples)
            in_v.resize(WavFile::kSampleRate);

        size_t maxSec = loadedFiles[0]->GetDuration();
        for (size_t sec = 0; sec < maxSec; sec++)
        {
            for (size_t f = 0; f < numFiles; f++)
                (*samples)[f] = reader.ReadNextSeconds(loadedFiles[f], 1);

            auto outSample = (*samples)[0];

            for (auto &c : converters)
                c->Process(outSample, samples, sec);

            outFile.write(reinterpret_cast<char *>(outSample.data()),
                          outSample.size() * sizeof(int16_t));
        }

        outFile.close();

        std::cout << "Processing completed successfully!\n";
    }
    catch (const Exceptions::Exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return e.Code();
    }

    return 0;
}