#include <iostream>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

//***************************************** 二维树状数组 模板 *****************************************
#define maxn 1001
#define ll long long

ll c[maxn][maxn];

int lowbit(int x) {
    return x & (-x);
}

void clear() {
    memset(c, 0, sizeof(c));
}

void add(int x, int y, int v, int xmax, int ymax) {
    while (x <= xmax) {
        int ty = y;
        while (ty <= ymax) {
            c[x][ty] += v;
            ty += lowbit(ty);
        }
        x += lowbit(x);
    }
}

int sum(int x, int y) {
    ll s = 0;
    while (x >= 1) {
        int ty = y;
        while (ty >= 1) {
            s += c[x][ty];
            ty -= lowbit(ty);
        }
        x -= lowbit(x);
    }
    return s;
}
//***************************************** 二维树状数组 模板 *****************************************

