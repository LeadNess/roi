#include "bellman_ford.hpp"

void initializeSingleSource(Graph &graph, Node *s) {
    for (auto& mapIt : graph._mapNodes) {
        graph._mapNodes[mapIt.first]->_shortestPath = -1;
        graph._mapNodes[mapIt.first]->_parent = nullptr;
    }
    s->_shortestPath = 0;
}

void relax(Node* uNode, Node* vNode, int weight) {
    if (vNode->_shortestPath > uNode->_shortestPath + weight) {
        vNode->_shortestPath = uNode->_shortestPath + weight;
        uNode->_parent = uNode;
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
    graph.updateAdjacencyMatrix();
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
