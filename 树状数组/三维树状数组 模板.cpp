#include <iostream>
#include <cstring>
using namespace std;

#define MAXN 110

int c[MAXN][MAXN][MAXN];
int n;

int lowbit(int x) {
	return x & -x;
}

void add(int x, int y, int z, int v) {
	v &= 1;
	if (v < 0) {
		v = -v;
	}

	int i, j, k;
	for (i = x; i <= n; i += lowbit(i)) {
		for (j = y; j <= n; j += lowbit(j)) {
			for (k = z; k <= n; k += lowbit(k)) {
				c[i][j][k] ^= v;
			}
		}
	}
}

int sum(int x, int y, int z) {
	int i, j, k;
	int s = 0;
	for (i = x; i; i -= lowbit(i)) {
		for (j = y; j; j -= lowbit(j)) {
			for (k = z; k; k -= lowbit(k)) {
				s ^= c[i][j][k];
			}
		}
	}
	return s;
}

int main() {
	int m;
	while (scanf("%d %d", &n, &m) != EOF) {
		memset(c, 0, sizeof(c));
		while (m--) {
			int tp;
			scanf("%d", &tp);
			if (!tp) {
				int x, y, z;
				scanf("%d %d %d", &x, &y, &z);
				printf("%d\n", sum(x, y, z));
			}
			else {
				int x1, y1, z1, x2, y2, z2;
				scanf("%d %d %d %d %d %d", &x1, &y1, &z1, &x2, &y2, &z2);
				add(x1, y1, z1, 1);
				add(x2 + 1, y1, z1, -1);
				add(x1, y2 + 1, z1, -1);
				add(x1, y1, z2 + 1, -1);
				add(x2 + 1, y2 + 1, z1, 1);
				add(x1, y2 + 1, z2 + 1, 1);
				add(x2 + 1, y1, z2 + 1, 1);
				add(x2 + 1, y2 + 1, z2 + 1, -1);
			}
		}
	}
	return 0;
}
