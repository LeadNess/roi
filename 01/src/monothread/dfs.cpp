#include "dfs.hpp"

void DFSVisit(Graph &graph, Node *u) {
    graph._time++;

    u->_openTime = graph._time;
    u->_color = GRAY;

    for (auto& mapIt : u->_mapEdges) {
        if (graph._mapNodes[mapIt.first]->_color == WHITE) {
            graph._mapNodes[mapIt.first]->_parent = u;
            DFSVisit(graph, graph._mapNodes[mapIt.first]);
        }
    }

    u->_color = BLACK;
    graph._time++;
    u->_closeTime = graph._time;
}

void DFSVisit(Graph &graph, Node *u, map<int, vector<Node*>> &mapComponents, int componentCount) {
    graph._time++;

    u->_openTime = graph._time;
    u->_color = GRAY;
    u->_component = componentCount;
    mapComponents[componentCount].emplace_back(u);

    for (auto& mapIt : u->_mapEdges) {
        if (graph._mapNodes[mapIt.first]->_color == WHITE) {
            graph._mapNodes[mapIt.first]->_parent = u;
            DFSVisit(graph, graph._mapNodes[mapIt.first], componentCount);
        }
    }

    u->_color = BLACK;
    graph._time++;
    u->_closeTime = graph._time;
}

void DFS(Graph &graph) {

    for (auto& vertex : graph._mapNodes) {
        vertex.second->_color = WHITE;
        vertex.second->_parent = nullptr;
    }

    graph._time = 0;

    for (auto& vertex : graph._mapNodes)
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
    for (auto& it : g._mapNodes)
        mapVer[it.second->_closeTime] = it.second;


    for (auto& vertex : g._mapNodes) {
        vertex.second->_color = WHITE;
        vertex.second->_parent = nullptr;
    }

    g._time = 0;
    int componentCount = 0;
    auto mapComponents = map<int, vector<Node*>>();

    for (auto it = mapVer.rbegin(); it != mapVer.rend(); it++)
        if (g._mapNodes[it->second->_nodeCode]->_color == WHITE) {
            DFSVisit(g, g._mapNodes[it->second->_nodeCode], mapComponents, componentCount);
            componentCount++;
        }

    auto vecGraph = vector<Graph>();
    for (auto& mapIt : mapComponents) {
        vecGraph.emplace_back(Graph(mapIt.second));
    }

    return vecGraph;
}