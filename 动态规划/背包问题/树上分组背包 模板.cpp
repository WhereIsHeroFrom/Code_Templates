#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;


//************************************ 树上分组背包 模板 ************************************
const int MAXN = 110;             // 物品数量 
const int MAXC = 510;             // 背包容量 
typedef int ValueType;            // 背包价值的类型 
const ValueType inf = -1e9;       // 求最大值则最小，求最小值则最大 
const ValueType init = 0;         // 一般无论求最大值还是最小值，都固定为 0 

ValueType dp[MAXN][MAXC];

struct Knapsack {
    int capacity;
    ValueType weight;
    Knapsack(){
    }
    Knapsack(int c, ValueType w) : capacity(c), weight(w) {
    }
}Knap[MAXN];

ValueType opt(ValueType x, ValueType y) {
    return x > y ? x : y;
}

vector <int> edges[MAXN];

void treeGroupKnapsack(int father, int u, int m) {
    int i, j, k;
    int cap[2][MAXC], pre = 0;

    int maxCapacity = m - Knap[u].capacity;
    for (i = 0; i <= maxCapacity; ++i)              // 1、分组背包容量初始化
        cap[0][i] = 0;
    for (i = 0; i < edges[u].size(); ++i) {         // 2、分组背包枚举组（1个子树1个组）
        int v = edges[u][i];
        if (v == father) continue;
        treeGroupKnapsack(u, v, maxCapacity);       // 3、递归求解子结点所有组的状态 dp[v][0 ... capacity]
        for (j = maxCapacity; j >= 0; --j) {        // 4、分组背包逆序枚举容量
            cap[pre ^ 1][j] = cap[pre][j];          // 5、这个组不选物品的情况，继承自上一组
            for (k = 0; k <= j; ++k) {               
                                                    // 6、(容量为k, 价值为dp[v][k]) 的物品进行决策
                                                    // 注意一定要考虑容量为 0 的情况
                cap[pre^1][j] = opt(
                    cap[pre^1][j],
                    cap[pre][j - k] + dp[v][k]
                );
            }
        }
        pre ^= 1;
    }
    for (i = m; i >= Knap[u].capacity; --i)         // 6、父结点必选
        dp[u][i] = cap[pre][i - Knap[u].capacity] + Knap[u].weight;
    for (i = 0; i < Knap[u].capacity; ++i) 
        dp[u][i] = 0;
    
    // 如果某个子结点为 0 的情况，不能走，则打开注释
    // dp[u][0] = 0;
}

//************************************ 树上分组背包 模板 ************************************