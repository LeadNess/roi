#include <iostream>
#include <mutex>
#include <fstream>

#include <args.hpp>
#include "mapreduce.hpp"

using namespace std::chrono;
using std::string;
using std::ofstream;

struct MapReduceAlgorithmArgs {
    string _inputFileNamePrefix;
    string _outputFileName;
    string _cfgFileName;

	MapReduceAlgorithmArgs() = default;

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

        std::ofstream fout(_outputFileName);
        fout << "Edges count;Nodes count;Time (ms)" << std::endl;
        auto vecCfg = parseConfig(_cfgFileName);

        for (auto& cfg : vecCfg) {
            string inputFileName = _inputFileNamePrefix + std::to_string(cfg.edgesCount) + ".txt";

            auto vecEdges = parseFileToEdgesVec(inputFileName);
            auto graph = Graph(vecEdges); // get Graph
            auto vecGraphs = getStronglyConnectedComponents(graph);
            auto vecUpdGraphs = vector<Graph>();

            std::cout << "Start algorithm for graph with " << vecEdges.size() << " edges and "
                      << graph.getNodesCount() << " nodes..." << std::endl;
            auto start = steady_clock::now();
            for (Graph &g : vecGraphs) {
                vecUpdGraphs.emplace_back(MapReduceRemoveExtraEdges(g));
            }
            double time = duration_cast<milliseconds>(steady_clock::now() - start).count();
            std::cout << "Finished in " << time << " millisecond" << std::endl;

            fout << vecEdges.size() << ";" << graph.getNodesCount() << ";" << time << std::endl;

            std::ofstream oldGraphFileOut(_inputFileNamePrefix + std::to_string(cfg.edgesCount) + "_Old.csv");
            std::ofstream updGraphFileOut(_inputFileNamePrefix + std::to_string(cfg.edgesCount) + "_Upd.csv");
            for (Graph &g : vecUpdGraphs) {
                for (auto& edge : g.getEdgesVec()) {
                    oldGraphFileOut << edge._firstNode << ";" << edge._secondNode << ";" << edge._weight << std::endl;
                    updGraphFileOut << edge._firstNode << ";" << edge._secondNode << ";" << edge._weight << std::endl;
                }
            }
            oldGraphFileOut.close();
            updGraphFileOut.close();
        }
        std::cout << "Time measurements are finished. Results are written in " << _outputFileName << std::endl;
        fout.close();
    }
};
