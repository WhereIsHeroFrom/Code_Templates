#include <iostream>
#include <algorithm>
#include <cstring>
#include <stack>
using namespace std;

typedef int ValueType;
const int maxn = 1010;
int f[2][maxn];
int p[maxn][maxn];

int pack(int x, int y) {
    return x * maxn + y;
}

int getLCSLength(int hsize, ValueType *h, int vsize, ValueType *v, stack<int>& path) {

    memset(f, 0, sizeof(f));
    while (!path.empty())
        path.pop();

    int cur = 1, last = 0;
    for (int i = 1; i <= vsize; ++i) {
        int opt = 0, pos = 0;
        for (int j = 1; j <= hsize; ++j) {
            if (v[i] > h[j]) {
                opt = max(opt, f[last][j]);
                if (opt == f[last][j]) {
                    pos = j;
                }
            }

            if (v[i] == h[j]) {
                f[cur][j] = opt + 1;
                p[i][j] = pack(i - 1, pos);
            }
            else {
                f[cur][j] = f[last][j];
                p[i][j] = pack(i - 1, j);
            }
        }
        swap(last, cur);
    }
    int vidx = vsize, hidx = hsize;
    int Max = -1;
    for (int i = 1; i <= hsize; ++i) {
        if (f[last][i] > Max) {
            Max = f[last][i];
            hidx = i;
        }
    }


    while (vidx && hidx) {
        int pre = p[vidx][hidx];
        int previdx = pre / maxn;
        int prehidx = pre % maxn;
        if (vidx - previdx && prehidx - hidx) {
            path.push(vidx * maxn + hidx);
        }
        vidx = previdx;
        hidx = prehidx;
    }
    return f[last][hsize];
}

ValueType s[2][maxn];

int main() {
    int len[2], t;
    scanf("%d", &t);
    while (t--){
        scanf("%d", &len[0]);
        for (int i = 1; i <= len[0]; ++i) {
            scanf("%d", &s[0][i]);
        }

        scanf("%d", &len[1]);
        for (int i = 1; i <= len[1]; ++i) {
            scanf("%d", &s[1][i]);
        }
        stack<int> path;
        getLCSLength(len[0], s[0], len[1], s[1], path);

        printf("%d\n", path.size());
        if (t) puts("");
    }
    return 0;
}
