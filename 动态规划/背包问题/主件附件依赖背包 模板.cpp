#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;


//************************************ 依赖背包 模板 ************************************
//**************************** 依赖只有一层的特殊依赖背包问题 ****************************

const int MAXN = 55;              // 物品数量
const int MAXC = 100010;          // 背包容量 
typedef int ValueType;            // 背包价值的类型 
const ValueType inf = -1e9;       // 求最大值则最小，求最小值则最大 
const ValueType init = 0;         // 一般无论求最大值还是最小值，都固定为 0 

ValueType dp[2][MAXC];

enum KnapsackType {
    KT_MASTER = 0,
    KT_SLAVE = 1,
};

struct Knapsack {
    int capacity;
    ValueType weight;
    Knapsack(){
    }
    Knapsack(int c, ValueType w) : capacity(c), weight(w) {
    }
};

struct MasterKnapsack : public Knapsack {
    vector <Knapsack> slaves;
    MasterKnapsack(){}

    MasterKnapsack(int c, ValueType w) {
        capacity = c;
        weight = w;
        slaves.clear();
    }
    void add(const Knapsack& k) {
        slaves.push_back(k);
    }
    int size() const {
        return slaves.size();
    }
    const Knapsack& get(int idx) const {
        return slaves[idx];
    }
}MKnap[MAXN];

ValueType opt(ValueType x, ValueType y) {
    return x > y ? x : y;
}

// 依赖一层的特殊依赖背包问题，用于解决购买 '附件' 必须先购买 '主件' 的问题
int relyOnelayerKnapsack(int knapsackSize, MasterKnapsack* knap, int maxCapacity) {
    int cur = 0, last = 1;
    memset(dp, 0, sizeof(dp));
    for (int i = 0; i < knapsackSize; ++i) {
        const MasterKnapsack &mItem = knap[i];

        // 1. 这里的目的是假设的 i 个盒子一定要买的情况
        for (int j = 0; j <= maxCapacity; ++j) {
            if (j < mItem.capacity) {
                // 1.1 买不起，无限凄凉
                dp[cur][j] = inf;
            }
            else {
                // 1.2 买得起，那就把箱子买下来，这一步有点类似 0/1 背包中的 1
                dp[cur][j] = dp[last][j - mItem.capacity] + mItem.weight;
            }
        }

        // 2. 现在，当前的箱子已经买下来了，然后就可以开始进行箱子内部物品的 0/1 背包了
        for (int j = 0; j < mItem.size(); ++j) {
            const Knapsack &sItem = mItem.get(j);
            for (int k = maxCapacity; k >= sItem.capacity; --k) {
                dp[cur][k] = opt(dp[cur][k], dp[cur][k - sItem.capacity] + sItem.weight);
            }
        }

        // 3. 最后，我们比较一下到底是买比较优，还是不买比较优
        for (int j = maxCapacity; j >= 0; --j) {
            dp[cur][j] = opt(dp[cur][j], dp[last][j]);
        }

        cur ^= 1;
        last = 1 - cur;
    }
    return dp[last][maxCapacity];
}

//**************************** 依赖只有一层的特殊依赖背包问题 ****************************
//************************************ 依赖背包 模板 ************************************

