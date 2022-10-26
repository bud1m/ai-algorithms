#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>

using namespace std;

class Graph
{
public:
    map<int, bool> visited;
    map<int, vector<int>> adj;

    void addEdge(int v, int w);

    void DFS(int v);
};

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
}

void Graph::DFS(int v)
{
    visited[v] = true;
    cout << v << " ";

    for (int i = 0; i < adj[v].size(); i++)
    {
        if (!visited[adj[v][i]])
        {
            DFS(adj[v][i]);
        }
    }
}
int main()
{
    // Create a graph given in the above diagram
    Graph g;
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);
 
    cout << "Following is Depth First Traversal"
            " (starting from vertex 2) \n";
 
    // Function call
    g.DFS(2);
 
    return 0;
}