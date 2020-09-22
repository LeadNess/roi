#include <queue>
#include "bellman_ford.hpp"

using std::priority_queue;

void Dijkstra(Graph &graph, Node *sNode, map<pair<int,int>,int>& mapEdges) {
    initializeSingleSource(graph, sNode);
    auto mapNodes = map<int, Node *>();
    auto queueNodes = priority_queue<Node*>();

    for (auto &mapIt : graph._mapNodes) {
        queueNodes.push(mapIt.second);
    }

    while (!queueNodes.empty()) {
        Node *uNode = queueNodes.top();
        queueNodes.pop();
        mapNodes.insert(make_pair(uNode->_nodeCode, uNode));
        for (auto& mapIt : uNode->_mapEdges) {
            relax(uNode, graph._mapNodes[mapIt.first], mapIt.second);
        }
    }

    for (auto& mapIt : graph._mapNodes) {
        Node *node = mapIt.second;
        while(node != nullptr) {
            if (node->_parent == nullptr) {
                if (graph._mapNodes.size() == 1) {
                    mapEdges[make_pair(mapIt.first, mapIt.first)] = 0;
                }
                break;
            }
            mapEdges[make_pair(node->_parent->_nodeCode, node->_nodeCode)] = node->_parent->_mapEdges[node->_nodeCode];
            node = node->_parent;
        }
    }
}
