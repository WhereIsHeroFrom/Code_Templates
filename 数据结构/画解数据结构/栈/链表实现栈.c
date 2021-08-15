#include <malloc.h>

/************************************* 栈的链表实现 *************************************/
typedef int DataType;

struct StackNode;
 
struct StackNode {
    DataType data;
    struct StackNode *next;
};

struct Stack {
    struct StackNode *top;
    int size;
};

void StackPushStack(struct Stack *stk, DataType dt) {
    struct StackNode *insertNode = (struct StackNode *) malloc( sizeof(struct StackNode) );
    insertNode->next = stk->top;
    insertNode->data = dt;
    stk->top = insertNode;
    ++ stk->size;
}
void StackPopStack(struct Stack* stk) {
    struct StackNode *temp = stk->top;
    stk->top = temp->next;
    --stk->size; 
    free(temp);
}

DataType StackGetTop(struct Stack* stk) {
    return stk->top->data;
}
int StackGetSize(struct Stack* stk) {
    return stk->size;
}

int StackIsEmpty(struct Stack* stk) {
    return !StackGetSize(stk);
}

void StackClear(struct Stack* stk) {
    while(!StackIsEmpty(stk)) {
        StackPopStack(stk);
    }
    stk->top = NULL; 
    stk->size = 0;
}
/************************************* 栈的链表实现 *************************************/

int main() {
    
    return 0;
}
