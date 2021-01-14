#pragma   comment(linker, "/STACK:1024000000,1024000000")

#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

//****** 二分图最大匹配模板(链式前向星) ******

const int VMAX = 2510;
const int EMAX = 2510 * 24;

struct Edge {
    int to, next;

    Edge() {}
    Edge(int _t, int _n) : to(_t), next(_n) {}
};

class BipartiteGraph {
public:
    BipartiteGraph();
    virtual ~BipartiteGraph();
    void initialize(int n);
    void addEdge(int u, int v);
    int getTreeMaxMatch();
    int getMaxMatch();
    void getMaxMatchPath(int& maxMatch, int left[], int right[]);
    bool isBipartite();
private:
    int max(int a, int b);
    bool findMatch(int u);               // 匈牙利算法找增广路
    void findTreeMatch(int fat, int u);
    void dye();
    bool dyeBFS(int u);                  // bfs 对顶点进行染色
    void doAddEdge(int u, int v);

    bool isVisited(int v);
    void setVisit(int v);
private:
    // 注意这是：左、右点集加起来的数目
    int vertexcount_;

    //  链式前向星  
    Edge *edges_;                // 边集
    int edgecount_;              // 已用边数
    int head_[VMAX];             // 每个节点边集的链表头

    // 染色
    int color_[VMAX];

    int pre_[VMAX];              // 匈牙利算法找增广路时的 pre
    int visit_[VMAX];            // 匈牙利算法找增广路时的 visit
    int visittag_;

    // 树形DP 求匹配
    int dp_[VMAX][2];            // 利用树形DP求树的最大匹配
    // 0 以当前结点为父结点，和子结点之间没有匹配边
    // 1 以当前结点为父节点，和子结点有一条匹配边（也顶多一条了）
};

BipartiteGraph::BipartiteGraph() :
edges_(NULL), vertexcount_(0)
{
    edges_ = new Edge[EMAX];
}

BipartiteGraph::~BipartiteGraph() {
    if (edges_) {
        delete[] edges_;
    }
}

int BipartiteGraph::max(int a, int b)
{
    return a > b ? a : b;
}

bool BipartiteGraph::findMatch(int u) {
    // u -> v
    if (u == -1) {
        return true;
    }
    for (int e = head_[u]; ~e; e = edges_[e].next) {
        int v = edges_[e].to;
        if (!isVisited(v)) {
            setVisit(v);
            if (findMatch(pre_[v])) {
                pre_[v] = u;
                return true;
            }
        }
    }
    return false;
}


void BipartiteGraph::findTreeMatch(int fat, int u) {
    dp_[u][0] = 0;
    for (int e = head_[u]; ~e; e = edges_[e].next) {
        int v = edges_[e].to;
        if (v == fat) {
            continue;
        }
        findTreeMatch(u, v);
        dp_[u][0] += max(dp_[v][0], dp_[v][1]);
    }

    dp_[u][1] = 0;
    for (int e = head_[u]; ~e; e = edges_[e].next) {
        int v = edges_[e].to;
        if (v == fat) {
            continue;
        }
        // 选择 (u, v) 作为匹配边，那么 v 往下选择的时候必然不能选择匹配边，所以取 dp_[v][0] 的情况
        int s = dp_[u][0] - max(dp_[v][0], dp_[v][1]) + 1 + dp_[v][0];
        if (s > dp_[u][1]) {
            dp_[u][1] = s;
        }
    }
}

void BipartiteGraph::dye() {
    memset(color_, -1, sizeof(color_));
    for (int i = 0; i <= vertexcount_; ++i) {
        if (color_[i] == -1) {
            dyeBFS(i);
        }
    }
}

bool BipartiteGraph::dyeBFS(int u) {
    queue <int> Q;
    Q.push(u);
    color_[u] = 0;

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();


        for (int e = head_[u]; ~e; e = edges_[e].next) {
            int v = edges_[e].to;
            if (color_[v] == -1) {
                color_[v] = 1 - color_[u];
                Q.push(v);
            }
            else if (color_[v] == color_[u]) {
                return false;
            }
        }
    }
    return true;
}

void BipartiteGraph::doAddEdge(int u, int v) {
    edges_[edgecount_] = Edge(v, head_[u]);
    head_[u] = edgecount_++;
}

bool BipartiteGraph::isVisited(int v)
{
    return visit_[v] == visittag_;
}

void BipartiteGraph::setVisit(int v)
{
    visit_[v] = visittag_;
}

void BipartiteGraph::initialize(int n) {
    vertexcount_ = n;
    edgecount_ = 0;
    visittag_ = 0;
    memset(head_, -1, sizeof(head_));
    memset(visit_, 0, sizeof(visit_));
}

void BipartiteGraph::addEdge(int u, int v) {
    doAddEdge(u, v);
    doAddEdge(v, u);
}

int BipartiteGraph::getMaxMatch() {
    dye();
    int cnt = 0;
    memset(pre_, -1, sizeof(pre_));
    for (int i = 0; i <= vertexcount_; ++i) {
        if (color_[i] == 0) {
            ++visittag_;
            if (findMatch(i)) {
                ++cnt;
            }
        }
    }
    return cnt;
}

int BipartiteGraph::getTreeMaxMatch() {
    // 建立虚根 0
    addEdge(0, 1);
    findTreeMatch(0, 1);
    return max(dp_[1][0], dp_[1][1]);
}


void BipartiteGraph::getMaxMatchPath(int& maxMatch, int left[], int right[]) {
    maxMatch = getMaxMatch();
    int matchCnt = 0;
    for (int i = 0; i <= vertexcount_; ++i) {
        if (color_[i] == 1 && pre_[i] != -1) {
            left[matchCnt] = pre_[i];
            right[matchCnt] = i;
            ++matchCnt;
        }
    }
}

bool BipartiteGraph::isBipartite() {
    memset(color_, -1, sizeof(color_));
    for (int i = 0; i <= vertexcount_; ++i) {
        if (color_[i] == -1 && !dyeBFS(i)) {
            return false;
        }
    }
    return true;
}

//****** 二分图最大匹配模板(链式前向星) ******