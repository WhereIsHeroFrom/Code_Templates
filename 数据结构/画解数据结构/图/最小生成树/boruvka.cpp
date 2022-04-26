#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

using namespace std;

#define maxn 200010
#define maxb 31
#define maxnodes (maxn*maxb)

#define UNDEF -1 
#define ROOT 0

struct TrieNode {
    int nodes[2];  // 字典树的两个子结点 
    int l, r;      // [l, r] 代表以当前结点为根的子树，管辖的 原数组 a[] 的区间范围 
}T[maxnodes];
int TrieNodes;

int a[200010];

void Init() {
    memset(T, UNDEF, sizeof(T));
    TrieNodes = 1;
}

int GetTrieNode() {
    return TrieNodes++;
}

void TrieInsert(int x, int idx) {
    int now = ROOT;
    for(int i = maxb-1; i >= 0; --i) {
        int bit = ((x>>i)&1);
        if(T[now].nodes[bit] == UNDEF) {
            T[now].nodes[bit] = GetTrieNode();
        }
        
        if( T[now].l == UNDEF ) {
            T[now].l = idx;
        }
        T[now].r = idx;
        
        now = T[now].nodes[bit];
    }
}

long long TrieQuery(int now, int depth, int x) {
    long long ret = 0;
    for(int i = depth; i >= 0; --i) {
        int bit = ((x>>i)&1);
        if(T[now].nodes[bit] != UNDEF) {
            now = T[now].nodes[bit];
        }else {
            now = T[now].nodes[bit^1];
            ret += (1<<i);
        }
    }
    return ret;
}

long long Boruvka(int now, int depth) {
    if(now == UNDEF) {
        return 0;
    }
    long long l = Boruvka(T[now].nodes[0], depth-1);
    long long r = Boruvka(T[now].nodes[1], depth-1);
    long long ans = l + r;
    if(T[now].nodes[0] != UNDEF && T[now].nodes[1] != UNDEF) {
        int x = T[now].nodes[0], y = T[now].nodes[1];    // (1)
        long long ret = 1e9; ret *= ret;
        for(int i = T[x].l; i <= T[x].r; ++i) {          // (2)
            ret = min(ret,  TrieQuery(y, depth-1, a[i]) + (1<<depth) );
        }
        ans += ret;              
    }
    return ans;
}

int n;
int main() {
    while(scanf("%d", &n) != EOF) {
        Init();
        for(int i = 0; i < n; ++i) {
            scanf("%d", &a[i]);
        }
        sort(a, a + n);
        for(int i = 0; i < n; ++i) {
            TrieInsert(a[i], i);
        }
        printf("%lld\n", Boruvka(ROOT, maxb-1));       
    }
    return 0;
} 
