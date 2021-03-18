#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;

typedef int ValueType;
const int maxn = 1010;
const int maxm = 2000010;
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

// 顶点 0 ~ n-1
// 起点 start
// 终点 end
ValueType Dijkstra(int n, int start, int end) {
    for (int i = 0; i < n; ++i) {
        dist[i] = inf;
    }
    memset(visited, false, sizeof(visited));
    dist[start] = 0;
    while (1) {
        ValueType Min = inf;
        int u;
        for (int i = 0; i < n; ++i) {
            if (visited[i]) {
                continue;
            }
            if (dist[i] < Min) {
                Min = dist[i];
                u = i;
            }
        }
        if (Min == inf) {
            break;
        }
        visited[u] = true;

        for (int e = head[u]; ~e; e = edges[e].next) {
            Edge& to = edges[e];
            if (dist[u] + to.w < dist[to.v]) {
                dist[to.v] = dist[u] + to.w;
            }
        }
    }
    return dist[end];
}
