#include <iostream>
#include <string>
#include <args.hpp>
#include "graph.hpp"

vector<Edge> parseEdgesFile(const string &edgesFileName) {
    auto vecEdges = vector<Edge>();
    std::ifstream inFile;
    try {
        inFile.open(edgesFileName);
    } catch (std::exception& e) {
        std::cerr << "Error on opening file with edges: " << e.what() << std::endl;
        return vecEdges;
    }
    string buf;
    while (std::getline(inFile, buf, '\n')) {
        stringstream bufStream(buf);

        std::getline(bufStream, buf, ' ');
        int firstNode = stoi(buf);

        std::getline(bufStream, buf, ' ');
        int secondNode = stoi(buf);

        std::getline(bufStream, buf, ' ');
        int weight = stoi(buf);

        vecEdges.emplace_back(Edge(firstNode, secondNode, weight));
    }
    return vecEdges;
}

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

        auto vecEdges = parseEdgesFile(_inputFilesName);
        auto graph = Graph(vecEdges);

    }
};
