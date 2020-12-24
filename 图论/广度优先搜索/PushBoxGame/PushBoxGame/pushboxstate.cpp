#include "pushboxstate.h"



int PushBoxState::base_ = 0;

void PushBoxState::setBase(int b)
{
    PushBoxState::base_ = b;
}

int PushBoxState::getBase()
{
    return base_;
}

PushBoxState::PushBoxState()
{

}

PushBoxState::~PushBoxState()
{

}

// 根据传入位置，生成状态
StateType PushBoxState::Serialize(int man, int boxcnt, int box[MAXBOX])
{
    setManCode(man);
    boxcnt_ = boxcnt;
    for (int i = 0; i < boxcnt_; ++i) {
        box_[i] = box[i];
    }
    calcState(true);
    return state_;
}

// 根据状态，生成位置
void PushBoxState::DeSerialize(StateType state)
{
    state_ = state;
    boxstate_ = state_ / getBase();

    calcManCode();
    calcBoxCode();
}


StateType PushBoxState::getBoxState()
{
    return boxstate_;
}

StateType PushBoxState::getState()
{
    return state_;
}

void PushBoxState::setManCode(int val)
{
    man_ = val;
    calcState(false);
}

int PushBoxState::getManCode()
{
    return man_;
}

void PushBoxState::setBoxCode(int idx, int val)
{
    box_[idx] = val;
    calcState(true);
}

int PushBoxState::getBoxCode(int idx)
{
    return box_[idx];
}

int PushBoxState::getBoxCount()
{
    return boxcnt_;
}

int PushBoxState::getMatchBoxIndex(int id)
{
    // 如果 id 在其中一个箱子上，则返回具体下标，否则返回 -1
    for (int i = 0; i < boxcnt_; ++i) {
        if (id == box_[i]) {
            return i;
        }
    }
    return -1;
}

// 主要通过 box_[] 和 man_
// 计算出 boxstate_ 和 state_
void PushBoxState::calcState(bool bReCalcBox)
{
    if (bReCalcBox)
    {
        // 因为涉及到排序，所以只能拷贝一份出来，否则对于数组来说会修改传参的值
        int boxcopy[MAXBOX];
        for (int i = 0; i < boxcnt_; ++i) {
            boxcopy[i] = box_[i];
        }

        boxstate_ = 0;
        // 最小化编码
        sort(boxcopy, boxcopy + boxcnt_);
        for (int i = 0; i < boxcnt_; ++i) {
            boxstate_ = boxstate_ * (StateType)getBase() + (StateType)boxcopy[i];
        }
    }
    state_ = boxstate_ * (StateType)getBase() + man_;
}

// 计算 man 在 state 中对应位的值
void PushBoxState::calcManCode()
{
    man_ = state_ % getBase();
}

// 计算 box[] 在 state 中对应位的值
void PushBoxState::calcBoxCode()
{
    boxcnt_ = 0;
    int bs = boxstate_;
    while (bs) {
        box_[boxcnt_++] = bs % getBase();
        bs /= getBase();
    }
}
