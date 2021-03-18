#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

typedef int ValueType;
const int maxn = 100010;
const int maxm = 200010;
const ValueType inf = 1e9;

struct Edge {
    int u, v, next;
    ValueType w;
    Edge(){}
    Edge(int _u, int _v, ValueType _w, int _next) :
        u(_u), v(_v), w(_w), next(_next)
    {
    }
}edges[maxm];

struct Dist {
    int u;
    ValueType w;
    Dist(){}
    Dist(int _u, ValueType _w) : u(_u), w(_w)
    {
    }
    bool operator < (const Dist& d) const {
        return w > d.w;
    }
};

int head[maxn], edgeCount;
ValueType dist[maxn];
bool visited[maxn];

void init() {
    edgeCount = 0;
    memset(head, -1, sizeof(head));
}

void addEdge(int u, int v, ValueType w) {
    edges[edgeCount] = Edge(u, v, w, head[u]);
    head[u] = edgeCount++;
}

void DijkstraHeap(int n, int st, ValueType *dist) {
    for (int i = 0; i < n; ++i) {
        dist[i] = (st == i) ? 0 : inf;
        visited[i] = false;
    }
    priority_queue <Dist> heap;
    heap.push(Dist(st, 0));

    while (!heap.empty()) {
        Dist s = heap.top();
        heap.pop();
        int u = s.u;
        ValueType dis = s.w;
        if (visited[u]) continue;           // 访问过的不用重复更新
        visited[u] = true;

        // 利用 Dijkstra 更新其它点的最短路信息
        for (int e = head[u]; ~e; e = edges[e].next) {
            int v = edges[e].v;
            dist[v] = min(dist[v], dis + edges[e].w);
            if (dis + edges[e].w == dist[v])
                heap.push(Dist(v, dist[v]));
        }
    }
}

