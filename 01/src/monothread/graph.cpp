#include "graph.hpp"

#include <utility>

Graph::Graph(const vector<Edge>& vecEdges) {
    auto mapNodes = map<int, int>(); // map<nodeCode, node>
    auto mapNode2Edges = map<int, vector<pair<int, int>>>(); // map<node, vector<pair<node, edgeWeight>>
    int counter = 0;
    for (auto& edge : vecEdges) {
        if (mapNodes.find(edge._secondNode) == mapNodes.end()) {
            mapNodes[edge._secondNode] = counter;
            counter++;
        }
        if (mapNodes.find(edge._firstNode) == mapNodes.end()) {
            mapNodes[edge._firstNode] = counter;
            counter++;
        }
        int firstNode = mapNodes[edge._firstNode];
        int secondNode = mapNodes[edge._secondNode];
        mapNode2Edges[firstNode].emplace_back(make_pair(secondNode, edge._weight));
    }

    vector<int> vecNodes;
    for (auto& mapIt : mapNodes) {
        vecNodes.emplace_back(mapIt.second);
        auto mapEdges = map<int,int>();
        if (mapNode2Edges.find(mapIt.second) != mapNode2Edges.end()) {
            for (auto& vecIt : mapNode2Edges[mapIt.second]) {
                mapEdges.insert(make_pair(vecIt.first, vecIt.second));
            }
        }
        _mapNodes[mapIt.second] = new Node(mapIt.second, mapEdges);
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
    auto mapNodeCode2Index = map<int,int>();
    int counter = 0;
    for (Node *node : vecNodes) {
        mapNodeCode2Index[node->_nodeCode] = counter;
        counter++;
    }

    auto vecVert = vector<int>();
    for (Node *node : vecNodes) {
        vecVert.emplace_back(mapNodeCode2Index[node->_nodeCode]);
        auto mapEdges = map<int,int>();
        for (auto& mapIt : node->_mapEdges) {
            mapEdges.insert(make_pair(mapNodeCode2Index[mapIt.first], mapIt.second));
        }
        _mapNodes[mapNodeCode2Index[node->_nodeCode]] = new Node(mapNodeCode2Index[node->_nodeCode], mapEdges);
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

    for (auto& node: _mapNodes) {
        node.second->_component = -1;
    }
}

Graph::Graph(vector<vector<int>> adjMatrix) {
    auto vecEdges = vector<Edge>();
    for (int i = 0; i < adjMatrix.size(); i++) {
        for (int t = 0; t < adjMatrix[i].size(); t++) {
            if (adjMatrix[i][t]) {
                vecEdges.emplace_back(Edge(i, t, adjMatrix[i][t]));
            }
        }
    }

    auto mapNodes = map<int, int>(); // map<nodeCode, node>
    auto mapNode2Edges = map<int, vector<pair<int, int>>>(); // map<node, vector<pair<node, edgeWeight>>
    int counter = 0;
    for (auto& edge : vecEdges) {
        if (mapNodes.find(edge._secondNode) == mapNodes.end()) {
            mapNodes[edge._secondNode] = counter;
            counter++;
        }
        if (mapNodes.find(edge._firstNode) == mapNodes.end()) {
            mapNodes[edge._firstNode] = counter;
            counter++;
        } else {
            int firstNode = mapNodes[edge._firstNode];
            int secondNode = mapNodes[edge._secondNode];
            mapNode2Edges[firstNode].emplace_back(make_pair(secondNode, edge._weight));
        }
    }

    vector<int> vecNodes;
    for (auto& mapIt : mapNodes) {
        vecNodes.emplace_back(mapIt.second);
        auto mapEdges = map<int,int>();
        if (mapNode2Edges.find(mapIt.second) != mapNode2Edges.end()) {
            for (auto& vecIt : mapNode2Edges[mapIt.second]) {
                mapEdges.insert(make_pair(vecIt.first, vecIt.second));
            }
        }
        _mapNodes[mapIt.second] = new Node(mapIt.second, mapEdges);
    }

    _adjMatrix = adjMatrix;
}

Graph::Graph(): _adjMatrix(vector<vector<int>>(0)), _mapNodes(map<int, Node*>()) {}

Graph::~Graph() {
    /*for (auto& it : _mapNodes) {
        delete _mapNodes[it.first];
    }*/
}

vector<Edge> Graph::getEdgesVec() const {
    auto vecEdges = vector<Edge>();
    for (auto& mapIt : _mapNodes) {
        for (auto& mapEdge : mapIt.second->_mapEdges) {
            vecEdges.emplace_back(Edge(mapIt.first, mapEdge.first, mapEdge.second));
        }
    }
    return vecEdges;
}

size_t Graph::getNodesCount() const {
    return _mapNodes.size();
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
