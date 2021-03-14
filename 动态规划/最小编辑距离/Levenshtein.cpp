#include <iostream>
#include <algorithm>
#include <cstring>
#include <stack>
using namespace std;

typedef char ValueType;
const int maxn = 1010;
int f[maxn][maxn];
#define I 1
#define D 1
#define R 1

int min3(int a, int b, int c) {
    return min(min(a, b), c);
}

int minEditCost(int ssize, ValueType *src, int tsize, ValueType *tar) {
    f[0][0] = 0;
    // 空串到目标串的代价
    for (int j = 1; j <= tsize; ++j) {
        f[0][j] = f[0][j - 1] + I;
    }
    // 源串到空串的代价
    for (int i = 1; i <= ssize; ++i) {
        f[i][0] = f[i - 1][0] + D;
    }
    for (int i = 1; i <= ssize; ++i) {
        for (int j = 1; j <= tsize; ++j) {
            int rcost = (src[i] == tar[j]) ? 0 : R;       // 替换的消耗
            f[i][j] = min3(
                f[i - 1][j] + D,         // 源少1个，所以目标需要删除1个
                f[i][j - 1] + I,         // 目标少1个，所以源需要插入1个
                f[i - 1][j - 1] + rcost  // 源和目标 都 替换 最后一个字符
                );
        }
    }
    return f[ssize][tsize];
}