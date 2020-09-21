#include <queue>
#include "bellman_ford.hpp"

using std::priority_queue;

void Dijkstra(Graph &graph, Node *sNode) {
    initializeSingleSource(graph, sNode);
    auto mapNodes = map<int, Node *>();
    auto queueNodes = priority_queue<Node *>();

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
}
