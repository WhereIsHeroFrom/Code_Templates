#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

//************************************ K 大 01背包 模板 ************************************
const int MAXN = 110;             // 物品数量 
const int MAXC = 1010;            // 背包容量
const int MAXK = 31;              // 第 K 大
typedef int ValueType;            // 背包价值的类型 
const ValueType inf = -1e9;       // 求最大值则最小，求最小值则最大 
const ValueType init = 0;         // 一般无论求最大值还是最小值，都固定为 0 

ValueType dp[MAXC][MAXK];

struct Knapsack {
    int capacity;
    ValueType weight;
    Knapsack(){
    }
    Knapsack(int c, ValueType w) : capacity(c), weight(w) {
    }
}Knap[MAXN];

void opt(int K, ValueType* x, ValueType* y, ValueType *res) {
    ValueType totala[MAXK << 1];
    int s = 0;
    for (int i = 0; i < K; ++i) {
        totala[s++] = x[i];
        totala[s++] = y[i];
    }

    sort(totala, totala + s, greater<int>());
    int residx = 0;
    for (int i = 0; i < s; ++i) {
        if (residx == 0 || totala[i] != res[residx - 1])
            res[residx++] = totala[i];
        if (residx == K) return;
    }
}

void zeroOneKthKnapsackInit(int maxCapacity) {
    for (int i = 0; i <= maxCapacity; ++i) {
        for (int j = 0; j < MAXK; ++j)
            dp[i][j] = (!i && !j) ? init : inf;
    }
}

// dp[i][j][0...k] = maxk{ dp[i-1][j][0...k], dp[i-1][j - c[i]][0...k] + w[i] }
void zeroOneKthKnapsack(int knapsackSize, int K, Knapsack *knap, int maxCapacity) {
    zeroOneKthKnapsackInit(maxCapacity);
    for (int i = 0; i < knapsackSize; ++i) {
        for (int j = maxCapacity; j >= knap[i].capacity; --j) {
            ValueType o[MAXK];
            for (int k = 0; k < K; ++k) {
                o[k] = dp[j - knap[i].capacity][k] + knap[i].weight;
            }
            opt(K, dp[j], o, dp[j]);
        }
    }
}

//************************************ K 大 01背包 模板 ************************************

