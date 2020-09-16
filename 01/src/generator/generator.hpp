#include <random>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;
using std::set;
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
    vector<Edge> _edgesVec;

public:

    explicit Generator(const int& , const int& , const int& , const int& );
    void generateData(const string& );

};

struct Config {
    int edgesCount;
    int maxNodesCount;
    int minEdgeWeight;
    int maxEdgeWeight;

    Config(const int& edgesCount, const int& maxNodesCount, const int& minEdgeWeight, const int& maxEdgeWeight):
        edgesCount(edgesCount), maxNodesCount(maxNodesCount), minEdgeWeight(minEdgeWeight), maxEdgeWeight(maxEdgeWeight) {}
};

vector<Config> parseConfig(const string& );
