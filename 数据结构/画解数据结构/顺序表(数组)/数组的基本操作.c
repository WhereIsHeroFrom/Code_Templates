#include <stdio.h>

#define MAXN 1024
#define DataType int

struct SeqList {
    DataType data[MAXN];
    int length;
}; 

DataType SeqListIndex(struct SeqList *sq, int i) {
    return sq->data[i];    
}

DataType SeqListFind(struct SeqList *sq, DataType dt) {
    int i;
    for(i = 0; i < sq->length; ++i) {
        if(sq->data[i] == dt) {
            return i;               // (1)
        }    
    }
    return -1;                      // (2)
}

DataType SeqListGetLength(struct SeqList *sq) {
    return sq->length; 
}

int SeqListInsert(struct SeqList *sq, int k, DataType v) {
    int i;
    if(sq->length == MAXN) {
        return 0;                        // (1) 返回0代表插入失败 
    } 
    for(i = sq->length; i > k; --i) {
        sq->data[i] = sq->data[i-1];     // (2) 从后往前 
    }
    sq->data[k] = v;                     // (3) 第 k 个变成 v 
    sq->length ++;                       // (4) 数组长度加一 
    return 1;                            // (5) 返回1代表插入成功 
}

int SeqListDelete(struct SeqList *sq, int k) {
    int i;
    if(sq->length == 0) {
        return 0;                        // (1) 返回0代表删除失败 
    } 
    for(i = k; i < sq->length - 1; ++i) {
        sq->data[i] = sq->data[i+1];     // (2) 从前往后
    } 
    sq->length --;                       // (3) 数组长度减一 
    return 1;                            // (4) 返回1代表删除成功 
}


int main() {
    int i, j;
    struct SeqList sq;
    sq.length = 0;
     
    for(i = 0; i < 7; ++i) {
        SeqListInsert(&sq, 0, i);
    }
    
    for(j = 0; j < 5; ++j) {
        SeqListDelete(&sq, j);
        for(i = 0; i < 7; ++i) {    
            printf( "%d ", SeqListIndex( &sq, i) );
        }
        puts("");
    }

    
    return 0;
}
