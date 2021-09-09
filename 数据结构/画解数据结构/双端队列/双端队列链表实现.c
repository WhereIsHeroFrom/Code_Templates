#include <malloc.h>


/**************************** 链表 实现双端队列 ****************************/
#define DataType int
struct QueueNode;

struct QueueNode {
    DataType data;
    struct QueueNode *prev;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *head, *tail;
    int size;
};

struct QueueNode *QueueCreateNode(DataType dt) {
    struct QueueNode *p = (struct QueueNode *) malloc( sizeof(struct QueueNode));
    p->data = dt;
    p->next = p->prev = NULL;
    return p;
}

void _QueueEnqueue(struct Queue *que, DataType dt, int isFrontOrRear) {
    struct QueueNode *p = QueueCreateNode(dt);
    if(que->size == 0) {
        que->head = que->tail = p;
    }else {
        if(isFrontOrRear) {
            p->next = que->head;
            que->head->prev = p;
            que->head = p;
        }else {
            que->tail->next = p;
            p->prev = que->tail;
            que->tail = p;
        }
    } 
    ++que->size;
}

void _QueueDequeue(struct Queue *que, struct QueueNode *node, int isFrontOrRear) {
    if(isFrontOrRear) {
        que->head = node->next;
        que->head->prev = NULL;
    }else {
        que->tail = node->prev;
        que->tail->next = NULL;
    }
    free(node);
    --que->size;
}

void QueueClear(struct Queue* que) {
    que->head = que->tail = NULL;
}

void QueueEnqueueFront(struct Queue *que, DataType dt) {
    _QueueEnqueue(que, dt, 1);
}
void QueueEnqueueRear(struct Queue *que, DataType dt) {
    _QueueEnqueue(que, dt, 0);
}
void QueueDequeueFront(struct Queue* que) {
    _QueueDequeue(que, que->head, 1);
}
void QueueDequeueRear(struct Queue* que) {
    _QueueDequeue(que, que->tail, 0);
}

DataType QueueGetFront(struct Queue* que) {
    return que->head->data;
}
DataType QueueGetRear(struct Queue* que) {
    return que->tail->data;
}
int QueueGetSize(struct Queue* que) {
    return que->size;
}
int QueueIsEmpty(struct Queue* que) {
    return !QueueGetSize(que);
}

/**************************** 链表 实现双端队列 ****************************/
