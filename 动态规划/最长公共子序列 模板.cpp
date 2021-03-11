#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

typedef char ValueType;
const int maxn = 2010;
int f[maxn][maxn];

int getLCS(int hsize, ValueType *h, int vsize, ValueType *v) {
    memset(f, 0, sizeof(f));
    f[0][0] = 0;
    for (int i = 1; i <= vsize; ++i) {
        for (int j = 1; j <= hsize; ++j) {
            if (v[i] == h[j]) {
                f[i][j] = f[i - 1][j - 1] + 1;
            }
            else {
                f[i][j] = max(f[i - 1][j], f[i][j - 1]);
            }
        }
    }
    return f[vsize][hsize];
}

char s[2][maxn];

int main() {
    int len[2];
    while (scanf("%s %s", &s[0][1], &s[1][1]) != EOF) {
        len[0] = strlen(&s[0][1]);
        len[1] = strlen(&s[1][1]);
        printf("%d\n", getLCS(len[0], s[0], len[1], s[1]));

    }
    return 0;
}
