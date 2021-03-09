#include <iostream>
#include <algorithm>

using namespace std;

struct Pair {
    int w, s;
    int idx;
    bool read(int i) {
        idx = i;
        return scanf("%d %d", &w, &s) != EOF;
    }
    bool operator < (const Pair&  p) const {
        if (s != p.s)
            return s > p.s;
        return w < p.w;
    }

};

/*******************************************最长递增子序列 模板*******************************************/
typedef Pair ValueType;
const int maxn = 100010;
const int inf = -1;
// 在 g 数组中找的时候
// 如果是严格单调，则用 <= 
// 如果是非递增或非递减，则用 < 
ValueType g[maxn];
int idx[maxn];
int pre[maxn];
int f[maxn];
int gsize;
int path[maxn], pasize;

enum LISType {
    LIST_STRICTLY = 0,            // 严格单调
    LIST_NOTSTRICTLY = 1,         // 非严格单调
};

bool cmpLIS(LISType lt, ValueType a, ValueType b) {
    if (LIST_STRICTLY == lt) {
        return a.w <= b.w;
    }
    else if (LIST_NOTSTRICTLY == lt) {
        return a < b;
    }
}

int findLISIndex(LISType lt, ValueType val, ValueType *g, int& gsize) {
    int l = 1, r = gsize, ans = -1;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (cmpLIS(lt, val, g[mid])) {
            ans = mid;
            r = mid - 1;
        }
        else {
            l = mid + 1;
        }
    }
    return ans;
}

void findPath(int asize) {
    int maxIdx = inf;
    for (int i = asize; i >= 1; --i) {
        if (maxIdx == -1 || f[i] > f[maxIdx]) {
            maxIdx = i;
        }
    }
    pasize = 0;
    while (maxIdx != inf) {
        path[pasize++] = maxIdx;
        maxIdx = pre[maxIdx];
    }

    for (int i = 0; i < pasize / 2; ++i) {
        swap(path[i], path[pasize - 1 - i]);
    }
}

void findLIS(LISType lt, ValueType *a, int asize) {
    gsize = 0;
    idx[0] = inf;
    for (int i = 1; i <= asize; ++i) {
        int ans = findLISIndex(lt, a[i], g, gsize);
        if (ans == -1) {
            ans = ++gsize;
        }
        g[ans] = a[i];            // g[ans]: 长度为ans的最长递增子序列最后一个值的最小值
        idx[ans] = i;             // idx[ans]: 匹配 g 数组的下标
        pre[i] = idx[ans - 1];    // pre[i]: 第i个数作为递增子序列的最后一个数，前驱是 pre[i]
        f[i] = ans;               // f[i]:   以a[i]结尾的最长递增子序列长度
    }
    findPath(asize);
}


/*******************************************最长递增子序列 模板*******************************************/