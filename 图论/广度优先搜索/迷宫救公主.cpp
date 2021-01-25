#include <iostream>
#include <queue>
#include <cstring>
#include <cmath>
#include <windows.h>
using namespace std;

//************ 迷宫类 广度优先搜索 模板 ************
const int MAXQUEUE = 1000000;                          // 采用循环队列，所以可以不用很大
const int MAXSTATE = 1000100;                          // 最大状态数
const int MAXN = 20;                                   // 地图的大小
const int DIR_COUNT = 4;                               // 方向数
const int inf = -1;

// 广搜的地图，作为全局变量存储
int XMAX, YMAX, ZMAX;
char Map[MAXN + 1][MAXN + 1];
int pos2State[ MAXN ][ MAXN ];
const char MAP_BLOCK = 'X'; 

void initPos2State() {
    int stateId = 0;
    for (int i = 0; i < MAXN; ++i) {
        for (int j = 0; j < MAXN; ++j) {
            pos2State[i][j] = stateId++;
        }
    }
}
/*
 二维
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
};*

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
*/

/*
 三维
*/

const int dir[DIR_COUNT][2] = {
    { 1, 0 },
    { 0, 1 },
    { 0, -1 },
    { -1, 0 }
};




struct Pos {
    int x, y;
    Pos() : x(0), y(0){}
    Pos(int _x, int _y) : x(_x), y(_y) {}
    
    bool isInBound() {
        return !(x < 0 || y < 0 || x >= XMAX || y >= YMAX);
    }
    bool isObstacle() {
        return (Map[x][y] == MAP_BLOCK);
    }

    bool operator == (const Pos & o) const {
        return x == o.x && y == o.y;
    }

    Pos move(int dirIndex) const {
        return Pos(x + dir[dirIndex][0], y + dir[dirIndex][1]);
    }
};

// 广搜时塞入队列的状态结点
struct BFSState {
    Pos p;                                       // 1. 一般迷宫问题都会有的位置字段

protected:
    int getStateKey() const;                      // 2. 所有关键字打包出一个哈希值进行哈希映射
public:
    inline bool isValidState();                    // 1. 格子是否合法，一般是判断 阻碍 和边界
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
    int  bfs(BFSState startState);
private:
    void bfs_extendstate(const BFSState& fromState);
    void bfs_initialize(BFSState startState);
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


void BFSState::print() const {
}

int BFSState::getStateKey() const {
    return pos2State[p.x][p.y];
}

bool BFSState::isValidState() {
    return p.isInBound() && !p.isObstacle();
}

BFSState finalState;
bool BFSState::isFinalState() {
    return p == finalState.p;
}

void output() {
    Sleep(70);
    system("cls");
    for (int i = 0; i < XMAX; ++i) {
        for (int j = 0; j < YMAX; ++j) {
            if (Map[i][j] == '.') {
                BFSState s;
                s.p = Pos(i, j);
                if (s.getStep() == inf) {
                    printf("  ");
                }
                else {
                    printf("♂");
                }
            }
            else {
                if (Map[i][j] == 'X') {
                    printf("■");
                }
                else if (Map[i][j] == 'y') {
                    printf("♂");
                }
                else if (Map[i][j] == 'o') {
                    printf("♀");
                }
            }
        }
        puts("");
    }
}

void BFSGraph::bfs_extendstate(const BFSState& fromState) {
    int stp = fromState.getStep() + 1;
    BFSState toState;
    for (int i = 0; i < DIR_COUNT; ++i) {
        toState.p = fromState.p.move(i);
        if (!toState.isValidState()) {
            continue;
        }
        if (toState.getStep() != inf) {
            continue;
        }
        toState.setStep(stp);
        queue_.push(toState);
        output();
    }
}

//************ 迷宫类 广度优先搜索 需要修改的内容 ************ 



BFSGraph bfs;
char str[100];

int main() {
    initPos2State();
    while (scanf("%d %d", &XMAX, &YMAX) != EOF) {
        BFSState startState;
        for (int x = 0; x < XMAX; ++x) {
            scanf("%s", Map[x]);
            for (int y = 0; y < YMAX; ++y) {
                if (Map[x][y] == 'y') {
                    startState.p = Pos(x, y);
                }
                else if (Map[x][y] == 'o') {
                    finalState.p = Pos(x, y);
                }

            }
        }
        Sleep(2000);
        bfs.bfs(startState);

    }
    return 0;
}

/*
13 11 
XXXXXXXXXXX
X....y....X
X.X.X.X.X.X
XXX.XXX.XXX
X...XoX...X
X.XXX.XXX.X
X...X.X...X
XXX.X.X.XXX
X..XX.X...X
X.....X.X.X
X.XXXXX.X.X
X.......X.X
XXXXXXXXXXX
*/