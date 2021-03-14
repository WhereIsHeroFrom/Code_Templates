#include <iostream>
#include <algorithm>
#include <cstring>
#include <stack>
using namespace std;

typedef char ValueType;
const int maxn = 1010;
int f[maxn][maxn];
int p[maxn][maxn];
#define I 1
#define D 1
#define R 1

int min3(int a, int b, int c) {
    return min(min(a, b), c);
}

int pack(int x, int y) {
    return x * maxn + y;
}

enum EditType {
    ET_INSERT = 1,
    ET_DELETE = 2,
    ET_REPLACE = 3,
};

struct Edit {
    EditType et;
    int pos;
    ValueType val;
    Edit() {}
    Edit(EditType a, int b, ValueType c) : et(a), pos(b), val(c) {}
    void print() {
        if (et == ET_INSERT) {
            printf("Insert %d,%c\n", pos, val);
        }
        else if (et == ET_DELETE) {
            printf("Delete %d\n", pos);
        }
        else if (et == ET_REPLACE) {
            printf("Replace %d,%c\n", pos, val);
        }
    }
};

void findPath(int ssize, ValueType *src, int tsize, ValueType *tar, stack<Edit>& st) {
    while (!st.empty())
        st.pop();
    int sidx = ssize, tidx = tsize;
    while (sidx || tidx) {
        int spre = p[sidx][tidx] / maxn;
        int tpre = p[sidx][tidx] % maxn;

        if (sidx - spre == 1 && tidx - tpre == 1) {
            if (src[sidx] != tar[tidx]) {
                st.push(Edit(ET_REPLACE, tidx, tar[tidx]));
            }
        }
        else if (sidx - spre == 1) {
            st.push(Edit(ET_DELETE, tidx + 1, ' '));
        }
        else if (tidx - tpre == 1) {
            st.push(Edit(ET_INSERT, tidx, tar[tidx]));
        }
        sidx = spre;
        tidx = tpre;
    }
}

int minEditCost(int ssize, ValueType *src, int tsize, ValueType *tar, stack<Edit>& st) {
    f[0][0] = 0;
    // 空串到目标串的代价
    for (int j = 1; j <= tsize; ++j) {
        f[0][j] = f[0][j - 1] + I;
        p[0][j] = pack(0, j - 1);
    }
    // 源串到空串的代价
    for (int i = 1; i <= ssize; ++i) {
        f[i][0] = f[i - 1][0] + D;
        p[i][0] = pack(i - 1, 0);
    }

    for (int i = 1; i <= ssize; ++i) {
        for (int j = 1; j <= tsize; ++j) {
            int rcost = (src[i] == tar[j]) ? 0 : R;       // 替换的消耗
            f[i][j] = min3(
                f[i - 1][j] + D,         // 源少1个，所以目标需要删除1个
                f[i][j - 1] + I,         // 目标少1个，所以源需要插入1个
                f[i - 1][j - 1] + rcost  // 源和目标 都 替换 最后一个字符
                );
            if (f[i][j] == f[i - 1][j] + D) p[i][j] = pack(i - 1, j);
            if (f[i][j] == f[i][j - 1] + I) p[i][j] = pack(i, j - 1);
            if (f[i][j] == f[i - 1][j - 1] + rcost) p[i][j] = pack(i - 1, j - 1);
        }
    }
    findPath(ssize, src, tsize, tar, st);
    return f[ssize][tsize];
}