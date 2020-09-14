#include "graph.hpp"

Graph::Graph(const string& strGraph) {

    stringstream stream(strGraph);
    string buff;
    while (std::getline(stream, buff, ',')) {
        if (buff.size() == 1) {
            Vertex* vertex = new Vertex();
            vertex->_vertexCode = buff[0];
            vertex->_openTime = 0;
            vertex->_closeTime = 0;
            _mapVertex[buff[0]] = vertex;
        }
        else {
            char firstVertex = buff[0];
            char secondVertex = buff[1];

            _mapVertex[firstVertex]->_adjList.insert(secondVertex);
        }
    }

    vector<int> vecVert;
    for (auto& it : _mapVertex)
        vecVert.emplace_back(it.first);

    _adjMatrix = vector<vector<int>>(_mapVertex.size());
    for (int i = 0; i < _mapVertex.size(); i++) {
        _adjMatrix[i] = vector<int>(_mapVertex.size());
        for (int t = 0; t < _mapVertex.size(); t++) {
            if (_mapVertex[vecVert[i]]->_adjList.find(vecVert[t]) != _mapVertex[vecVert[i]]->_adjList.end())
                _adjMatrix[i][t] = 1;
            else
                _adjMatrix[i][t] = 0;
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