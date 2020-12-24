#ifndef PUSHBOXSTATE_H
#define PUSHBOXSTATE_H

#include "commondef.h"

// 注意：状态编码的时候，为了让解码的时候能够准确解出有多少个箱子，编码位 为不设置 0
class PushBoxState {
public:
    static void setBase(int b);
    static int getBase();

    PushBoxState();
    virtual ~PushBoxState();

    // 根据传入的 人和箱子位置，生成一个整数
    StateType Serialize(int man, int boxcnt, int box[MAXBOX]);

    // 根据传入的整数，反推出 人和箱子位置
    void DeSerialize(StateType state);

    // 对私有成员访问的封装
    StateType getBoxState();
    StateType getState();
    void setManCode(int val);
    int getManCode();
    void setBoxCode(int idx, int val);
    int getBoxCode(int idx);
    int getBoxCount();

    // 获取是否有一个箱子在id上
    // 有的话，返回箱子下标，否则返回 -1
    int getMatchBoxIndex(int id);

private:
    void calcState(bool bReCalcBox);
    void calcManCode();
    void calcBoxCode();


private:
    int man_;
    int boxcnt_;
    int box_[MAXBOX];

    StateType boxstate_;
    StateType state_;

    static int base_;
};

#endif // !PUSHBOXSTATE_H
