#include <map>
#include <utility>
#include "../generator/generator.hpp"

using std::map;
using std::make_pair;

enum COLOR { WHITE, GRAY, BLACK };

struct Node {
    int _nodeCode;
    int _openTime{};
    int _closeTime{};
    int _component{};
    int _shortestPath{};
    Node* _parent;
    COLOR _color;
    map<int, int> _mapEdges;

    Node(const int &nodeCode, map<int, int> mapEdges):
        _nodeCode(nodeCode), _mapEdges(std::move(mapEdges)), _parent(nullptr) {}
};

struct Graph {
    vector<vector<int>> _adjMatrix;
    map<int, Node*> _mapNodes;
    int _time{};

    explicit Graph(const vector<Edge>& );
    explicit Graph(const vector<Node*>& );
    explicit Graph(vector<vector<int>>  );
    ~Graph();

    vector<Edge> getEdgesVec();
};

vector<Edge> parseFileToEdgesVec(const string& );
