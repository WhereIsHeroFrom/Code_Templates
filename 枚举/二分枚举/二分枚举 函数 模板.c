#include <stdio.h>

/************** 二分查找 值类型 模板 **************/
/*

  1）传参的区间满足：红红红红红红红红绿绿绿绿绿绿绿； 
  2）返回值：绿色区段的左边界； 
*/

int isGreen(int val, int x);

int binarySearch(int l, int r, int x) {
    int mid;
    while(l + 1 < r) {
        mid = l + (r - l) / 2;
        if( isGreen(mid, x) )
            r = mid;
        else
            l = mid;
    }
    return r;
}
/************** 二分查找 值类型 模板 **************/

int isGreen(int val, int x) {
    return val >= x;
}

int search(int* nums, int n, int target){
    int r = binarySearch(1, n, target);
    if(r == n || nums[r] != target)
        return -1;
    return r;
}
