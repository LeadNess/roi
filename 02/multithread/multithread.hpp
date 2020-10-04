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
    std::condition_variable signal;
    bool algorithmIsDone;

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

    ResultsFile::ResultsFile(const string& fileName, int resultsCount) : _resultsCount(0) {
        algorithmIsDone = false;
        _vecResults = vector<string>(resultsCount);
        _vecGraphs = vector<vector<Graph>>(resultsCount);
        _file = ofstream(fileName);
        if (!_file.is_open()) throw FileIsNotOpened();
    }

    void ResultsFile::addResult(const string& result, const vector<Graph>& graph, int index) {
        _fileMutex.lock();
        _vecResults[index] = result;
        _vecGraphs[index] = graph;
        _resultsCount++;
        if (_resultsCount == _vecResults.size()) {
            std::unique_lock<std::mutex> lock(mainThreadLock);
            algorithmIsDone = true;
            std::notify_all_at_thread_exit(signal, std::move(lock));
        }
        _fileMutex.unlock();
    }

    void ResultsFile::writeResults() {
        _fileMutex.lock();
        _file << "Edges count;Nodes count;Time (ms)" << std::endl;
        for (auto& result: _vecResults) {
            _file << result << std::endl;
        }
        _fileMutex.unlock();
    }

    vector<vector<Graph>> ResultsFile::getProcessedGraphs() const {
        return _vecGraphs;
    }

    void ResultsFile::close() {
        _fileMutex.lock();
        _file.close();
        _fileMutex.unlock();
    }

} // MultiThread

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
    double time = duration_cast<milliseconds>(steady_clock::now() - start).count();
    std::cout << "Finished algorithm for graph with " << vecEdges.size() << " edges and "
              << graph.getNodesCount() << " nodes in " << time << " millisecond" << std::endl;

    string result = to_string(vecEdges.size()) + ";" + to_string(graph.getNodesCount()) + ";" + to_string(time);
    resultsFile.addResult(result, vecUpdGraphs, index);
}