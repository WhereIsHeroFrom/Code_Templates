#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;


typedef int ValueType;
const int maxn = 105;
const ValueType inf = 1e9;
ValueType mat[maxn][maxn];

void init(int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat[i][j] = (i == j) ? 0 : inf;
        }
    }
}

void addEdge(int u, int v, int w) {
    mat[u][v] = min(mat[u][v], w);
}

void FloydWarshall(int n) {
    int i, j, k;
    for (k = 0; k < n; ++k)
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j)
                mat[i][j] = min(mat[i][j], mat[i][k] + mat[k][j]);
}