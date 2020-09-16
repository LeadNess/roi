#include <map>
#include "../generator/generator.hpp"

using std::map;

struct Vertex {
    int _vertexCode;
    int _openTime;
    int _closeTime;
    int _component;
    Vertex* _parent;
    set<int> _adjList;
};

struct Graph {
    vector<vector<int>> _adjMatrix;
    map<int, Vertex*> _mapVertex;

    explicit Graph(const vector<Edge>& );
    ~Graph();
};

vector<Edge> parseFileToGraph(const string& fileName) {
    auto vecEdges = vector<Edge>();
    std::ifstream inFile;
    try {
        inFile.open(fileName);
    } catch (std::exception& e) {
        std::cerr << "Error on opening file with graph: " << e.what() << std::endl;
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
