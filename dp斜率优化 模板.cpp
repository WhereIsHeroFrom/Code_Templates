/*
  题意：给定N(N <= 400000)个数a[N]，现在需要将它们归类，每类至少T(1 < T <= N)个数。归在
  同一类的花费为每个数字减去最小那个数字之和。希望所有数归类后的总花费最小，求这个最小花费。

  题解：dp[i] = {dp[j] + sum[i]-sum[j] - (i-j)a[j+1] | 0<=j<i}
		
		b = dp[i] - sum[i]
		y = dp[j] - sum[j] + j*a[j+1]
		k = i
		x = a[j+1]

		y = kx + b

		单调队列维护 “下凸” 折线，单调队列内存储的点为可转移到状态i的状态j。
*/

#include <iostream>
#include <algorithm>

using namespace std;

#define MAXN 400010
#define LL long long
int N, T;
int a[MAXN];
LL sum[MAXN], dp[MAXN];

LL X(int idx) {
	return a[idx+1];
}

LL Y(int idx) {
	return (dp[idx] - sum[idx] + (LL)idx * (LL)a[idx+1]);
}

LL K(int i) {
	return i;
}

// 斜率判定：K(A, B) >= K(B, C)
bool isBigger(int A, int B, int C) {
	// K(A, B) = (Y(B)-Y(A)) / (X(B)-X(A))
	// K(C, B) = (Y(C)-Y(B)) / (X(C)-X(B))
	LL XB = X(B);
	LL YB = Y(B);
	return (YB-Y(A))*(X(C)-XB) >= (Y(C)-YB)*(XB-X(A));
}

// 斜率判定：K(A, B) <= slope
bool isSmaller(int A, int B, LL slope) {
	// K(A, B) = (Y(B)-Y(A)) / (X(B)-X(A))
	return (Y(B)-Y(A)) <= slope*(X(B)-X(A));
}

int deq[MAXN];
int head, tail;
#define STATE_INVALID -1

LL solve() {
	// 1.单调队列置空
	head = tail = 0;
	// 2.定义初始状态
	dp[0] = 0;
	// 3.定义非法状态(数的个数不足T个，无法归为一类，所以最优解不存在)
	for(int i = 1; i < T; ++i) {
		dp[i] = STATE_INVALID;
	}
	// 4.枚举i，计算dp[i] (T<=i<=N)
 	for(int i =	T; i <= N; ++i) {
		// 5.如果dp[i-T]这个状态合法，那么可以加入单调队列
		if(dp[i-T] != STATE_INVALID) {
			// 5.1 维护“下凸”折线
			while(head + 1 < tail && isBigger(deq[tail-2], deq[tail-1], i-T))
				--tail;
			// 5.2 插入单调队列尾部
			deq[tail++] = i-T;	
		}

		// 6.删除所有“永不录用”点
		while(head + 1 < tail && isSmaller(deq[head], deq[head+1], i))
			++head;

		// 7. 更新dp[i] = y - kx + sum[i]
		dp[i] = Y(deq[head]) - K(i)*X(deq[head]) + sum[i];
	}
	return dp[N];
}

int main() {
	int i;
	while(scanf("%d %d", &N, &T) != EOF) {
		for(i = 1; i <= N; i++) {
			scanf("%d", &a[i]);
		}
		sort(a + 1, a + N + 1);
		for(i = 1; i <= N; i++) {
			sum[i] = sum[i-1] + a[i];
		}
		printf("%lld\n", solve());
	}
	return 0;
}