#include <iostream>
#include <string>
#include <args.hpp>
#include <mutex>
#include "multithread.hpp"

struct MultiThreadAlgorithmArgs {
    string _inputFileNamePrefix;
    string _outputFileName;
    string _cfgFileName;

    MultiThreadAlgorithmArgs() = default;

    static const char* help() {
        return "Algorithm removing unnecessary edges in graph";
    }

    template<class F>
    void parse(F f) {
        f(_inputFileNamePrefix, "--input", "-i", args::help("Input files names prefix"), args::required());
        f(_outputFileName, "--output", "-o", args::help("Output file name"), args::required());
        f(_cfgFileName, "--config", "-c", args::help("Config file name"));
    }

    void run() {
        std::cout << "Input files names prefix: " << _inputFileNamePrefix << std::endl;
        std::cout << "Output file name: " << _outputFileName << std::endl;
        if (_cfgFileName.empty()) {
            _cfgFileName = "parameters.cfg";
        }
        std::cout << "Config file: " << _cfgFileName << std::endl;

        auto vecCfg = parseConfig(_cfgFileName);
        MultiThread::ResultsFile resultsFile(_outputFileName, vecCfg.size());
        for (int i = 0; i < vecCfg.size(); i++) {
            auto inputFileName = _inputFileNamePrefix + std::to_string(vecCfg[i].edgesCount) + ".txt";
            thread(processGraph, inputFileName, std::ref(resultsFile), i).detach();
        }
        std::unique_lock<std::mutex> lock(MultiThread::mainThreadLock);
        while(!MultiThread::algorithmIsDone) {
            MultiThread::signal.wait(lock);
        }
        auto processedGraphs = resultsFile.getProcessedGraphs();
        resultsFile.writeResults();
        resultsFile.close();
        std::cout << "Time measurements are finished. Results are written in " << _outputFileName << std::endl;
    }
};
