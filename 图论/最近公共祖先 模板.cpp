#include <iostream>
#include <vector>
#include <map>
using namespace std;

// RMQ 区间最小值询问
// 该模板用于求区间最小值，如果要求最大值，可以将所有数去相反数，然后求最小值后再取反即可。
// LCA 构造的时候点数MAXN需要是规模的两倍，因为记录边的时候记录的是双向边 
#define MAXN 200020
#define MAXM 20

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


/*
LCA 转化为 RMQ
       首先要有一棵树，以7个顶点，6条边为例:
              7
              (0,5) (5,2) (2,4) (3,0) (1,3) (3,6)
       1)如果根结点不确定，那么定义0为根结点，然后从根结点开始进行一次深度优先遍历，原有边是M条，那么一次遍历完毕就会有2M条边。
              (0,5) -> (5,2) -> (2,4) -> (4,2) -> (2,5) -> (5,0) -> (0,3) -> (3,1) -> (1,3) -> (3,6) -> (6,3) -> (3,0)
       2)由于相邻两条边的端点是一样的，所以可以压缩到一个一维数组E (edge)中：
              E[i] = 0 5 2 4 2 5 0 3 1 3 6 3 0
         将E数组中对应点在树上的深度记录在数组D (depth)中：
              D[i] = 0 1 2 3 2 1 0 1 2 1 2 1 0
       3)将每个点在E数组中第一次出现的位置记录在I (index)中：
              I[i] = 0 8 2 7 3 1 10
       4) 然后得到结点x和y的最近公共祖先：
              LCA(x, y) = E[ RMQ_MIN( D, I[x], I[y] ) ]
*/

int edgeCount;
int E[MAXN], I[MAXN];
ValueType D[MAXN];
int D_RMQ[MAXM][MAXN];

// 存储树上的邻接边 
vector <int> edge[MAXN];

void addEdgeVertex(int vertex, int depth) {
    ++edgeCount;
    E[ edgeCount ] = vertex;
    D[ edgeCount ] = depth;        
}

void LCA_Dfs(int fat, int u, int depth) {
    addEdgeVertex(u, depth);
    for(int i = 0; i < edge[u].size(); i++) {
        int v = edge[u][i];
        if(v == fat) {
            // 处理双向边的情况，针对无根树 
            continue; 
        }
        LCA_Dfs(u, v, depth+1);
        addEdgeVertex(u, depth);    
    }    
}

// 这里的树的顶点集合
// 可以是 [0, n)，也可以是[1, n]，主要影响的是I数组的初始化 
void LCA_Init(int n, int root) {
    int i;
    
    edgeCount = 0;
    LCA_Dfs(-1, root, 0);
    RMQ_Init(edgeCount, D, D_RMQ);
    
    for(i = 0; i <= n; i++) {
        I[i] = -1;
    }
    for(i = 1; i <= edgeCount; i++) {
        if( I[ E[i] ] == -1 ) {
            I[ E[i] ] = i;
        }
    }
    
}

int LCA_Query(int x, int y) {
    return E[ RMQ_Query(D, D_RMQ, I[x], I[y]) ];
}
