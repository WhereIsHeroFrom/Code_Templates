#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

//************ 迷宫类 广度优先搜索 模板 ************
const int MAXQUEUE = 1000000;                         // 采用循环队列，所以可以不用很大
const int MAXN = 110;                                 // 地图的大小
const int DIR_COUNT = 4;                              // 方向数
const int inf = -1;

enum eDirection {
    ED_RIGHT = 0,
    ED_UP = 1,
    ED_LEFT = 2,
    ED_DOWN = 3,
};

const int dir[DIR_COUNT][2] = {
    { 0, 1 },   // 0. 右
    { -1, 0 },  // 1. 上
    { 0, -1 },  // 2. 左
    { 1, 0 },   // 3. 下
};

int getDirIndex(int dx, int dy) {
    for (int i = 0; i < DIR_COUNT; ++i) {
        if (dx == dir[i][0] && dy == dir[i][1]) {
            return i;
        }
    }
    return -1;
}

int optDir[] = {
    2, 3, 0, 1
};

// 广搜的地图，作为全局变量存储
char Map[MAXN][MAXN];

// 广搜时塞入队列的状态结点
struct BFSState {
    int x, y;                              // 1. 一般迷宫问题都会有的位置字段

protected:
    int getStateKey() const;                // 1. 所有关键字打包出一个哈希值进行哈希映射

public:

    inline bool isValidState(int row, int col);    // 1. 格子是否合法，一般是判断 阻碍 和边界
    inline bool isFinalState();                    // 2. 是否结束状态
    inline int getStep() const;                    // 3. 获取从 初始状态 到 当前状态的步数
    inline void setStep(int step);                 // 4. 设置从 初始状态 到 当前状态的步数
    inline void print() const;                     // 5. 调试打印当前状态用
    inline bool operator==(const BFSState & o);    // 6. 用来判断两个状态是否相等

public:
    static int step[MAXN][MAXN];                  //  标记状态对应的步数
};


class BFSQueue {
public:
    BFSQueue();
    virtual ~BFSQueue();

    void clear();
    bool empty();
    void push(const BFSState& bs);
    BFSState& pop();
private:
    BFSState *data_;
    int front_, rear_;
};

BFSQueue::BFSQueue() : data_(NULL) {
    data_ = new BFSState[MAXQUEUE];
}

BFSQueue::~BFSQueue() {
    if (data_) {
        delete[] data_;
        data_ = NULL;
    }
}

void BFSQueue::clear() {
    front_ = rear_ = 0;
}

void BFSQueue::push(const BFSState& bs) {
    data_[rear_++] = bs;
    if (rear_ == MAXQUEUE) rear_ = 0;
}

BFSState& BFSQueue::pop(){
    if (++front_ == MAXQUEUE) front_ = 0;
    if (front_ == 0)
        return data_[MAXQUEUE - 1];
    else
        return data_[front_ - 1];
}

bool BFSQueue::empty() {
    return front_ == rear_;
}

class BFSGraph {
public:

    void bfs_extendstate(const BFSState& fromState);
    void bfs_initialize(BFSState startState);
    int  bfs(BFSState startState);
private:
    BFSQueue queue_;
    int uselen_;
};


int BFSState::getStateKey() const {
    // y        : 11111
    // x        : 11111
    return x << 5 | y;
}

bool BFSState::isValidState(int row, int col) {
    if (x < 0 || y < 0 || x >= row || y >= col) {
        return false;
    }
    if (Map[x][y]) {
        return false;
    }
    return true;
}

bool BFSState::isFinalState() {
    return true;
}

int BFSState::getStep() const {
    return step[x][y];
}

void BFSState::setStep(int sp) {
    step[x][y] = sp;
}

void BFSState::print() const {
    printf("%d %d\n", x, y);
}

int BFSState::step[MAXN][MAXN];
int n, m;

void BFSGraph::bfs_extendstate(const BFSState& fromState) {
    int stp = fromState.getStep() + 1;
    BFSState to;

    for (int i = 0; i < DIR_COUNT; ++i) {
        to.x = fromState.x + dir[i][0];
        to.y = fromState.y + dir[i][1];

        if (!to.isValidState(n, m)) {
            continue;
        }

        if (to.getStep() == inf) {
            to.setStep(stp);
            queue_.push(to);
        }
    }
}

void BFSGraph::bfs_initialize(BFSState startState) {
    memset(BFSState::step, -1, sizeof(BFSState::step));
    queue_.clear();
    queue_.push(startState);
    startState.setStep(0);
}

int BFSGraph::bfs(BFSState startState) {
    bfs_initialize(startState);
    while (!queue_.empty()) {
        BFSState bs = queue_.pop();
        if (bs.isFinalState()) {
            return bs.getStep();
        }
        bfs_extendstate(bs);
    }
    return inf;
}

//************ 迷宫类 广度优先搜索 模板 ************

BFSGraph bfs;
