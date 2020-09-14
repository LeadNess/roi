#include <string>
#include <vector>
#include <map>
#include <set>

using std::string;
using std::vector;
using std::map;
using std::set;

struct Vertex {
    int _vertexCode;
    int _openTime;
    int _closeTime;
    int _component;
    Vertex* _parent;
    set<int> _adjList;
};

struct Graph {
    vector<vector<int>> _adjMatrix;
    map<int, Vertex*> _mapVertex;

    explicit Graph(const string& );
    ~Graph();
};