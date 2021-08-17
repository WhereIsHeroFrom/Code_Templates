#include <malloc.h>

/**************************** 链表 实现队列 ****************************/
typedef int DataType;

struct QueueNode;
struct QueueNode {
    DataType data;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *head, *tail;
    int size;
};

void QueueEnqueue(struct Queue *que, DataType dt) {
    struct QueueNode *insertNode = (struct QueueNode *) malloc( sizeof(struct QueueNode) );
    insertNode->data = dt;
    insertNode->next = NULL;
    if(que->tail) {
        que->tail->next = insertNode;
        que->tail = insertNode;
    }else {
        que->head = que->tail = insertNode;
    }
    ++que->size;
}

void QueueDequeue(struct Queue* que) {
    struct QueueNode *temp = que->head;
    que->head = temp->next;
    free(temp);
    --que->size;
    if(que->size == 0) {
        que->tail = NULL;
    } 
}

DataType QueueGetFront(struct Queue* que) {
    return que->head->data;
}
int QueueGetSize(struct Queue* que) {
    return que->size;
}
int QueueIsEmpty(struct Queue* que) {
    return !QueueGetSize(que);
}
void QueueClear(struct Queue* que) {
    que->head = que->tail = NULL;
    que->size = 0;
}

/**************************** 链表 实现队列 ****************************/
