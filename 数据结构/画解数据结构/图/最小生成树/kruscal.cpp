#define maxn 1010

int pre[maxn];
void unionfind_init(int n) {             // (1)
    for(int i = 0; i < n; ++i) {
        pre[i] = i;
    }
}

int unionfind_find(int x) {              // (2)
    return pre[x] == x ? (x) : (pre[x] = unionfind_find(pre[x]));
}

bool unionfind_union(int x, int y) {     // (3)
    int px = unionfind_find(x);
    int py = unionfind_find(y);
    if(px == py) {
        return false;
    }
    pre[px] = py;
    return true;
}


struct KEdge {                            // (4)
    int u, v, w;
}E[maxn * maxn];

int cmp(const void* a, const void* b) {   // (5)
    struct KEdge *pa = (struct KEdge *)a;
    struct KEdge *pb = (struct KEdge *)b;
    return pa->w - pb->w;
}

// 点的个数 n，边的个数 m
int Kruscal(int n, int m, struct KEdge* edges) {
    int i, ret = 0;
    int edgeCnt = 0;
    qsort(edges, m, sizeof(struct KEdge), cmp);  // (6)
    unionfind_init(n);                           // (7)
    for(i = 0; i < m; ++i) {
        if( unionfind_union( edges[i].u, edges[i].v ) ) {
            ret += edges[i].w;                   // (8)
            if(++edgeCnt == n-1) {               // (9)
                return ret;
            }
        }
    }
    return 0;
}
