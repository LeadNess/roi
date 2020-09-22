#include <iostream>
#include <string>
#include <args.hpp>
#include "extra_paths.hpp"

struct AlgorithmArgs {
    std::string _inputFilesName;
    std::string _outputFileName;

    AlgorithmArgs() = default;

    static const char* help() {
        return "Algorithm removing unnecessary edges in graph";
    }

    template<class F>
    void parse(F f) {
        f(_inputFilesName, "--input", "-i", args::help("Input files names prefix")/*, args::required()*/);
        f(_outputFileName, "--output", "-o", args::help("Output file name"));
    }

    void run() {
        std::cout << "Input files names: " << _inputFilesName << std::endl;
        std::cout << "Output file name: " << _outputFileName << std::endl;

        _inputFilesName = "/home/leadness/Study/roi/01/src/test10.txt";
        auto vecEdges = parseFileToEdgesVec(_inputFilesName);
        auto graph = Graph(vecEdges); // get Graph
        auto vecGraphs = getStronglyConnectedComponents(graph);
        auto vecUpdGraphs = vector<Graph>();
        for (Graph &g : vecGraphs) {
            vecUpdGraphs.emplace_back(RemoveExtraEdges(g));
        }
        std::cout << "End!" << std::endl;
    }
};
