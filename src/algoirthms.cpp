#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <queue>
using namespace std;

class Graph
{
public:
    map<int, bool> visited;
    map<int, vector<int>> adj;

    void addEdge(int v, int w);

    // Uninformed
    void DFS(int v);
    void BFS(int v);

    // Informed
    void aStar(int v);
};

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
}
void Graph::DFS(int v)
{
    visited[v] = true;

    for (size_t i = 0; i < adj[v].size(); i++)
        if (!visited[adj[v][i]])
            DFS(adj[v][i]);
}

void Graph::BFS(int v)
{
    queue<int> q;

    q.push(v);

    while (!q.empty())
    {
        cout << q.front();
        v = q.front();
        visited[v] = true;

        for (size_t i = 0; i < adj[v].size(); i++)
        {
            if (!visited[adj[v][i]])
                q.push(adj[v][i]);
        }
        q.pop();
    }
}


int main()
{
    Graph g;
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);

    cout << "Following is Breadth First Traversal "
         << "(starting from vertex 2) \n";
    g.BFS(2);
}
