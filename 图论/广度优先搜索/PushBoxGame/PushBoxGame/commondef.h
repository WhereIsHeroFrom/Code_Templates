#ifndef COMMON_DEF
#define COMMON_DEF

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// 静态哈希数组大小
const int MAXH = ((1 << 25) - 1);
#define StateType long long 

// 地图大小
const int MAXN = 13;

// 最大箱子个数
const int MAXBOX = 6;

// 四方向增量
const int dir[4][2] = {
    { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 }
};


enum BlockType {
    BT_EMPTY = 0,    // 空地
    BT_WALL = 1,     // 障碍物、墙
    BT_BOX = 2,      // 箱子
    BT_TARGET = 3,   // 目标位置
    BT_MAN = 4,      // 推箱子的人
    BT_MAX
};

// 这些都是宽字符，占用2个字节，加上字符串末尾 '\0'，总共3个字节
const char BlockSign[BT_MAX][3] = {
    "□", "■", "●", "◎", "♂"
};

enum VisitType {
    VT_UNVISITED = -1,
    VT_VISITED = 0,
};

#endif // !COMMON_DEF