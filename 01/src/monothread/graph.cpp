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

    vector<int> vecNodes;
    for (auto& mapIt : mapNode2Edges) {
        vecNodes.emplace_back(mapIt.first);
        auto mapEdges = map<int,int>();
        for (auto& vecIt : mapIt.second) {
            mapEdges.insert(make_pair(vecIt.first, vecIt.second));
        }
        _mapNodes[mapIt.first] = new Node(mapIt.first, mapEdges);
    }

    _adjMatrix = vector<vector<int>>(_mapNodes.size());
    for (int i = 0; i < _mapNodes.size(); i++) {
        _adjMatrix[i] = vector<int>(_mapNodes.size());
        for (int t = 0; t < _mapNodes.size(); t++) {
            if (_mapNodes[vecNodes[i]]->_mapEdges.find(vecNodes[t]) != _mapNodes[vecNodes[i]]->_mapEdges.end()) {
                _adjMatrix[i][t] = _mapNodes[vecNodes[i]]->_mapEdges[vecNodes[t]];
            } else {
                _adjMatrix[i][t] = 0;
            }
        }
    }

    for (auto& vertex: _mapNodes) {
        vertex.second->_component = -1;
    }
}

Graph::Graph(const vector<Node*>& vecNodes) {
    vector<int> vecVert;
    for (Node *node : vecNodes) {
        vecVert.emplace_back(node->_nodeCode);
        _mapNodes[node->_nodeCode] = node;
    }

    _adjMatrix = vector<vector<int>>(_mapNodes.size());
    for (int i = 0; i < _mapNodes.size(); i++) {
        _adjMatrix[i] = vector<int>(_mapNodes.size());
        for (int t = 0; t < _mapNodes.size(); t++) {
            if (_mapNodes[vecVert[i]]->_mapEdges.find(vecVert[t]) != _mapNodes[vecVert[i]]->_mapEdges.end()) {
                _adjMatrix[i][t] = _mapNodes[vecVert[i]]->_mapEdges[vecVert[t]];
            } else {
                _adjMatrix[i][t] = 0;
            }
        }
    }

    for (auto& vertex: _mapNodes) {
        vertex.second->_component = -1;
    }
}

Graph::~Graph() {
    for (auto& it : _mapNodes) {
        delete _mapNodes[it.first];
    }
}

void Graph::updateAdjacencyMatrix() {
    vector<int> vecVert;
    for (auto& it : _mapNodes) {
        vecVert.emplace_back(it.second->_nodeCode);
    }

    _adjMatrix = vector<vector<int>>(_mapNodes.size());
    for (int i = 0; i < _mapNodes.size(); i++) {
        _adjMatrix[i] = vector<int>(_mapNodes.size());
        for (int t = 0; t < _mapNodes.size(); t++) {
            if (_mapNodes[vecVert[i]]->_mapEdges.find(vecVert[t]) != _mapNodes[vecVert[i]]->_mapEdges.end()) {
                _adjMatrix[i][t] = _mapNodes[vecVert[i]]->_mapEdges[vecVert[t]];
            } else {
                _adjMatrix[i][t] = 0;
            }
        }
    }
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
