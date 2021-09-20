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
    struct QueueNode *vtx = (struct QueueNode *) malloc( sizeof(struct QueueNode));
    vtx->data = dt;
    vtx->next = vtx->prev = NULL;
    return vtx;
}

void _QueueEnqueue(struct Queue *que, DataType dt, int isFrontOrRear) {
    struct QueueNode *vtx = QueueCreateNode(dt);
    if(que->size == 0) {
        que->head = que->tail = vtx;
    }else {
        if(isFrontOrRear) {
            vtx->next = que->head;
            que->head->prev = vtx;
            que->head = vtx;
        }else {
            que->tail->next = vtx;
            vtx->prev = que->tail;
            que->tail = vtx;
        }
    } 
    ++que->size;
}

void _QueueDequeue(struct Queue *que, struct QueueNode *temp, int isFrontOrRear) {
    if(que->size == 1) {
        que->head = que->tail = NULL;
    }else {
        if(isFrontOrRear) {
            que->head = temp->next;
            que->head->prev = NULL;
        }else {
            que->tail = temp->prev;
            que->tail->next = NULL;
        }        
    }
    free(temp);
    --que->size;
}

void QueueClear(struct Queue* que) {
    que->head = que->tail = NULL;
    que->size = 0;
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
