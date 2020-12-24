#include <iostream>
#include <cstring>
#include <algorithm>
#include <windows.h>

#include "pushboxstate.h"
#include "pushboxgame.h"

PushBoxGame::PushBoxGame() : row_(0), col_(0) {
}

PushBoxGame::~PushBoxGame() {
}

bool PushBoxGame::read(const char *filename) {
    char strtmp[1000];
    //printf("请输入游戏尺寸（行和列）:\n");

    freopen(filename, "r", stdin);

    scanf("%d %d", &row_, &col_);
    getchar();
    //printf("请输入 (%d行 X %d列) 的地图:\n", row_, col_);
    //printf("□空地, ■墙, ●箱子, ◎目标位置, ♂人\n");

    memset(blocks_, 0, sizeof(blocks_));
    for (int i = 0; i < row_; ++i) {
        gets(strtmp);
        if (strlen(strtmp) != 2 * col_) {
            puts(strtmp);
            break;
        }
        for (int j = 0; j < col_; ++j) {
            blocks_[i][j] = convertCharToBlockType(strtmp[2 * j], strtmp[2 * j + 1]);
        }
    }

    fclose(stdin);
    freopen("CON", "r", stdin);

    if (!isBlockValid()) {
        printf("输入数据存在隐患，请重新输入，保证如下条件满足：\n");
        printf("    1. 地图大小最大 %d X %d\n", MAXN, MAXN);
        printf("    2. 箱子最多个数 %d\n", MAXBOX);
        printf("    3. 目标位置必须 和 箱子数匹配\n");
        printf("    4. 必须严格有一个 '小人'\n");
        puts("");
        return false;
    }
    return true;
}

bool PushBoxGame::search() {
    fakeProgress("路径计算中");
    return bfs();
}

void PushBoxGame::output() {
    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < col_; ++j) {
            printf("%s", BlockSign[blocks_[i][j]]);
        }
        puts("");
    }
}


void PushBoxGame::fakeProgress(const char *tips)
{
    printf("%s", tips);
    for (int i = 0; i < 7; ++i) {
        Sleep(150);
        printf(".");
    }
    puts("");
}

BlockType PushBoxGame::convertCharToBlockType(char chigh, char clow) {
    BlockType tp = BlockType::BT_EMPTY;
    for (int i = BlockType::BT_EMPTY; i < BlockType::BT_MAX; ++i) {
        if (BlockSign[i][0] == chigh && BlockSign[i][1] == clow) {
            tp = (BlockType)i;
            break;
        }
    }
    return tp;
}

bool PushBoxGame::isBlockValid() {
    // 1 地图大小最大 MAXN X MAXN
    if (row_ > MAXN || col_ > MAXN) {
        return false;
    }

    int blockCnt[BT_MAX];
    memset(blockCnt, 0, sizeof(blockCnt));

    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < col_; ++j) {
            ++blockCnt[blocks_[i][j]];
        }
    }
    // 2 箱子最多个数 MAXBOX
    if (blockCnt[BlockType::BT_BOX] > MAXBOX) {
        return false;
    }

    // 3 目标位置必须 和 箱子数匹配
    if (blockCnt[BlockType::BT_TARGET] != blockCnt[BlockType::BT_BOX]) {
        return false;
    }

    // 4 必须严格有一个 '小人'
    if (blockCnt[BlockType::BT_MAN] != 1) {
        return false;
    }

    return true;
}


bool PushBoxGame::isInBound(int r, int c) {
    // 越界检测
    return r >= 0 && r < row_ && c >= 0 && c < col_;
}

bool PushBoxGame::isInWall(int r, int c) {
    // 墙体障碍检测
    return blocks_[r][c] == BT_WALL;
}

// 综合障碍检测
bool PushBoxGame::isObstacle(int r, int c) {
    return !isInBound(r, c) || isInWall(r, c);
}

void PushBoxGame::genId_dfs(int r, int c) {
    // 1. 障碍检测
    if (isObstacle(r, c)) {
        return;
    }
    // 2. 重复访问检测
    if (id_[r][c] == VisitType::VT_VISITED) {
        return;
    }
    // 3. 标记当前格子已访问
    id_[r][c] = VisitType::VT_VISITED;

    // 4. 递归处理相邻四个格子
    for (int i = 0; i < 4; ++i) {
        genId_dfs(r + dir[i][0], c + dir[i][1]);
    }
}

void PushBoxGame::genId_floodfill() {
    // 1.初始化所有格子为未访问
    memset(id_, VisitType::VT_UNVISITED, sizeof(id_));
    // 2.找到 BT_MAN 的格子进行深搜，标记访问到的格子
    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < col_; ++j) {
            if (blocks_[i][j] == BT_MAN) {
                genId_dfs(i, j);
            }
        }
    }
}

void PushBoxGame::genId_genTerr() {
    fakeProgress("地形数据生成中");
    // 1. 按照从左往右，从上往下的顺序标记所有已访问格子
    int idCount = 1;
    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < col_; ++j) {
            if (id_[i][j] == VisitType::VT_VISITED) {
                // 添加 id 正向映射
                id_[i][j] = idCount++;

                // 添加 id 反向映射
                idrow_[id_[i][j]] = i;
                idcol_[id_[i][j]] = j;
            }
        }
    }
    PushBoxState::setBase(idCount);
}

void PushBoxGame::genId() {
    genId_floodfill();
    genId_genTerr();
}

StateType PushBoxGame::getInitState() {
    int manValue, boxCnt = 0, boxValue[MAXBOX];
    PushBoxState pbs;

    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < col_; ++j) {
            // 1. 找出人的位置
            if (blocks_[i][j] == BlockType::BT_MAN) {
                manValue = id_[i][j];
            }
            // 2. 找出所有箱子的位置
            else if (blocks_[i][j] == BlockType::BT_BOX) {
                boxValue[boxCnt++] = id_[i][j];
            }
        }
    }
    // 3. 组合出箱子和人的初始状态
    return pbs.Serialize(manValue, boxCnt, boxValue);
}

StateType PushBoxGame::getFinalBoxState() {
    int manValue, boxCnt = 0, boxValue[MAXBOX];
    PushBoxState pbs;

    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < col_; ++j) {
            // 1. 找出箱子的最终归位位置
            if (blocks_[i][j] == BlockType::BT_TARGET) {
                boxValue[boxCnt++] = id_[i][j];
            }
        }
    }
    // 2. 组合出箱子的最终状态
    pbs.Serialize(0, boxCnt, boxValue);
    return pbs.getBoxState();
}

void PushBoxGame::bfs_initialize() {
    path_.initialize();
    hash_.initialize();
}



void PushBoxGame::bfs_checkAndExtendState(queue<int>& Q, int preState, PushBoxState& pbs) {
    StateType state = pbs.getState();
    if (hash_.hasKey(state)) {
        return;
    }
    // 1. 哈希标记
    int nowState = hash_.getKey(state);
    // 2. 加入前驱链表方便路径回溯
    path_.add(nowState, preState);
    // 3. 加入队列，进行状态扩展
    Q.push(nowState);
}

bool PushBoxGame::bfs() {
    queue <int> Q;
    PushBoxState pbs;

    bfs_initialize();
    // 提前计算出终止状态
    StateType finalBoxState = getFinalBoxState();
    
    // 将初始状态压入队列
    int startState = hash_.getKey(getInitState());
    Q.push(startState);

    while (!Q.empty()) {
        int nowState = Q.front();
        Q.pop();

        // 将编码后的数据 反序列化 到 pbs
        pbs.DeSerialize(hash_.getValue(nowState));

        // 找到解，将最终状态持久化
        if (pbs.getBoxState() == finalBoxState) {
            finalState_ = nowState;
            return true;
        }

        // 人往四个方向走一格，对于每个方向，都有3种情况：
        // 1. 前面没路，这种情况无法扩展状态；
        // 2. 前面没有箱子，直接往前走，扩展状态；
        // 3. 前面有个箱子，又分两种情况：
        //   3.1 箱子前面无法放置，这种情况无法扩展状态；
        //   3.2 箱子前面可以放置，人和箱子同时往这个方向前进一格，扩展状态;
        
        int man = pbs.getManCode();

        for (int i = 0; i < 4; ++i) {
            
            int manr = idrow_[man] + dir[i][0];
            int manc = idcol_[man] + dir[i][1];

            if (isObstacle(manr, manc)) {
                // 情况1
                continue;
            }

            int nextman = id_[manr][manc];

            // 模拟人走到了这个位置
            pbs.setManCode(nextman);

            int boxIndex = pbs.getMatchBoxIndex(nextman);
            if (boxIndex == -1) {
                // 情况2
                bfs_checkAndExtendState(Q, nowState, pbs);
            }
            else {
                // 情况3 箱子必须往前推进一格
                int boxr = idrow_[nextman] + dir[i][0];
                int boxc = idcol_[nextman] + dir[i][1];

                if (isObstacle(boxr, boxc) || pbs.getMatchBoxIndex(id_[boxr][boxc]) != -1) {
                    // 情况3.1
                    continue;
                }
                // 情况3.2
                // 模拟箱子往前走了一格
                pbs.setBoxCode(boxIndex, id_[boxr][boxc]);
                bfs_checkAndExtendState(Q, nowState, pbs);
                // 回退箱子
                pbs.setBoxCode(boxIndex, nextman);
            }
        }
    }
    return false;
}


void PushBoxGame::render() {
    // 1. 根据最终状态进行路径回溯
    vector <int> path;
    path_.getPath(finalState_, path);

    // 2.因为人和箱子的状态存储在状态上，所以原图上的这两个元素可以清空
    for (int i = 0; i < row_; ++i) {
        for (int j = 0; j < col_; ++j) {
            if (blocks_[i][j] == BlockType::BT_BOX || blocks_[i][j] == BlockType::BT_MAN) {
                blocks_[i][j] = BlockType::BT_EMPTY;
            }
        }
    }
    PushBoxState pbs;
    BlockType blockCache[MAXN][MAXN];

    for (int i = 0; i < path.size(); ++i) {
        Sleep(120);
        system("cls");
        pbs.DeSerialize(hash_.getValue(path[i]));

        // 3.将当前状态 人 和 箱子 的状态缓存下来
        memcpy(blockCache, blocks_, MAXN * MAXN * sizeof(BlockType));

        // 4.将当前状态 人 和 箱子 放置上去
        blocks_[idrow_[pbs.getManCode()]][idcol_[pbs.getManCode()]] = BlockType::BT_MAN;
        for (int j = 0; j < pbs.getBoxCount(); ++j) {
            int b = pbs.getBoxCode(j);
            blocks_[idrow_[b]][idcol_[b]] = BlockType::BT_BOX;
        }
        output();

        // 5.恢复成 人 和 箱子 放置上去 之前的情况
        memcpy(blocks_, blockCache, MAXN * MAXN * sizeof(BlockType));

        if (i == 0) {
            // 第一次多停留500秒吧
            Sleep(500);
        }
    }
}
