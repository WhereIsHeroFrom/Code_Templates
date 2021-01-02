/*
题意：
    给定N(N <= 200000)个元素的序列A[i]，再给定一些三元组(i, j, k)，表示A[i] + A[i+1] + ... A[j] = k
问M个三元组中错误的数量有多少个（当某个三元组和之前的情况冲突的时候表明它是错的，并且忽略它）。
 
题解： 
    并查集
    令S[i]为数列A的前i项和，那么A[i] + A[i+1] + ... A[j] = k可以表示为 S[j] - S[i-1] = k，那么将j和i-1
看成两个点x和y (x = j, y = i-1)。 
    进行并查集的查找操作，如果x和y不在一个集合中，则将他们合并到一个集合，这里必然会牵涉到x和y的父子关
系，于是引入一个差值的概念，令d[i] = S[i] - S[pre[i]]，其中pre[i]代表并查集的树中i的父亲结点，特殊的，
当i为并查集森林中的根结点的时候i == pre[i]，即d[i] = S[i] - S[i] = 0。这样就可以通过从子结点往根结点回
溯将经过的所有d值累加，就能计算出S[x]的值了(S[x] = d[x] + d[pre[x]] + ... + d[root])。
    算法如下：
    1) 
        计算x相对于它根结点的差值 x2fx = getRelToRootVal(x); // S[x] - S[fx] = x2fx;
        计算y相对于它根结点的差值 y2fy = getRelToRootVal(y); // S[y] - S[fy] = y2fy;
    2)  如果x和y不在一个集合中： 
        合并x和y 
        a) 如果合并后xroot为yroot的子结点 
            val[xroot] = S[xroot] - S[yroot] = (S[x] - x2fx) - (S[y] - y2fy) = (S[x] - S[y]) + (y2fy - x2fx) = -z + (y2fy - x2fx);
        b) 如果合并后yroot为xroot的子结点 
            val[yroot] = S[yroot] - S[xroot] = (S[y] - y2fy) - (S[x] - x2fx) = (S[y] - S[x]) + (x2fx - y2fy) = z + (x2fx - y2fy);
    3)  如果x和y不在一个集合  
         z 不等于  y2fy - x2fx ，表示这对x和y不合法，答案累加； 
*/
#include <iostream>

using namespace std;

#define LL __int64
#define MAXN 200010

int pre[MAXN], h[MAXN];

int unionset_find(int p) {
    int x = p;
    while(pre[p] != p) {
        p = pre[p];
    }
    /*while(x != p) {
        int tmp = pre[x];
        pre[x] = p;
        x = tmp;
    }*/
    return p;
}

int unionset_union(int x, int y) {
    int fx = unionset_find(x);
    int fy = unionset_find(y);
    if(x == y) {
        return false;
    }else {
        if( h[fx] == h[fy] ) {
            pre[fy] = fx;
            h[fx]++;
        }else if( h[fx] < h[fy] ) {
            pre[fx] = fy;
        }else {
            pre[fy] = fx;
        }
        return true;
    }
}

LL val[ MAXN ];

// 得到 sum[x] 的值 
int getRelToRootVal(int p) {
    int sum = 0;
    while(pre[p] != p) {
        sum += val[p];
        p = pre[p];         
    }
    return sum;
}

int n, m;

int main() {
    int i;
    while( scanf("%d %d", &n, &m) != EOF ) {
        for(i = 0; i <= n; i++) {
            pre[i] = i;
            h[i] = 1;
            val[i] = 0;
        }
        int cnt = 0;
        while(m--) {
            int x, y, z;
            
            scanf("%d %d %d", &x, &y, &z);
            x--;
            
            // s[y] - s[x] = z
            int fy = unionset_find(y);
            int fx = unionset_find(x);
            if( fx != fy ) {
                int x2fx = getRelToRootVal(x); // s[x] - s[fx] = x2fx;
                int y2fy = getRelToRootVal(y); // s[y] - s[fy] = y2fy;
                unionset_union(x, y);
                if(pre[fx] == fy) {
                    // val[fx] = s[fx] - s[fy] = (s[x] - x2fx) - (s[y] - y2fy) = (s[x] - s[y]) + (y2fy - x2fx);
                    val[fx] = -z + (y2fy - x2fx);
                }else {
                    // val[fy] = s[fy] - s[fx] = (s[y] - y2fy) - (s[x] - x2fx) = (s[y] - s[x]) + (x2fx - y2fy);
                    val[fy] = z + (x2fx - y2fy);
                }
            }else {
                int x2fx = getRelToRootVal(x); // s[x] - s[root] = x2fx;
                int y2fy = getRelToRootVal(y); // s[y] - s[root] = y2fy;
                if( z != y2fy - x2fx ) cnt++;
            }
        }
        printf("%d\n", cnt);
    }
}

