/*
1. 初始化 计数器数组 cnt 
2. 遍历数列中的每个元素，相应数 的 计数器 增加1
3. 每轮计数，都从最小的值开始
    当计数为非零数时循环
    重新将元素存储于列表
    将计数减1
*/
#include <stdio.h>
#include <string.h>

#define maxn 1000001
#define maxk 10001

int a[maxn];
int cnt[maxk]; 

void Input(int n, int *a) {
    for(int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
}

void Output(int n, int *a) {
    for(int i = 0; i < n; ++i) {
        if(i)
            printf(" ");
        printf("%d", a[i]);
    }
    puts("");
}

void CountingSort(int n, int *a) {       // (1)
    int i, top; 
    memset(cnt, 0, sizeof(cnt));         // (2) 
    for(i = 0; i < n; ++i) {             // (3)
        ++cnt[ a[i] ];                   // (4)
    }
    top = 0;                             // (5)
    for(i = 0; i < maxk; ++i) {
        while(cnt[i]) {                  // (6)
            a[top++] = i;                // (7)
            --cnt[i];                    // (8)
        }
    }
    // top == n;
} 

int main() {
    int n;
    while(scanf("%d", &n) != EOF) {
        Input(n, a);
        CountingSort(n, a);
        Output(n, a);
    }
    return 0;
} 
