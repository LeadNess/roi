#include <limits>
#include "dfs.hpp"

void initializeSingleSource(Graph &graph, Node *s) {
    for (auto& mapIt : graph._mapNodes) {
        graph._mapNodes[mapIt.first]->_shortestPath = std::numeric_limits<int>::max();
        graph._mapNodes[mapIt.first]->_parent = nullptr;
        graph._mapNodes[mapIt.first]->_color = WHITE;
    }
    s->_shortestPath = 0;
}

void relax(Node* uNode, Node* vNode, int weight) {
    if (vNode->_shortestPath > uNode->_shortestPath + weight) {
        vNode->_shortestPath = uNode->_shortestPath + weight;
        vNode->_parent = uNode;
    }
}

bool BellmanFord(Graph &graph, Node *sNode) {
    initializeSingleSource(graph, sNode);
    for (int i = 0; i < graph._adjMatrix.size(); i++) {
        for (int t = 0; t < graph._adjMatrix[i].size(); t++) {
            if (i != t) {
                relax(graph._mapNodes[i], graph._mapNodes[t], graph._adjMatrix[i][t]);
            }
        }
    }
    for (int i = 0; i < graph._adjMatrix.size(); i++) {
        for (int t = 0; t < graph._adjMatrix[i].size(); t++) {
            if (i != t) {
                if (graph._mapNodes[i]->_shortestPath > graph._mapNodes[t]->_shortestPath + graph._adjMatrix[i][t]) {
                    return false;
                }
            }
        }
    }
    return true;
}

void ShortestParts(Graph &graph, Node *sNode, map<pair<int,int>,int>& mapEdges) {
    initializeSingleSource(graph, sNode);
    for (int count = 0; count < graph._mapNodes.size() - 1; count++) {
        for (int i = 0; i < graph._adjMatrix.size(); i++) {
            for (int t = 0; t < graph._adjMatrix[i].size(); t++) {
                if (graph._adjMatrix[i][t]) {
                    relax(graph._mapNodes[i], graph._mapNodes[t], graph._adjMatrix[i][t]);
                }
            }
        }
    }

    for (auto& mapIt : graph._mapNodes) {
        Node *node = mapIt.second;
        while(node != nullptr) {
            if (node->_parent == nullptr || node->_color == BLACK) {
                if (graph._mapNodes.size() == 1) {
                    mapEdges[make_pair(mapIt.first, mapIt.first)] = 0;
                }
                break;
            }
            mapEdges[make_pair(node->_parent->_nodeCode, node->_nodeCode)] = node->_parent->_mapEdges[node->_nodeCode];
            node->_color = BLACK;
            node = node->_parent;
        }
    }
}
