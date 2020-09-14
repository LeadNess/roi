struct Graph {
    vector<vector<int>> _adjMatr;
    map<char, Vertex*> _mapVertex;
    int _time;

    Graph(const string& );
    ~Graph();
};