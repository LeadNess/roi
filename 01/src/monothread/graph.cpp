#include "graph.hpp"

Graph::Graph(const vector<Edge>& vecEdges) {
    auto mapNodes = map<int, int>(); // map<nodeCode, nodeIndexInAdjMatrix>
    auto mapNode2Edges = map<int, vector<pair<int, int>>>(); // map<node, vector<pair<node, edgeWeight>>
    int counter = 0;
    for (auto& edge : vecEdges) {
        if (mapNodes.find(edge._secondNode) == mapNodes.end()) {
            mapNodes[counter++] = edge._secondNode;
        }
        if (mapNodes.find(edge._firstNode) == mapNodes.end()) {
            mapNodes[counter++] = edge._firstNode;
        } else {
            int firstNode = mapNodes[edge._firstNode];
            int secondNode = mapNodes[edge._secondNode];
            mapNode2Edges[firstNode].emplace_back(make_pair(secondNode, edge._weight));
        }
    }
    mapNodes.clear();

    vector<int> vecVert;
    for (auto& mapIt : mapNode2Edges) {
        vecVert.emplace_back(mapIt.first);
        auto adjList = set<int>();
        for (auto& vecIt : mapIt.second) {
            adjList.insert(vecIt.first);
        }
        _mapVertex[mapIt.first] = new Node(mapIt.first, adjList);
    }

    _adjMatrix = vector<vector<int>>(_mapVertex.size());
    for (int i = 0; i < _mapVertex.size(); i++) {
        _adjMatrix[i] = vector<int>(_mapVertex.size());
        for (int t = 0; t < _mapVertex.size(); t++) {
            if (_mapVertex[vecVert[i]]->_adjList.find(vecVert[t]) != _mapVertex[vecVert[i]]->_adjList.end()) {
                _adjMatrix[i][t] = 1;
            } else {
                _adjMatrix[i][t] = 0;
            }
        }
    }

    for (auto& vertex: _mapVertex) {
        vertex.second->_component = -1;
    }
}

Graph::~Graph() {
    for (auto& it : _mapVertex) {
        delete _mapVertex[it.first];
    }
}

Graph::Graph(const vector<Node>& vecVertex) {
    _adjMatrix = vector<vector<int>>(vecVertex.size());
    for (auto& vertex : vecVertex) {
        auto
    }
}

void Graph::addNode(Node &) {
    _mapVertex[vertex._vertexCode] = &vertex;
    _adjMatrix
}

vector<Edge> parseFileToEdgesVec(const string& fileName) {
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
