#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "../../01/src/monothread/extra_paths.hpp"

using namespace std::chrono;
using std::thread;
using std::to_string;
using std::ofstream;

namespace MultiThread {

    std::mutex mainThreadLock;
    std::condition_variable mainThreadSignal;
    bool algorithmIsDone;

    class GraphProcessor {

        vector<Graph> _vecGraph;
        int _resultsCount;
        std::mutex _lock;

    public:

        explicit GraphProcessor(int );

        void addProcessedComponent(Graph& ,int ,int );
        vector<Graph> getProcessedGraph() const;

    };

} // MultiThread

MultiThread::GraphProcessor::GraphProcessor(int componentsCount) : _resultsCount(0) {
    _vecGraph = vector<Graph>(componentsCount);
}

void MultiThread::GraphProcessor::addProcessedComponent(Graph& graph, int componentIndex, int stopNumber) {
    _lock.lock();
    _vecGraph[componentIndex] = graph;
    _resultsCount++;
    if (_resultsCount == stopNumber) {
        std::unique_lock<std::mutex> lock(mainThreadLock);
        algorithmIsDone = true;
        std::notify_all_at_thread_exit(mainThreadSignal, std::move(lock));
    }
    _lock.unlock();
}

vector<Graph> MultiThread::GraphProcessor::getProcessedGraph() const {
    return _vecGraph;
}


