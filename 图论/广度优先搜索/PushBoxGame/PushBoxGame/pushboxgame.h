#include "commondef.h"
#include "hash.h"
#include "path.h"
#include "pushboxstate.h"

// 推箱子游戏
// 限定：
//  1. 地图大小不超过 MAXN X MAXN
//  2. 箱子最多个数 MAXBOX
//  3. 目标位置必须 和 箱子数匹配
//  4. 必须严格有1个"小人"

class PushBoxGame {

public:
    PushBoxGame();
    virtual ~PushBoxGame();
    bool read(const char *filename);
    void output();
    void genId();
    bool search();
    void render();
    
private:
    
    void fakeProgress(const char *tips);
    BlockType convertCharToBlockType(char chigh, char clow);
    bool isBlockValid();
    bool isInBound(int r, int c);
    bool isInWall(int r, int c);
    bool isObstacle(int r, int c);

    void genId_dfs(int r, int c);
    void genId_floodfill();
    void genId_genTerr();
    
    StateType getInitState();
    StateType getFinalBoxState();


    void bfs_initialize();
    void bfs_checkAndExtendState(queue<int>& Q, int preState, PushBoxState& pbs);
    bool bfs();

private:
    // 关卡相关
    int row_, col_;                   // 游戏关卡行和列
    BlockType blocks_[MAXN][MAXN];    // 游戏关卡地图

    // 深搜相关
    int id_[MAXN][MAXN];              // 关卡格子编号，idCount_为上文提到的 K
    int idrow_[MAXN*MAXN];            // id 行反查
    int idcol_[MAXN*MAXN];            // id 列反查

    // 广搜相关
    Path path_;                       // 路径生成器
    Hash hash_;                       // 标记状态的hash表
    int finalState_;                   // 搜索的最终状态
};