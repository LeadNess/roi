#include <random>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <sstream>

using std::string;
using std::stringstream;
using std::pair;

struct Edge {
    int _firstNode;
    int _secondNode;
    int _weight;

    Edge(const int& firstNode, const int& secondNode, const int& weight):
        _firstNode(firstNode), _secondNode(secondNode), _weight(weight) {}
};

class InvalidGeneratorData : public std::exception {
public:
    virtual const char* what() noexcept {
        return "invalid nodesCount/edgesCount values";
    }
};

class Generator {

private:

    int _edgesCount;
    int _maxNodesCount;
    int _minEdgeWeight;
    int _maxEdgeWeight;
    std::vector<Edge> _edgesVec;

public:

    explicit Generator(const int& edgesCount, const int& maxNodesCount, const int& minEdgeWeight, const int& maxEdgeWeight) {
        if (maxNodesCount < 1 || edgesCount < 0 || minEdgeWeight > maxEdgeWeight) {
            throw InvalidGeneratorData();
        }
        _maxNodesCount = maxNodesCount;
        _edgesCount = edgesCount;
        _minEdgeWeight = minEdgeWeight;
        _maxEdgeWeight = maxEdgeWeight;
        _edgesVec = std::vector<Edge>();
    }

    void generateData(string outputFileNamePrefix) {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> getNode(1, _maxNodesCount);
        std::uniform_int_distribution<int> getEdgeWeight(_minEdgeWeight, _maxEdgeWeight);

        auto existEdgesSet = std::set<pair<int, int>>();

        for (int i = 0; i < _edgesCount; i++) {
            int firstNode = getNode(rng);
            int secondNode = getNode(rng);
            while (secondNode == firstNode) {
                secondNode = getNode(rng);
            }

            while (existEdgesSet.find(pair<int, int>(firstNode, secondNode)) != existEdgesSet.end()) {
                firstNode = getNode(rng);
                secondNode = getNode(rng);
                while (secondNode == firstNode) {
                    secondNode = getNode(rng);
                }
            }
            existEdgesSet.insert(pair<int, int>(firstNode, secondNode));
            _edgesVec.emplace_back(Edge(firstNode, secondNode, getEdgeWeight(rng)));
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
};

struct Config {
    int edgesCount;
    int maxNodesCount;
    int minEdgeWeight;
    int maxEdgeWeight;

    Config(const int& edgesCount, const int& maxNodesCount, const int& minEdgeWeight, const int& maxEdgeWeight):
        edgesCount(edgesCount), maxNodesCount(maxNodesCount), minEdgeWeight(minEdgeWeight), maxEdgeWeight(minEdgeWeight) {}
};


std::vector<Config> parseConfig(const string& cfgFileName) {
    auto vecCfg = std::vector<Config>();
    std::ifstream inFile(cfgFileName);
    std::string buf;
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