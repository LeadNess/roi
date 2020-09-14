#include <iostream>
#include <string>
#include <args.hpp>

struct GeneratorArgs {
    std::string _cfgFileName;
    std::string _outputFilesName;

    GeneratorArgs() = default;

    static const char* help() {
        return "Generates data for algorithm that removes unnecessary edges in graph";
    }

    template<class F>
    void parse(F f) {
        f(_outputFilesName, "--output", "-o", args::help("Output files names prefix"), args::required());
        f(_cfgFileName, "--config", "-c", args::help("Config file name"), args::required());
    }

    void run() {
        std::cout << "Config file: " << _cfgFileName << std::endl;
        std::cout << "Output files name prefix: " << _outputFilesName << std::endl;
    }
};
