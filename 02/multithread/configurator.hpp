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
        f(_inputFileNamePrefix, "--input", "-i", args::help("Input files names prefix")/*, args::required()*/);
        f(_outputFileName, "--output", "-o", args::help("Output file name")/*, args::required()*/);
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

        for (auto & cgf : vecCfg) {
            string inputFileName = _inputFileNamePrefix + std::to_string(cgf.edgesCount) + ".txt";

            auto vecEdges = parseFileToEdgesVec(inputFileName);
            auto graph = Graph(vecEdges); // get Graph
            auto vecGraphs = getStronglyConnectedComponents(graph);

            auto availableThreadsCount = std::thread::hardware_concurrency();
            int delta = vecGraphs.size() < availableThreadsCount ? 1 : round(float(vecGraphs.size()) / availableThreadsCount);
            auto vecThreads = vector<std::thread>();
            MultiThread::GraphProcessor processor(vecGraphs.size());

            std::cout << "Start algorithm for graph with " << vecEdges.size() << " edges and "
                      << graph.getNodesCount() << " nodes..." << std::endl;
            std::cout << "Components count: " << vecGraphs.size() << std::endl;
            auto start = steady_clock::now();

            auto threadsCount = vecGraphs.size() < availableThreadsCount ? vecGraphs.size() : availableThreadsCount;
            for (int i = 0; i < threadsCount; i++) {
                int beginIndex = i  * delta;
                int endIndex = (i == threadsCount - 1) ? vecGraphs.size() : (i + 1) * delta;
                vecThreads.emplace_back(std::thread(
                        [](MultiThread::GraphProcessor &processor, vector<Graph>& vecGraphs, int beginIndex, int endIndex) {
                            for (int i = beginIndex; i < (endIndex < vecGraphs.size() ? endIndex : vecGraphs.size()); i++) {
                                auto graph = RemoveExtraEdges(vecGraphs[i]);
                                processor.addProcessedComponent(graph);
                            }
                        },
                        std::ref(processor), std::ref(vecGraphs), beginIndex, endIndex));
            }
            std::for_each(vecThreads.begin(), vecThreads.end(), [&](std::thread& th) {
                th.join();
            });
            std::cout << "Complete for " << inputFileName << std::endl;
            /*std::unique_lock<std::mutex> lock(MultiThread::mainThreadLock);
            while(!MultiThread::flag) {
                MultiThread::signal.wait(lock);
            }
            MultiThread::flag = false;*/
            auto vecUpdGraphs = processor.getProcessedGraph();
            double time = duration_cast<milliseconds>(steady_clock::now() - start).count();
            std::cout << "Finished in " << time << " millisecond" << std::endl;

            fout << vecEdges.size() << ";" << graph.getNodesCount() << ";" << time << std::endl;
        }
        std::cout << "Time measurements are finished. Results are written in " << _outputFileName << std::endl;
        fout.close();
    }
};
