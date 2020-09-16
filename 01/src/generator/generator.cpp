#include "generator.hpp"

Generator::Generator(const int& edgesCount, const int& maxNodesCount, const int& minEdgeWeight, const int& maxEdgeWeight) {
    if (maxNodesCount < 1 || edgesCount < 0 || minEdgeWeight > maxEdgeWeight) {
        throw InvalidGeneratorData();
    }
    _maxNodesCount = maxNodesCount;
    _edgesCount = edgesCount;
    _minEdgeWeight = minEdgeWeight;
    _maxEdgeWeight = maxEdgeWeight;
    _edgesVec = vector<Edge>();
}

void Generator::generateData(const string& outputFileNamePrefix) {
    std::random_device rd;
    std::mt19937 range(rd());
    std::uniform_int_distribution<int> getNode(1, _maxNodesCount);
    std::uniform_int_distribution<int> getEdgeWeight(_minEdgeWeight, _maxEdgeWeight);

    auto existEdgesSet = set<pair<int, int>>();

    for (int i = 0; i < _edgesCount; i++) {
        int firstNode = getNode(range);
        int secondNode = getNode(range);
        while (secondNode == firstNode) {
            secondNode = getNode(range);
        }

        while (existEdgesSet.find(pair<int, int>(firstNode, secondNode)) != existEdgesSet.end()) {
            firstNode = getNode(range);
            secondNode = getNode(range);
            while (secondNode == firstNode) {
                secondNode = getNode(range);
            }
        }
        existEdgesSet.insert(pair<int, int>(firstNode, secondNode));
        _edgesVec.emplace_back(Edge(firstNode, secondNode, getEdgeWeight(range)));
    }
    string fileName = outputFileNamePrefix + std::to_string(_edgesVec.size()) + ".txt";
    try {
        std::ofstream outFile(fileName);
        for (auto &edge : _edgesVec) {
            outFile << edge._firstNode << " "
                    << edge._secondNode << " "
                    << edge._weight << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Error on writing generator data" << e.what() << std::endl;
    }
}

std::vector<Config> parseConfig(const string& cfgFileName) {
    auto vecCfg = vector<Config>();
    std::ifstream inFile;
    try {
        inFile.open(cfgFileName);
    } catch (std::exception& e) {
        std::cerr << "Error on opening config file: " << e.what() << std::endl;
        return vecCfg;
    }
    string buf;
    while (std::getline(inFile, buf, '\n')) {
        stringstream bufStream(buf);

        std::getline(bufStream, buf, ' ');
        int edgesCount = stoi(buf);

        std::getline(bufStream, buf, ' ');
        int maxNodesCount = stoi(buf);

        std::getline(bufStream, buf, ' ');
        int minEdgeWeight = stoi(buf);

        std::getline(bufStream, buf, ' ');
        int maxEdgeWeight = stoi(buf);

        vecCfg.emplace_back(Config(edgesCount, maxNodesCount, minEdgeWeight, maxEdgeWeight));
    }
    return vecCfg;
}