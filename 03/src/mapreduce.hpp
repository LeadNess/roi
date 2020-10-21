#include <qt5/QtConcurrent/QtConcurrent>

#include <functional>
#include <ctime>
#include <iostream>
#include <fstream>

#include "../../01/src/monothread/bellman_ford.hpp"

map<pair<int, int>, int> MapFunction(const pair<Graph, map<int, Node*>> & InterData) {
	map<pair<int, int>, int> mapEdges;
	auto copiedGraph = Graph(InterData.first._adjMatrix);

	auto mapIt = InterData.first._mapNodes.begin();
	for (auto& it : InterData.second) {
		ShortestParts(copiedGraph, mapIt->second, mapEdges);
	}
	for (auto& mapIt : copiedGraph._mapNodes) {
		Node *node = mapIt.second;
		while (node != nullptr) {
			if (node->_parent == nullptr || node->_color == BLACK) {
				if (copiedGraph._mapNodes.size() == 1) {
					mapEdges[make_pair(mapIt.first, mapIt.first)] = 0;
				}
				break;
			}
			mapEdges[make_pair(node->_parent->_nodeCode, node->_nodeCode)] = node->_parent->_mapEdges[node->_nodeCode];
			node->_color = BLACK;
			node = node->_parent;
		}
	}
	return mapEdges;
}

void ReduceFunction(map<pair<int, int>, int>& Results, const map<pair<int, int>, int>& InterData) {
	for (auto& it : InterData)
		Results[it.first] = it.second;
}

Graph MapReduceRemoveExtraEdges(Graph& g) {
	auto copiedGraph = Graph(g._adjMatrix);

	auto qVecEdges = vector<Edge>();
	vector<pair<Graph, map<int, Node*>>> VData(0);

	auto Ithread = QThread::idealThreadCount();
	int first = 0;
	int delta = qVecEdges.size() / Ithread;
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

		VData.push_back(item);
		first = last;
		last = last + delta < qVecEdges.size() ? last + delta : qVecEdges.size();
	}

	auto mapEdges = QtConcurrent::blockingMappedReduced(VData, MapFunction, ReduceFunction);

	auto vecEdgesUpd = vector<Edge>();
	for (auto& mapIt : mapEdges) {
		vecEdgesUpd.emplace_back(Edge(mapIt.first.first, mapIt.first.second, mapIt.second));
	}
	return Graph(vecEdgesUpd);
}
