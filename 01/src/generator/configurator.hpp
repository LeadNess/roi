#include <iostream>
#include <string>
#include <args.hpp>
#include "generator.hpp"

struct GeneratorArgs {
    string _cfgFileName;
    string _outputFilesNamePrefix;

    GeneratorArgs() = default;

    static const char* help() {
        return "Generates data for algorithm that removes unnecessary edges in graph";
    }

    template<class F>
    void parse(F f) {
        f(_cfgFileName, "--config", "-c", args::help("Config file name"), args::required());
        f(_outputFilesNamePrefix, "--output", "-o", args::help("Output files names prefix"), args::required());
    }

    void run() {
        std::cout << "Config file: " << _cfgFileName << std::endl;
        std::cout << "Output files name prefix: " << _outputFilesNamePrefix << std::endl;

        auto vecCfg = parseConfig(_cfgFileName);
        for (auto& cfg : vecCfg) {
            auto gen = Generator(cfg.edgesCount, cfg.maxNodesCount, cfg.minEdgeWeight, cfg.maxEdgeWeight);
            gen.generateData(_outputFilesNamePrefix);
        }
    }
};
