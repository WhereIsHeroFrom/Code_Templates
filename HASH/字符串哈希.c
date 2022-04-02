#define ull unsigned long long
#define maxn 100010
#define P 10207
ull h[maxn], p[maxn];

void initHash(const char *s) {
    // 这个字符串是从 1 - n-1
    int n = strlen(s);
    int i;
    p[0] = 1;
    h[0] = 0;
    for(i = 1; i < n; ++i) {
        h[i] = h[i-1] * P + s[i];
        p[i] = p[i-1] * P;
    }
}

ull getSubHash(int l, int r) {
    return h[r] - h[l-1] * p[r - l + 1];
}
