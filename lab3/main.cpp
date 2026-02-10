#include <iostream>
#include <string>

#include <CLI11.hpp>

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

    return 0;
}