
#include <stdio.h>
#include <string.h>

const int MAXN = 100005;          // (1) 排序数组的元素最大个数 
const int MAXT = 10;              // (2) 排序元素的数字的最大位数 
const int BASE = 10;              // (3) 排序元素的进制，这里为 十进制 
int PowOfBase[MAXT];              // (4) BASE 的 i 次幂 
int RadixBucket[BASE][MAXN];      // (5) 第 i 个基数槽
int RadixBucketTop[BASE];         // (6) 第 i 个基数槽的顶部 


void InitPowOfBase() {
    int i;
    PowOfBase[0] = 1;
    for(i = 1; i < MAXT; ++i) {
        PowOfBase[i] = PowOfBase[i-1] * BASE;
    }
}

void Input(int n, int *a) {
    int i;
    for(i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
}

void Output(int n, int *a) {
    int i; 
    for(i = 0; i < n; ++i) {
        if(i)
            printf(" ");
        printf("%d", a[i]);
    }
    puts("");
}

int getRadix(int value, int pos) {
    return value / PowOfBase[pos] % BASE; 
}

void RadixSort(int n, int *a) {   
    int i, j, top = 0, pos = 0;
    while (pos < MAXT) {
        memset(RadixBucketTop, 0, sizeof(RadixBucketTop));
        for(i = 0; i < n; ++i) {
            int rdx = getRadix(a[i], pos);
            RadixBucket[ rdx ][ RadixBucketTop[rdx]++ ] = a[i];
        }
        top = 0;
        for(i = 0; i < BASE; ++i) {
            for(j = 0; j < RadixBucketTop[i]; ++j) {
                a[top++] = RadixBucket[i][j];
            }
        }
        ++pos; 
    }
}

int a[MAXN];

int main() {
    int n;
    InitPowOfBase();
    while(scanf("%d", &n) != EOF) {
        Input(n, a);
        RadixSort(n, a);
        Output(n, a);
    }
    return 0;
} 
/*
15
3221 1 10 9680 577 9420 7 5622 4793 2030 3138 82 2599 743 4127
*/
