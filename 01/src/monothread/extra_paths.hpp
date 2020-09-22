#include "dijkstra.hpp"

Graph RemoveExtraEdges(Graph &graph) {
    /*if (!BellmanFord(graph, graph._mapNodes[0])) {
        std::cout << "The input graph contains a cycle with a negative weight" << std::endl;
        return Graph(vector<vector<int>>());
    }*/
    auto mapEdges = map<pair<int,int>,int>();
    auto vecEdges = graph.getEdgesVec();
    for (auto& mapIt : graph._mapNodes) {
        ShortestParts(graph, mapIt.second, mapEdges);
    }

    auto vecEdgesUpd = vector<Edge>();
    for (auto& mapIt : mapEdges) {
        vecEdgesUpd.emplace_back(Edge(mapIt.first.first, mapIt.first.second, mapIt.second));
    }
    return Graph(vecEdgesUpd);
}