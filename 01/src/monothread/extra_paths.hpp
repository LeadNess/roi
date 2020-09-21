#include "dijkstra.hpp"

Graph RemoveExtraEdges(Graph &graph) {
    /*if (!BellmanFord(graph, graph._mapNodes[0])) {
        std::cout << "The input graph contains a cycle with a negative weight" << std::endl;
        return Graph(vector<vector<int>>());
    }*/
    auto vecEdges = graph.getEdgesVec();
    for (auto& mapIt : graph._mapNodes) {
        Dijkstra(graph, mapIt.second);
    }

    auto setEdges = set<pair<int, int>>(); // set<pair<firstNode, secondNode>>
    auto adjMatrix = vector<vector<int>>(graph._mapNodes.size());
    for (auto & vecRow : adjMatrix) {
        vecRow = vector<int>(graph._mapNodes.size());
        for (int & cell : vecRow) {
            cell = 0;
        }
    }
    // TODO: debug it
    for (auto& mapIt : graph._mapNodes) {
        Node *node = mapIt.second;
        while(node != nullptr) {
            if (node->_parent == nullptr) {
                break;
            }
            adjMatrix[mapIt.first][node->_parent->_nodeCode] = node->_mapEdges[node->_parent->_nodeCode];
            setEdges.insert(make_pair(node->_nodeCode, node->_parent->_nodeCode));
            node = node->_parent;
        }
    }
    return Graph(adjMatrix);
}