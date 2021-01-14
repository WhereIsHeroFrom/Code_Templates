#include <iostream>
#include <cstring>

using namespace std;

//****** 二分图最大匹配模板(邻接表) ******
// 顶点范围  左边的点[1, X]，右边的点[1,Y]

const int BGMAX = 510;

class BipartiteGraph {
public:
    BipartiteGraph();
    virtual ~BipartiteGraph();

    void initialize(int lefts, int rights);
    void addEdge(int u, int v);

    int getMaxMatch();
private:
    bool findMatch(int u);       // 匈牙利算法找增广路
private:
    int leftsize_, rightsize_;   // 左、右点集的数目
    int *adj[BGMAX];             // 邻接表
    int adjsize_[BGMAX];         // 邻接表的size
    int pre_[BGMAX];             // 匈牙利算法找增广路时的 pre
    bool visit_[BGMAX];          // 匈牙利算法找增广路时的 visit
};

BipartiteGraph::BipartiteGraph() :
leftsize_(0), rightsize_(0)
{
    for (int i = 1; i < BGMAX; ++i) {
        adj[i] = new int[BGMAX];
    }
}

BipartiteGraph::~BipartiteGraph() {
    for (int i = 1; i < BGMAX; ++i) {
        delete[] adj[i];
        adj[i] = NULL;
    }
}

bool BipartiteGraph::findMatch(int u) {
    // u -> v
    for (int i = 0; i < adjsize_[u]; ++i) {
        int v = adj[u][i];
        if (!visit_[v]) {
            visit_[v] = true;
            int vpre = pre_[v];
            pre_[v] = u;
            if (vpre == -1 || findMatch(vpre)) {
                return true;
            }
            pre_[v] = vpre;
        }
    }
    return false;
}

void BipartiteGraph::initialize(int lefts, int rights) {
    leftsize_ = lefts;
    rightsize_ = rights;
    memset(pre_, -1, sizeof(pre_));
    memset(adjsize_, 0, sizeof(adjsize_));
}


void BipartiteGraph::addEdge(int u, int v) {
    adj[u][adjsize_[u]++] = v;
}

int BipartiteGraph::getMaxMatch() {
    int cnt = 0;
    for (int i = 1; i <= leftsize_; ++i) {
        memset(visit_, false, sizeof(visit_));
        if (findMatch(i)) {
            ++cnt;
        }
    }
    return cnt;
}
//****** 二分图最大匹配模板(邻接表) ******