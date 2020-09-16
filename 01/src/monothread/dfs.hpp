#include "graph.hpp"

void DFS(Graph& );
void DFSVisit(Graph& , Vertex* );
void DFSVisit(Graph& , Vertex* , int );

vector<Graph> getStronglyConnectedComponents(Graph &g);
