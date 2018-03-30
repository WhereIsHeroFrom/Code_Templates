/*
斜率优化的DP (2D)
    对于状态转移方程：dp[i][j] = min{ dp[i-1][k] + f(j) - h(j)*g(k) + d(k) }
    这里的k就是决策，利用的就是决策的单调性。
    我们现在要枚举i，j，然后在O(1)的时间确定k的取值，所以这种情况下，i和j都是常数，k是未知数。
于是f(j)和h(j)都是已知的，所以我们将这些值进行归类。

1、将所有只和i、j有关的变量归为一类，作为直线的截距b；b = dp[i][j]-f(j)
2、将j和k相乘的部分提出来，h(j)让它保持单调递增，作为斜率K，g(k)则为自变量x；
3、剩下的部分就是应变量y了；应变量y一般和i、k有关，即dp[i-1][k]+d(k)。
   得到直线方程y = kx+b
          dp[i-1][k]+d(k) = h(j)*g(k) + dp[i][j]-f(j)
   
Author: WhereIsHeroFrom
Update Time: 2018-3-30
Algorithm Complexity: O(nm) 
*/

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

#define MAXN 1010
#define LL int
int n, m;
int a[MAXN], b[MAXN];
int sa[MAXN], sb[MAXN];
int dpIdx;
LL dp[MAXN][MAXN];

// 自变量x，和决策k相关 
LL X(int k) {
	return sa[k];
}

// 应变量y，和决策k以及上一层状态相关 
LL Y(int k) {
	return sb[k] + dp[dpIdx-1][k];
}

// 斜率K，只取决于j，且单调递增 
LL K(int j) {
	return sa[j];
}

// y = Kx + b，其中b = dp[i][j] + B; 
// y = Kx + dp[i][j] + B
LL B(int j) {
	return sb[j-1] - sa[j-1]*sa[j];
}

// 于是 dp[i][j] =  y - Kx - B

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
	int i, j; 
	// 1.定义非法状态
	memset(dp, -1, sizeof(dp));
	// 2.定义初始状态
	dp[0][0] = 0;
	// 3.枚举i，j, 计算dp[i][j]
	// 状态转移如下： dp[i][j] <- dp[i-1][k]
	// i放置在外层，这样可以保证 K(j) 始终单调递增 
 	for(i =	1; i <= m; ++i) {
 		head = tail = 0;
 		dpIdx = i;
 		for(j = 1; j <= n; ++j) {
			// 4.如果dp[i-1][j-1]这个状态合法，那么可以加入单调队列
			if(dp[i-1][j-1] != STATE_INVALID) {
				// 4.1 维护“下凸”折线
				while(head + 1 < tail && isBigger(deq[tail-2], deq[tail-1], j-1))
					--tail;
				// 4.2 插入单调队列尾部
				deq[tail++] = j-1;
			}
			// 5.删除所有“永不录用”点
			while(head + 1 < tail && isSmaller(deq[head], deq[head+1], K(j)))
				++head;
	
			// 6. 更新y = kx + b
			dp[i][j] = Y(deq[head]) - K(j)*X(deq[head]) - B(j);		 
		}
	}
	return dp[m][n];
}

int main() {
	int i;
	while(scanf("%d %d", &n, &m) != EOF) {
		if(!n && !m) {
			break;
		}
		for(i = 1; i <= n; ++i) {
			scanf("%d", &a[i]);
			sa[i] = sa[i-1] + a[i];
			b[i] = a[i] * sa[i];
			sb[i] = sb[i-1] + b[i];
		}
		++m;
		printf("%d\n", solve());
	}
	return 0;
}
