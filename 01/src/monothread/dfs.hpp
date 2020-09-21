#include "graph.hpp"

void DFS(Graph& );
void DFSVisit(Graph& , Node* );
void DFSVisit(Graph& , Node* , int );

vector<Graph> getStronglyConnectedComponents(Graph &g);
