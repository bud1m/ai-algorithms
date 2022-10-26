#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <queue>

#define VERY_BIG_NUM 10000
using namespace std;

struct Node
{
    char name;
    int h;
    int f;
    Node(char val, int h, int c) : name(val), h(h), f(c)
    {
    }
};

class Graph
{
public:
    map<char, vector<pair<Node, int>>> adj;

    void addEdge(Node &v, Node &w, int dist);
};

void Graph::addEdge(Node &v, Node &w, int dist)
{
    adj[v.name].push_back({w, dist});
    adj[w.name].push_back({v, dist});
}

bool sortbysec(const pair<Node, int> &a,
               const pair<Node, int> &b)
{
    return (a.first.f < b.first.f);
}

void aStar(Graph &g, Node &startNode, Node &endNode)
{

    vector<Node> nodes;
    nodes.push_back(startNode);

    while (!nodes.empty() || startNode.name == endNode.name)
    {
        nodes.erase(nodes.begin());

        for (size_t i = 0; i < g.adj[startNode.name].size(); i++)
        {
            Node newNode = g.adj[startNode.name][i].first;
            int dist = g.adj[startNode.name][i].second;

            // f(x) = distance + h(x)
            newNode.f = dist + newNode.h;

            nodes.push_back(newNode);
        }
        sort(nodes.begin(), nodes.end(), sortbysec);
    }
}

int main()
{
    Graph g;
    Node a('a', 14, 0);
    Node b('b', 12, VERY_BIG_NUM);
    Node c('c', 11, VERY_BIG_NUM);
    Node d('d', 6, VERY_BIG_NUM);
    Node e('e', 4, VERY_BIG_NUM);
    Node f('f', 11, VERY_BIG_NUM);
    Node z('z', 0, VERY_BIG_NUM);

    g.addEdge(a, b, 4);
    g.addEdge(a, c, 3);
    g.addEdge(b, e, 12);
    g.addEdge(b, f, 5);
    g.addEdge(c, e, 10);
    g.addEdge(c, d, 7);
    g.addEdge(d, e, 2);
    g.addEdge(e, z, 5);
    g.addEdge(z, f, 16);

    aStar(g, a, z);
    cout << "Following is Breadth First Traversal "
         << "(starting from vertex 2) \n";
}
