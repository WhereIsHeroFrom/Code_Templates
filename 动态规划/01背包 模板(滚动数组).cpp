#include <iostream>
#include <cstring>
using namespace std;


//************************************ 01背包 模板 ************************************
const int MAXN = 5010;     // 物品数量 
const int MAXC = 250010;   // 背包容量 
const int inf = false;     // dp数组的非法值 
const int init = true;     // dp数组的初始值 
typedef bool ValueType;

ValueType dp[2][MAXC];

struct Knapsack {
	int capacity;
	int weight;
	Knapsack(){
	} 
	Knapsack(int c, int w) : capacity(c), weight(w) {
	}
}Knap[MAXN];

ValueType opt(ValueType x, ValueType y) {
	return x || y;
}

void zeroOneKnapsackInit() {
	memset(dp, inf, sizeof(dp));
	dp[0][0] = true;
}

void zeroOneKnapsackRollClear(int index, int maxCapacity) {
	for(int i = 0; i <= maxCapacity; ++i) {
		dp[index][i] = inf;
	}
}

int zeroOneKnapsack(int knapsackSize, Knapsack *knap) {
	zeroOneKnapsackInit();
	int maxCapacity = 0;
	int pre = 0;
	 
	for(int i = 0; i < knapsackSize; ++i) {
		maxCapacity += knap[i].capacity;
		zeroOneKnapsackRollClear(pre^1, maxCapacity);
		for(int j = 0; j <= maxCapacity; ++j) {
			if( j >= knap[i].capacity )
				dp[pre^1][j] = opt(dp[pre][j], dp[pre][j - knap[i].capacity] + knap[i].weight);
			else
				dp[pre^1][j] = dp[pre][j]; 
		}
		pre ^= 1;
	}
	return pre;
}

//************************************ 01背包 模板 ************************************

