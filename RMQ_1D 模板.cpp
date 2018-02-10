#include <iostream>

using namespace std;

// RMQ 区间最小值询问
// 该模板用于求区间最小值，如果要求最大值，可以将所有数去相反数，然后求最小值后再取反即可。
 
#define MAXN 50010
#define MAXM 17

// typedef __int64 ValueType;
// typedef double ValueType;
typedef int ValueType;

int lg2K[MAXN];

// n val[]    表示元素组的大小和元素值   -in 
// ret[i][j]  表示求得的RMQ数组          -out  
void RMQ_Init(int n, ValueType val[], int (*ret)[MAXN]) {
	int i, j, k = 0;
	for(i = 1; i <= n; i++) {
		lg2K[i] = k - 1;
		if((1<<k) == i) k++;
	}
	for(i = 0; i < MAXM; i++) {
		for(j = 1; j <= n; j++) {
			if(i == 0) {
				ret[i][j] = j;
			}else {
				// ret[i][j] = getMinIdx( ret[i-1][j], ret[i-1][ j+(1<<i-1) ] );
				ret[i][j] = ret[i-1][j];
				int ridx = j + (1<<i-1);
				if ( ridx <= n ) {
					int r = ret[i-1][ridx];
					if( val[r] < val[ ret[i][j] ] ) {
						ret[i][j] = r;
					}
				}
			}
		}
	}
}

/*
    设区间长度为2^k，则X表示的区间为[a, a + 2^k)，Y表示的区间为(b - 2^k, b]，
则需要满足一个条件就是X的右端点必须大于等于Y的左端点-1，即 a+2^k-1 >= b-2^k，
则2^(k+1) >= (b-a+1), 两边取对数（以2为底），得 k+1 >= lg(b-a+1)，则k >= lg(b-a+1) - 1。
k只要需要取最小的满足条件的整数即可( lg(x)代表以2为底x的对数 )。
*/
int RMQ_Query(ValueType val[], int (*rmq)[MAXN], int a, int b) {
	if(a == b) {
		return a;
	}else if(a > b) {
		a = a^b, b = a^b, a = a^b;
	}
	int k = lg2K[ b-a+1 ];
	return val[ rmq[k][a] ] < val[ rmq[k][b-(1<<k)+1] ] ? rmq[k][a] : rmq[k][b-(1<<k)+1];
}

int Max[MAXM][MAXN], Min[MAXM][MAXN];
ValueType val[MAXN], unval[MAXN];
int n,  m;

int main() {
	int i;
	while( scanf("%d %d", &n, &m) != EOF ) {
		for(i = 1;i <= n; i++) {
			scanf("%d", &val[i]);
			unval[i] = -val[i];
		}
		RMQ_Init(n, val, Min);
		RMQ_Init(n, unval, Max);
		while( m-- ) {
			int x, y;
			scanf("%d %d", &x, &y);
			int minidx = RMQ_Query(val, Min, x, y);
			int maxidx = RMQ_Query(unval, Max, x, y);
			ValueType ret = -unval[maxidx] - val[minidx];
			printf("%d\n", ret);
		}
	}
	return 0;
}
