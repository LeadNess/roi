#include <random>
#include <stdexcept>
#include <string>

struct Edge {
    int _firstNode;
    int _secondNode;
    int _weight;

    Edge(int firstNode, int secondNode, int weight):
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

    int _nodesCount;
    int _edgesCount;

public:

    explicit Generator(int nodesCount, int edgesCount) {
        if (nodesCount < 1 || edgesCount < 0) {
            throw InvalidGeneratorData();
        }
        _nodesCount = nodesCount;
        _edgesCount = edgesCount;
    }

    void generateData(std::string outputFileName) {
        for (int i = 0; i < _nodesCount; i++) {

        }
    }

};