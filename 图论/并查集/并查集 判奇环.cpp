const int MAXN = 1003010;
int fset[MAXN], rk[MAXN];
int n;

void init_set(int n) {
    for (int i = 1; i < MAXN; ++i) {
        fset[i] = i;
        rk[i] = 0;
    }
}

int find_set(int x) {
    return (fset[x] == x) ? x : find_set(fset[x]);
}

int find_depth(int x) {
    return (fset[x] == x) ? 0 : 1 + find_depth(fset[x]);
}

int add_set(int x) {
    fset[x] = ++n;
    rk[fset[x]] = rk[x] + 1;
    return fset[x];
}

bool union_set(int x, int y) {
    int fx = find_set(x), fy = find_set(y);
    int dx = find_depth(x), dy = find_depth(y);
    bool bfit = (dx - dy) % 2;

    if (fx == fy) {
        // 根节点一样，则需要判断奇偶性，深度相减为偶数，说明出现了奇环
        return bfit;
    }
    if (bfit) {
        // 奇偶性 不一致的情况：深度小的那个需要增加一个 伪根 
        if (rk[fx] < rk[fy]) {
            fx = add_set(fx);
        }
        else {
            fy = add_set(fy);
        }
    }

    // 奇偶性 一致的情况：直接连接根
    if (rk[fx] == rk[fy]) {
        ++rk[fx];
        fset[fy] = fx;
    }
    else if (rk[fx] < rk[fy]) {
        fset[fx] = fy;
    }
    else {
        fset[fy] = fx;
    }

    return true;
}
