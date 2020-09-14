#include <iostream>
#include <string>
#include <args.hpp>

struct GeneratorArgs {
    std::string outputFileName;
    std::string cfgFileName;

    GeneratorArgs() = default;

    static const char* help() {
        return "Generates data for algorithm that removes unnecessary edges in graph";
    }

    template<class F>
    void parse(F f) {
        f(cfgFileName, "--config", "-c", args::help("Config file name."), args::required());
        f(outputFileName, "--output", "-o", args::help("Output file name."));
    }

    void run() {
        if (outputFileName.empty()) {
            std::cout << "Output file name is not set - generated data will be output to stdout." << std::endl;
        } else {
            std::cout << "Output file: " << outputFileName << std::endl;

        }
        std::cout << "Config file: " << cfgFileName << std::endl;
    }
};
