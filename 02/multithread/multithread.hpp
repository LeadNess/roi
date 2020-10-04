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

    vector<std::unique_ptr<std::mutex>> vecLocks;
    vector<std::condition_variable> vecSignals;
    vector<bool> vecFlags;

    class FileIsNotOpened : public std::exception {
    public:
        virtual const char* what() noexcept {
            return "file is not opened";
        }
    };

    class ResultsFile {

        ofstream _file;
        vector<string> _vecResults;
        vector<vector<Graph>> _vecGraphs;
        int _resultsCount;
        std::mutex _fileMutex;

    public:

        ResultsFile(const string& , int );

        void addResult(const string& , const vector<Graph>& , int );
        void writeResults();
        void close();
        vector<vector<Graph>> getProcessedGraphs() const;

    };

} // MultiThread

void initLocks(int );

void processGraph(const string& , MultiThread::ResultsFile& , int );


MultiThread::ResultsFile::ResultsFile(const string& fileName, int resultsCount) : _resultsCount(0) {
    algorithmIsDone = false;
    _vecResults = vector<string>(resultsCount);
    _vecGraphs = vector<vector<Graph>>(resultsCount);
    _file = ofstream(fileName);
    if (!_file.is_open()) throw FileIsNotOpened();
}

void MultiThread::ResultsFile::addResult(const string& result, const vector<Graph>& graph, int index) {
    _fileMutex.lock();
    _vecResults[index] = result;
    _vecGraphs[index] = graph;
    _resultsCount++;
    if (_resultsCount == _vecResults.size()) {
        std::unique_lock<std::mutex> lock(mainThreadLock);
        algorithmIsDone = true;
        std::notify_all_at_thread_exit(mainThreadSignal, std::move(lock));
    }
    _fileMutex.unlock();
}

void MultiThread::ResultsFile::writeResults() {
    _fileMutex.lock();
    _file << "Edges count;Nodes count;Time (ms)" << std::endl;
    for (auto& result: _vecResults) {
        _file << result << std::endl;
    }
    _fileMutex.unlock();
}

vector<vector<Graph>> MultiThread::ResultsFile::getProcessedGraphs() const {
    return _vecGraphs;
}

void MultiThread::ResultsFile::close() {
    _fileMutex.lock();
    _file.close();
    _fileMutex.unlock();
}


void initLocks(int tasksCount) {
    MultiThread::vecLocks = vector<std::unique_ptr<std::mutex>>(tasksCount);
    MultiThread::vecSignals = vector<std::condition_variable>(tasksCount);
    MultiThread::vecFlags = vector<bool>(tasksCount);
    for (int i = 0; i < tasksCount; i++) {
        MultiThread::vecLocks[i] = std::unique_ptr<std::mutex>();
        MultiThread::vecFlags[i] = false;
    }
}

void processGraph(const string& inputFileName, MultiThread::ResultsFile& resultsFile, int index) {
    auto vecEdges = parseFileToEdgesVec(inputFileName);
    auto graph = Graph(vecEdges); // get Graph

    std::cout << "Get data from " << inputFileName << " file. Start algorithm for graph with " << vecEdges.size() << " edges and "
              << graph.getNodesCount() << " nodes..." << std::endl;

    auto vecGraphs = getStronglyConnectedComponents(graph);
    auto vecUpdGraphs = vector<Graph>();
    auto start = steady_clock::now();
    for (Graph &g : vecGraphs) {
        vecUpdGraphs.emplace_back(RemoveExtraEdges(g));
    }
    auto time = duration_cast<milliseconds>(steady_clock::now() - start).count();
    std::cout << "Finished algorithm for graph with " << vecEdges.size() << " edges and "
              << graph.getNodesCount() << " nodes in " << time << " millisecond" << std::endl;

    string result = to_string(vecEdges.size()) + ";" + to_string(graph.getNodesCount()) + ";" + to_string(time);
    resultsFile.addResult(result, vecUpdGraphs, index);
}