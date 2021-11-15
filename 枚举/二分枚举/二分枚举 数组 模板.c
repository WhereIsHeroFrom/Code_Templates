#include <stdio.h>

/************** 二分查找 数组 模板 **************/
/*

  1）传参的数组满足：红红红红红红红红绿绿绿绿绿绿绿； 
  2）返回值：绿色区段的左边界； 
*/

int isGreen(int val, int x);

int binarySearch(int *arr, int arrSize, int x) {
    int l = -1, r = arrSize;
    int mid;
    while(l + 1 < r) {
        mid = l + (r - l) / 2;
        if( isGreen(arr[mid], x) )
            r = mid;
        else
            l = mid;
    }
    return r;
}
/************** 二分查找 数组 模板 **************/

int isGreen(int val, int x) {
    return val >= x;
}

int search(int* nums, int n, int target){
    int r = binarySearch(nums, n, target);
    if(r == n || nums[r] != target)
        return -1;
    return r;
}
