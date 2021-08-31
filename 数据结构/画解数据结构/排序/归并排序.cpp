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

void MergeSort(int *nums, int l, int r) {
    int i, mid, p, lp, rp;
    int *tmp = (int *)malloc( (r-l+1) * sizeof(int) );    // (1) 申请一个辅助数组，用于对原数组进行归并计算  
    if(l >= r) {
        return ;                                          // (2) 只有一个元素，或者没有元素的情况，则不需要排序
    }
    mid = (l + r) >> 1;                                   // (3) 将数组分为 [l, mid] 和 [mid+1, r] 两部分
    MergeSort(nums, l, mid);                              // (4) 递归排序 [l, mid] 部分
    MergeSort(nums, mid+1, r);                            // (5) 递归排序 [mid+1, r] 部分
    p = 0;                                                // (6) 将需要排序的数组缓存到 tmp 中，用 p 作为游标
    lp = l, rp = mid+1;                                   // (7) 初始化两个数组的指针
    while(lp <= mid || rp <= r) {                         // (8) 当两个指针都没有到结尾，则继续迭代
        if(lp > mid) {
            tmp[p++] = nums[rp++];                        // (9) 只剩下右边的数组，直接排
        }else if(rp > r) {
            tmp[p++] = nums[lp++];                        // (10) 只剩下走右边的数组，直接排
        }else {
            if(nums[lp] <= nums[rp]) {                    // (11) 取小的那个先进 tmp 数组
                tmp[p++] = nums[lp++];
            }else {
                tmp[p++] = nums[rp++];
            }
        }
    }
    for(i = 0; i < r-l+1; ++i) {
        nums[l+i] = tmp[i];                              // (12) 别忘了将排序好的数据拷贝回原数组
    } 
    free(tmp);                                           // (13) 别忘了释放临时数据，否则就内存泄漏了！！！
}

int main() {
    int n;
    while(scanf("%d", &n) != EOF) {
        Input(n, a);
        MergeSort(a, 0, n-1);
        Output(n, a);
    }
    return 0;
} 
