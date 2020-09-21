#include <map>
#include <utility>
#include "../generator/generator.hpp"

using std::map;
using std::make_pair;

enum COLOR { WHITE, GRAY, BLACK };

struct Node {
    int _nodeCode;
    int _openTime;
    int _closeTime;
    int _component;
    Node* _parent;
    COLOR _color;
    set<int> _adjList;

    Node(const int &nodeCode, set<int> adjList): _nodeCode(nodeCode), _adjList(std::move(adjList)), _parent(nullptr) {}
};

struct Graph {
    vector<vector<int>> _adjMatrix;
    map<int, Node*> _mapVertex;
    int _time;

    explicit Graph(const vector<Edge>& );
    explicit Graph(const vector<Node>& );
    ~Graph();

    void addNode(Node& );
};

vector<Edge> parseFileToEdgesVec(const string& );
