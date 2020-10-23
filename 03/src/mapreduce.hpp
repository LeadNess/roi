#include <qt5/QtConcurrent/QtConcurrent>

#include <functional>
#include <ctime>
#include <iostream>
#include <fstream>

#include "../../01/src/monothread/bellman_ford.hpp"

map<pair<int, int>, int> MapFunction(const pair<Graph, map<int, Node*>> & InterData) {
	map<pair<int, int>, int> mapEdges;
	auto copiedGraph = Graph(InterData.first.getEdgesVec());
	auto mapIt = InterData.first._mapNodes.begin();
	for (auto& it : InterData.second) {
		ShortestParts(copiedGraph, mapIt->second, mapEdges);
	}
	return mapEdges;
}

void ReduceFunction(map<pair<int, int>, int>& Results, const map<pair<int, int>, int>& InterData) {
	for (auto& it : InterData)
		Results[it.first] = it.second;
}

Graph MapReduceRemoveExtraEdges(Graph& g) {
	auto copiedGraph = Graph(g.getEdgesVec());

	auto Ithread = QThread::idealThreadCount();
    vector<pair<Graph, map<int, Node*>>> VData;

	int first = 0;
	int delta = copiedGraph.getNodesCount() < Ithread ? 1 : round(float(copiedGraph.getNodesCount()) / Ithread); // copiedGraph.getNodesCount() / Ithread;
	int last = delta;
	for (int i = 0; i < Ithread; i++) {
		auto item = std::make_pair(copiedGraph, map<int, Node*>());
		auto mapIt = copiedGraph._mapNodes.begin();
		for (int i = 0; i < first; i++, mapIt++);
		for (int i = 0; i < last; i++, mapIt++) {
			if (mapIt == copiedGraph._mapNodes.end()) {
				break;
			}
			item.second[mapIt->first] = mapIt->second;
		}

		VData.emplace_back(item);
		first = last;
		last = last + delta < copiedGraph.getNodesCount() ? last + delta : copiedGraph.getNodesCount();
	}

	auto mapEdges = QtConcurrent::blockingMappedReduced(VData, MapFunction, ReduceFunction);

	auto vecEdgesUpd = vector<Edge>();
	for (auto& mapIt : mapEdges) {
		vecEdgesUpd.emplace_back(Edge(mapIt.first.first, mapIt.first.second, mapIt.second));
	}
	return Graph(vecEdgesUpd);
}
