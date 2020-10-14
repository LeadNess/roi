#include <thread>
#include <chrono>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "../../01/src/monothread/extra_paths.hpp"

using namespace std::chrono;
using std::thread;
using std::queue;
using std::to_string;
using std::ofstream;

namespace MultiThread {

    class GraphProcessor {

        vector<Graph> _vecGraph;
        int _resultsCount;
        std::mutex _lock;

    public:

        explicit GraphProcessor(int );

        void addProcessedComponent(Graph&);
        vector<Graph> getProcessedGraph() const;

    };

} // MultiThread

MultiThread::GraphProcessor::GraphProcessor(int componentsCount) : _resultsCount(componentsCount) {
    _vecGraph = vector<Graph>();
}

void MultiThread::GraphProcessor::addProcessedComponent(Graph& graph) {
    _lock.lock();
    _vecGraph.emplace_back(graph);
    std::cout << _vecGraph.size() << "/" << _resultsCount
              << " Component: nodes(" << graph.getNodesCount() << "), edges("
              << graph.getEdgesVec().size() << ")" << std::endl;
    _lock.unlock();
    /*if (_resultsCount == _vecGraph.size()) {
        std::unique_lock<std::mutex> lock(mainThreadLock);
        flag = true;
        std::notify_all_at_thread_exit(signal, std::move(lock));
    }*/
}

vector<Graph> MultiThread::GraphProcessor::getProcessedGraph() const {
    return _vecGraph;
}

Graph RemoveExtraEdges(Graph &graph) {
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
