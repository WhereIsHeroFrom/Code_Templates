#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

/***************************Õ» Ä£°å***********************************/
typedef int StackData;
const int MAXSTACK = 200010;

class  Stack {
private:
    StackData *data_;
    int top_;
public:
    Stack();
    virtual ~Stack();

    void clear();
    void push(const StackData& sd);
    StackData& top() const;
    StackData& pretop() const;
    int size() const;
    void pop();

public:
    bool empty() const;
};

Stack::Stack() : data_(NULL) {
    data_ = new StackData[MAXSTACK];
}
Stack::~Stack() {
    if (data_) {
        delete[] data_;
        data_ = NULL;
    }
}

void Stack::clear() {
    top_ = 0;
}

void Stack::push(const StackData& sd) {
    data_[top_++] = sd;
}

StackData& Stack::top() const {
    return data_[top_ - 1];
}

StackData& Stack::pretop() const {
    return data_[top_ - 2];
}

int Stack::size() const {
    return top_;
}

void Stack::pop() {
    if (!empty()) {
        --top_;
    }
}

bool Stack::empty() const {
    return !top_;
}
/***************************Õ» Ä£°å***********************************/
