#include <iostream>
#include <string>
#include <chrono>
#include <args.hpp>
#include "extra_paths.hpp"

using namespace std::chrono;

struct AlgorithmArgs {
    std::string _inputFilesName;
    std::string _outputFileName;

    AlgorithmArgs() = default;

    static const char* help() {
        return "Algorithm removing unnecessary edges in graph";
    }

    template<class F>
    void parse(F f) {
        f(_inputFilesName, "--input", "-i", args::help("Input files names prefix"), args::required());
        f(_outputFileName, "--output", "-o", args::help("Output file name"));
    }

    void run() {
        std::cout << "Input files names: " << _inputFilesName << std::endl;
        std::cout << "Output file name: " << _outputFileName << std::endl;
        if (_outputFileName.empty()) {
            _outputFileName = "results.csv";
        }

        auto vecEdges = parseFileToEdgesVec(_inputFilesName);
        auto graph = Graph(vecEdges); // get Graph
        auto vecGraphs = getStronglyConnectedComponents(graph);

        std::ofstream fout(_outputFileName);
        fout << "Edges count;Nodes count;Time (ms)" << std::endl;

        auto vecUpdGraphs = vector<Graph>();
        auto start = steady_clock::now();
        for (Graph &g : vecGraphs) {
            vecUpdGraphs.emplace_back(RemoveExtraEdges(g));
        }
        double time = duration_cast<microseconds>(steady_clock::now() - start).count();
        fout << vecEdges.size() << ";" << graph.getNodesCount() << ";" << time << std::endl;
    }
};
