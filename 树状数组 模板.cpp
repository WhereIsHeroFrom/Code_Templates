/*
	树状数组 模板

	lowbit
	add
	sum

	Author: WhereIsHeroFrom
	Update Time: 2020-10-24
	Algorithm Complexity: O(log(n))
*/

#define MAXV 100010
#define LL __int64

LL c[MAXV + 1];
int n;

int lowbit(int x) {
	return x & -x;
}

void add(int x, LL v) {
	while (x < MAXV) {
		c[x] += v;
		x += lowbit(x);
	}
}

LL sum(int x) {
	LL s = 0;
	while (x) {
		s += c[x];
		x ^= lowbit(x);
	}
	return s;
}
