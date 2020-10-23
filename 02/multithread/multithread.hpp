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

        map<pair<int,int>,int> _mapEdges;
        std::mutex _lock;

    public:

        explicit GraphProcessor();

        void addEdgesMap(map<pair<int,int>,int>& );
        map<pair<int,int>,int> getProcessedEdgesMap() const;

    };

} // MultiThread

MultiThread::GraphProcessor::GraphProcessor() {
    _mapEdges = map<pair<int,int>,int>();
}

void MultiThread::GraphProcessor::addEdgesMap(map<pair<int,int>,int>& mapEdges) {
    _lock.lock();
    for (auto& it : mapEdges) {
        _mapEdges[it.first] = it.second;
    }
    _lock.unlock();
}

map<pair<int,int>,int> MultiThread::GraphProcessor::getProcessedEdgesMap() const {
    return _mapEdges;
}

Graph ParallelRemoveExtraEdges(Graph &graph) {
    auto availableThreadsCount = std::thread::hardware_concurrency();
    int delta = graph.getNodesCount() < availableThreadsCount ? 1 : round(float(graph.getNodesCount()) / availableThreadsCount);
    auto vecThreads = vector<std::thread>();
    MultiThread::GraphProcessor processor;

    auto threadsCount = graph.getNodesCount() < availableThreadsCount ? graph.getNodesCount() : availableThreadsCount;
    for (int i = 0; i < threadsCount; i++) {
        int beginIndex = i  * delta;
        int endIndex = (i == threadsCount - 1) ? graph.getNodesCount() - 1 : (i + 1) * delta;
        vecThreads.emplace_back(std::thread(
                [](MultiThread::GraphProcessor &processor, Graph& graph, int beginIndex, int endIndex) {
                    auto copiedGraph = Graph(graph.getEdgesVec());
                    auto mapEdges = map<pair<int,int>,int>();

                    auto mapIt = copiedGraph._mapNodes.begin();
                    for (int i = 0; i < beginIndex; i++, mapIt++);
                    for (int i = 0; i < endIndex; i++, mapIt++) {
                        if (mapIt == copiedGraph._mapNodes.end()) {
                            break;
                        }
                        ShortestParts(copiedGraph, mapIt->second, mapEdges);
                    }
                    processor.addEdgesMap(std::ref(mapEdges));
                },
                std::ref(processor), std::ref(graph), beginIndex, endIndex));
    }
    std::for_each(vecThreads.begin(), vecThreads.end(), [&](std::thread& th) {
        th.join();
    });
    auto mapEdges = processor.getProcessedEdgesMap();
    auto vecEdgesUpd = vector<Edge>();
    for (auto& mapIt : mapEdges) {
        vecEdgesUpd.emplace_back(Edge(mapIt.first.first, mapIt.first.second, mapIt.second));
    }
    return Graph(vecEdgesUpd);
}
