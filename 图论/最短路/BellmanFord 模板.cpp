#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;

typedef int ValueType;
const int maxn = 100010;
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

void BellmanFordInit(int n) {
    for (int i = 0; i < n; ++i) {
        dist[i] = 0;
    }
}

bool BellmanFordUpdate() {
    bool flag = false;
    for (int i = 0; i < edgeCount; ++i) {
        Edge &edge = edges[i];
        if (dist[edge.u] + edge.w < dist[edge.v]) {
            flag = true;
            dist[edge.v] = dist[edge.u] + edge.w;
        }
    }
    return flag;
}

// 判断是否存在负环
bool BellmanFord(int n) {
    BellmanFordInit(n);
    for (int i = 0; i < n - 1; ++i) {
        if (!BellmanFordUpdate()) {
            return false;
        }
    }
    return BellmanFordUpdate();
}
