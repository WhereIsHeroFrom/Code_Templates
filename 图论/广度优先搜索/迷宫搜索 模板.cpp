#include <iostream>
#include <queue>
#include <cstring>
#include <cmath>
using namespace std;

//************ 迷宫类 广度优先搜索 模板 ************
const int MAXQUEUE = 100000;                          // 采用循环队列，所以可以不用很大
const int MAXSTATE = 160100;                          // 最大状态数
const int MAXN = 20;                                  // 地图的大小
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

int opposite[] = {
    2, 3, 0, 1
};

// 广搜的地图，作为全局变量存储
char Map[MAXN + 1][MAXN + 1];
const char MAP_BLOCK = 'X';

struct Pos {
    int x, y;
    Pos() : x(0), y(0){}
    Pos(int _x, int _y) : x(_x), y(_y) {}

    bool isValid(int row, int col) {
        if (x < 0 || y < 0 || x >= row || y >= col) {
            return false;
        }
        if (Map[x][y] == MAP_BLOCK) {
            return false;
        }
        return true;
    }
    Pos move(int dirIndex) const {
        return Pos(x + dir[dirIndex][0], y + dir[dirIndex][1]);
    }
};

// 广搜时塞入队列的状态结点
struct BFSState {
    Pos p[2];                               // 1. 一般迷宫问题都会有的位置字段

protected:
    int getStateKey() const;                // 2. 所有关键字打包出一个哈希值进行哈希映射

public:

    inline bool isValidState(int row, int col);    // 1. 格子是否合法，一般是判断 阻碍 和边界
    inline bool isFinalState();                    // 2. 是否结束状态
    inline int getStep() const;                    // 3. 获取从 初始状态 到 当前状态的步数
    inline void setStep(int step);                 // 4. 设置从 初始状态 到 当前状态的步数
    inline void print() const;                     // 5. 调试打印当前状态用
    inline bool operator==(const BFSState & o);    // 6. 用来判断两个状态是否相等

public:
    static int step[MAXSTATE];                  //  标记状态对应的步数
};

typedef BFSState QueueData;

// 循环队列模板
class Queue {
public:
    Queue();
    virtual ~Queue();

    void clear();
    bool empty() const;
    void push(const QueueData& bs);
    QueueData& pop();
private:
    QueueData *data_;
    int front_, rear_;
};

Queue::Queue() : data_(NULL) {
    data_ = new QueueData[MAXQUEUE];
}

Queue::~Queue() {
    if (data_) {
        delete[] data_;
        data_ = NULL;
    }
}

void Queue::clear() {
    front_ = rear_ = 0;
}

void Queue::push(const QueueData& bs) {
    data_[rear_++] = bs;
    if (rear_ == MAXQUEUE) rear_ = 0;
}

QueueData& Queue::pop(){
    if (++front_ == MAXQUEUE) front_ = 0;
    if (front_ == 0)
        return data_[MAXQUEUE - 1];
    else
        return data_[front_ - 1];
}

bool Queue::empty() const{
    return front_ == rear_;
}

class BFSGraph {
public:

    void bfs_extendstate(const BFSState& fromState);
    void bfs_initialize(BFSState startState);
    int  bfs(BFSState startState);
private:
    Queue queue_;
};

int BFSState::getStep() const {
    return step[getStateKey()];
}

void BFSState::setStep(int sp) {
    step[getStateKey()] = sp;
}

int BFSState::step[MAXSTATE];

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



//************ 迷宫类 广度优先搜索  需要修改的内容 ************
int pos2State[MAXN][MAXN][MAXN][MAXN];
int n, m;

void BFSState::print() const {
}

int BFSState::getStateKey() const {
    return pos2State[p[0].x][p[0].y][p[1].x][p[1].y];
}

bool BFSState::isValidState(int row, int col) {
    return p[0].isValid(row, col) && p[1].isValid(row, col);
}

bool BFSState::isFinalState() {
    return abs(p[0].x - p[1].x) + abs(p[0].y - p[1].y) <= 1;
}

void BFSGraph::bfs_extendstate(const BFSState& fromState) {
    int stp = fromState.getStep() + 1;
    BFSState to;
    for (int i = 0; i < DIR_COUNT; ++i) {
        to.p[0] = fromState.p[0].move(i);
        to.p[1] = fromState.p[1].move(opposite[i]);
        if (to.p[0].isValid(n, m)) {
            if (!to.p[1].isValid(n, m)) {
                to.p[1] = fromState.p[1];
            }
            if (to.getStep() == inf) {
                to.setStep(stp);
                queue_.push(to);
            }
        }
    }
}

//************ 迷宫类 广度优先搜索 需要修改的内容 ************ 



BFSGraph bfs;

int main() {
    int stateId = 0;
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; j++) {
            for (int k = 0; k < 20; ++k) {
                for (int l = 0; l < 20; ++l) {
                    pos2State[i][j][k][l] = stateId++;
                }
            }
        }
    }

    while (scanf("%d %d", &n, &m) != EOF) {
        BFSState start;
        for (int i = 0; i < n; ++i) {
            scanf("%s", Map[i]);
            for (int j = 0; j < m; ++j) {
                if (Map[i][j] == 'Z') {
                    start.p[0].x = i;
                    start.p[0].y = j;
                }
                else if (Map[i][j] == 'S') {
                    start.p[1].x = i;
                    start.p[1].y = j;
                }
            }
        }
        int ans = bfs.bfs(start);
        if (ans == inf) printf("Bad Luck!\n");
        else printf("%d\n", ans);
    }

    return 0;
}