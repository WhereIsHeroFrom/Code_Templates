/**************************** 顺序表 实现队列 ****************************/
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
void QueueDequeue(struct Queue* que) {
    ++que->head;
}

DataType QueueGetFront(struct Queue* que) {
    return que->data[ que->head ];
}
int QueueGetSize(struct Queue* que) {
    return que->tail - que->head;
}
int QueueIsEmpty(struct Queue* que) {
    return !QueueGetSize(que);
}

/**************************** 顺序表 实现队列 ****************************/
