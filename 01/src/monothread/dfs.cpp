#include "dfs.hpp"

void DFSVisit(Graph &graph, Node *u) {
    graph._time++;

    u->_openTime = graph._time;
    u->_color = GRAY;

    for (auto& vertex : u->_adjList) {
        if (graph._mapVertex[vertex]->_color == WHITE) {
            graph._mapVertex[vertex]->_parent = u;
            DFSVisit(graph, graph._mapVertex[vertex]);
        }
    }

    u->_color = BLACK;
    graph._time++;
    u->_closeTime = graph._time;
}

void DFSVisit(Graph &graph, Node *u, map<int, Graph> &mapComponents, int componentCount) {
    graph._time++;

    u->_openTime = graph._time;
    u->_color = GRAY;
    u->_component = componentCount;
    mapComponents[componentCount].addNode(u);

    for (auto& vertex : u->_adjList) {
        if (graph._mapVertex[vertex]->_color == WHITE) {
            graph._mapVertex[vertex]->_parent = u;
            DFSVisit(graph, graph._mapVertex[vertex], componentCount);
        }
    }

    u->_color = BLACK;
    graph._time++;
    u->_closeTime = graph._time;
}

void DFS(Graph &graph) {

    for (auto& vertex : graph._mapVertex) {
        vertex.second->_color = WHITE;
        vertex.second->_parent = nullptr;
    }

    graph._time = 0;

    for (auto& vertex : graph._mapVertex)
        if (vertex.second->_color == WHITE)
            DFSVisit(graph, vertex.second);
}

vector<Graph> getStronglyConnectedComponents(Graph& g) {
    DFS(g);

    vector<vector<int>> transformedAdjMatrix(g._adjMatrix.size());
    for (int i = 0; i < g._adjMatrix.size(); i++) {
        transformedAdjMatrix[i] = vector<int>(g._adjMatrix.size());
    }

    for (int i = 0; i < g._adjMatrix.size(); i++) {
        for (int t = 0; t < g._adjMatrix.size(); t++) {
            transformedAdjMatrix[i][t] = g._adjMatrix[t][i];
        }
    }

    auto gTrans = Graph(vector<Edge>());
    gTrans._adjMatrix = transformedAdjMatrix;
    map<int,Node*> mapVer;
    for (auto& it : g._mapVertex)
        mapVer[it.second->_closeTime] = it.second;


    for (auto& vertex : g._mapVertex) {
        vertex.second->_color = WHITE;
        vertex.second->_parent = nullptr;
    }

    g._time = 0;
    int componentCount = (g._adjMatrix.size()) ? 1 : 0;
    map<int, Graph> mapComponents;

    for (auto it = mapVer.rbegin(); it != mapVer.rend(); it++)
        if (g._mapVertex[it->second->_nodeCode]->_color == WHITE) {
            DFSVisit(g, g._mapVertex[it->second->_nodeCode], mapComponents, componentCount);
            componentCount++;
        }
    return componentCount;
}