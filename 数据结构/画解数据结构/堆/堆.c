#include <malloc.h>

#define bool int 
#define false 0
#define true 1
 
/**********************************小顶堆模板************************************/
#define lson(idx) (idx << 1|1)
#define rson(idx) ((idx + 1) << 1)
#define parent(idx) ((idx - 1) >> 1)
#define root 0

typedef struct {
    int key;
    void *any; 
}DataType;

// -1 和 1 交换，就变成了大顶堆
int compareData(const DataType* a, const DataType* b) {
    if(a->key < b->key) {
        return -1;
    }else if(a->key > b->key) {
        return 1;
    }
    return 0;
}
void swap(DataType* a, DataType* b) {
    DataType tmp = *a;
    *a = *b;
    *b = tmp;
}

typedef struct {
    DataType *data;
    int size;
    int capacity;
}Heap;

// 内部接口，小写驼峰

// heapShiftDown 这个接口是一个内部接口，所以用小写驼峰区分，用于对堆中元素进行删除的时候的下沉调整；
void heapShiftDown(Heap* heap, int curr) {
    int son = lson(curr);

    while(son < heap->size) {
        if( rson(curr) < heap->size ) {
            if( compareData( &heap->data[rson(curr)], &heap->data[son] ) < 0 ) {
                son = rson(curr);                        // (1) 始终选择值更小的结点
            }        
        }
        if( compareData( &heap->data[son], &heap->data[curr] ) < 0 ) {
            swap(&heap->data[son], &heap->data[curr]);   // (2) 子结点的值小于父结点，则执行交换；
            curr = son;
            son = lson(curr);
        }else {
            break;                                       // (3) 子结点的值大于父结点，说明已经正确归位，下沉操作结束，跳出循环；
        }
    }
}

// heapShiftUp 这个接口是一个内部接口，所以用小写驼峰区分，用于对堆中元素进行插入的时候的上浮调整；
void heapShiftUp(Heap* heap, int curr) {
    int par = parent(curr);
    while(par >= root) {
        if( compareData( &heap->data[curr], &heap->data[par] ) < 0 ) {
            swap(&heap->data[curr], &heap->data[par]);   // (1) 子结点的值小于父结点，则执行交换；
            curr = par;
            par = parent(curr);
        }else {
            break;                                       // (2) 子结点的值大于父结点，说明已经正确归位，上浮操作结束，跳出循环；
        }
    }
}

bool heapIsFull(Heap *heap) {
    return heap->size == heap->capacity;
}

// 外部接口，大写驼峰

// 堆的判空
bool HeapIsEmpty(Heap *heap) {
    return heap->size == 0;
}

// 堆的插入
// 插到最后一个位置，然后不断进行上浮操作
bool HeapPush(Heap* heap, DataType data) {
    if( heapIsFull(heap) ) {
        return false;
    }
    heap->data[ heap->size++ ] = data;
    heapShiftUp(heap, heap->size-1);
    return true;
}


// 堆的删除
// 1、删除堆顶元素时，将堆底部的下标最大的元素放入对顶；
// 2、然后调用 shiftDown 将这个元素进行下沉操作；
// 对于小顶堆来说，从根到叶子的路径必然是单调不降的，所以下沉操作一定会终止在路径的某个点，并且保证所有的堆路径还是能够维持单调不降；
bool HeapPop(Heap *heap) {
    if(HeapIsEmpty(heap)) {
        return false;
    }
    heap->data[root] = heap->data[ --heap->size ];
    heapShiftDown(heap, root);
    return true;
}

DataType HeapTop(Heap *heap) {
    // assert(!HeapIsEmpty(heap));
    return heap->data[root];
}

// 创建堆
Heap* HeapCreate(DataType *data, int dataSize, int maxSize) {
    int i;
    Heap *h = (Heap *)malloc( sizeof(Heap) );
    
    h->data = (DataType *)malloc( sizeof(DataType) * maxSize );
    h->size = 0;
    h->capacity = maxSize;

    for(i = 0; i < dataSize; ++i) {
        HeapPush(h, data[i]);
    }
    return h;
}

// 销毁堆
void HeapFree(Heap *heap) {
    free(heap->data);
    free(heap);
}

/**********************************小顶堆模板************************************/













