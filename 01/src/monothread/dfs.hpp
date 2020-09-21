#include "graph.hpp"

void DFS(Graph& );
void DFSVisit(Graph& , Node* );
void DFSVisit(Graph& , Node* , map<int, vector<Node*>> & ,int );

vector<Graph> getStronglyConnectedComponents(Graph &g);
