#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;


//************************************ 分组背包 模板 ************************************
const int MAXN = 110;           // 物品数量 
const int MAXC = 10010;           // 背包容量 
typedef int ValueType;            // 背包价值的类型 
const ValueType inf = -1e9;       // 求最大值则最小，求最小值则最大 
const ValueType init = 0;         // 一般无论求最大值还是最小值，都固定为 0 

ValueType dp[MAXN][MAXC];

struct Knapsack {
    int groupId;
    int capacity;
    ValueType weight;
    Knapsack(){
    }
    Knapsack(int g, int c, ValueType w) : groupId(g), capacity(c), weight(w) {
    }

    bool operator < (const Knapsack& knap) const {
        return groupId < knap.groupId;
    }
}Knap[MAXN];

struct GroupKnapsack {
    vector <Knapsack> items;
    void clear() {
        items.clear();
    }
    void add(const Knapsack& knap) {
        items.push_back(knap);
    }
    int size() const {
        return items.size();
    }
    int getGroupId() {
        if (size()) {
            return items[0].groupId;
        }
        return -1;
    }
    const Knapsack& get(int idx) const {
        return items[idx];
    }
}GKnap[MAXN];

ValueType opt(ValueType x, ValueType y) {
    return x > y ? x : y;
}

void groupKnapsackInit(int maxCapacity) {
    for (int i = 0; i <= maxCapacity; ++i) {
        dp[0][i] = (!i) ? init : inf;
    }
}

int groupKnapsackRegroup(int knapsackSize, Knapsack *knap) {
    sort(knap, knap + knapsackSize);
    int groupId = -1;
    for (int i = 0; i < knapsackSize; ++i) {
        if (groupId == -1 || knap[i].groupId != GKnap[groupId].getGroupId())
            GKnap[++groupId].clear();

        GKnap[groupId].add(knap[i]);
    }
    return ++groupId;
}

// 注意：
// 1）返回组数，用于和原组数作判断用
// 2）容量有可能为0，注意状态转移顺序：先计算当前组、再计算上一组
int groupKnapsack(int knapsackSize, Knapsack *knap, int maxCapacity) {
    groupKnapsackInit(maxCapacity);
    int groupSize = groupKnapsackRegroup(knapsackSize, knap);
    for (int k = 1; k <= groupSize; ++k) {
        for (int j = 0; j <= maxCapacity; ++j) {
            dp[k][j] = inf;
        }
        for (int i = 0, s = GKnap[k-1].size(); i < s; ++i) {
            const Knapsack &item = GKnap[k-1].get(i);
            for (int j = maxCapacity; j >= item.capacity; --j) {
                // 三种选择：
                // 1) 从当前组状态过来，则这个物品可以不选：dp[i + 1][j]
                // 2）从当前组状态过来，则这个物品可以选：dp[i + 1][j - item.capacity] + item.weight)
                // 3）从上一组状态过来，则这个物品必须选：dp[i][j - item.capacity] + item.weight
                dp[k][j] = opt(dp[k][j], dp[k][j - item.capacity] + item.weight);
                dp[k][j] = opt(dp[k][j], dp[k-1][j - item.capacity] + item.weight);
            }
        }
    }
    return groupSize;
}

//************************************ 分组背包 模板 ************************************