const int MAXN = 300010;
int fset[MAXN];

void init_set(int n) {
    for (int i = 1; i <= n; ++i) {
        fset[i] = i;
    }
}

int find_set(int x) {
    return (fset[x] == x) ? x : (fset[x] = find_set(fset[x]));
}

bool union_set(int x, int y) {
    int fx = find_set(x);
    int fy = find_set(y);
    if (fx != fy) {
        fset[fx] = fy;
        return true;
    }
    return false;
}
