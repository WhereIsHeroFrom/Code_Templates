/************************************* 栈的顺序表实现 *************************************/
#define DataType int
#define maxn 100010

struct Stack {
    DataType data[maxn];
    int top;
};

void StackClear(struct Stack* stk) {
    stk->top = 0;
}
void StackPushStack(struct Stack *stk, DataType dt) {
    stk->data[ stk->top++ ] = dt;
}
void StackPopStack(struct Stack* stk) {
    --stk->top;
}
DataType StackGetTop(struct Stack* stk) {
    return stk->data[ stk->top - 1 ];
}
int StackGetSize(struct Stack* stk) {
    return stk->top;
}
int StackIsEmpty(struct Stack* stk) {
    return !StackGetSize(stk);
}
/************************************* 栈的顺序表实现 *************************************/
