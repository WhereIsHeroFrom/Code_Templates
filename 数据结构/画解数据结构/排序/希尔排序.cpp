#include <stdio.h>
#include <malloc.h>
 
#define maxn 1000001

int a[maxn];

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

void ShellSort(int n, int a[]){
    int i, j, tmp, gap;
    for(gap = n / 2; gap > 0; gap /= 2) {      // (1)   每隔gap个数作为一组，gap = n/2, n/4, n/8, ..., 1
        for(i = gap; i < n; ++i) {             // (2)   从第 gap 个数开始 
            tmp = a[i];
            for(j = i; j >= gap; j -= gap) {   // (3)   和前一个相隔 gap 的数比较 
                if(tmp < a[j - gap]) {         // (4)   如果小于它，则类似插入排序一样，往后挪一个位置 
                    a[j] = a[j - gap];
                }else {
                    break;                     // (5)   否则，跳出循环 
                }
            }
            a[j] = tmp;                        // (6)   第 j  个位置就是这个数需要插入的位置 
        }
    }
}

int main() {
    int n;
    while(scanf("%d", &n) != EOF) {
        Input(n, a);
        ShellSort(n, a);
        Output(n, a);
    }
    return 0;
} 
