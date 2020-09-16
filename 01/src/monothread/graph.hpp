#include <map>
#include "../generator/generator.hpp"

using std::map;

enum COLOR { WHITE, GRAY, BLACK };

struct Vertex {
    int _vertexCode;
    int _openTime;
    int _closeTime;
    int _component;
    Vertex* _parent;
    COLOR _color;
    set<int> _adjList;
};

struct Graph {
    vector<vector<int>> _adjMatrix;
    map<int, Vertex*> _mapVertex;
    int _time;

    explicit Graph(const vector<Edge>& );
    explicit Graph(const vector<Vertex>& );
    ~Graph();
};

vector<Edge> parseFileToEdgesVec(const string& );
