/**************************** 顺序表 实现双端队列 ****************************/
#define DataType int
#define maxn 100005

struct Queue {
    DataType data[maxn];
    int head, tail;
};

void QueueClear(struct Queue* que) {
    que->head = que->tail = 0;
}
void QueueEnqueue(struct Queue *que, DataType dt) {
    que->data[ que->tail++ ] = dt;
}
void QueueDequeueFront(struct Queue* que) {
    ++que->head;
}
void QueueDequeueRear(struct Queue* que) {
    --que->tail;
}

DataType QueueGetFront(struct Queue* que) {
    return que->data[ que->head ];
}
DataType QueueGetRear(struct Queue* que) {
    return que->data[ que->tail - 1 ];
}
int QueueGetSize(struct Queue* que) {
    return que->tail - que->head;
}
int QueueIsEmpty(struct Queue* que) {
    return !QueueGetSize(que);
}

/**************************** 顺序表 实现双端队列 ****************************/

