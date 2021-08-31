
int minIndex(int *nums, int numsSize, int target) {
    int mid, ans = -1;
    int l = 0, r = numsSize-1;
    while(l <= r) {
        mid = (l + r) >> 1;
        if( target <= nums[mid-1] ) {
            r = mid - 1;                  // (1)
            ans = mid;                    // (2)
        }else {
            l = mid + 1;                  // (3)
        }
    }
    if(ans != -1 && nums[ans-1] != target) {
        ans = -1;                         // (4)
    }
    return ans;                           // (5)
}
/*
* $(1)$ 当满足```target >= sum[mid-1]```，满足条件的下标一定在区间 $[mid, r]$，并且 $mid$ 这个位置是一个可行解（最优解待定），所以将左区间缩小为 $mid+1$；
* $(2)$ 记录下可行解 $mid$ 到 $ans$ 中；
* $(3)$ 否则，```target < sum[mid-1]```，满足条件的下标一定在区间 $[l, mid-1]$，将右区间缩小为 $mid-1$；
* $(4)$ 目前找到的位置只是一个```target >= sum[ans-1]```的位置，所以当```target != sum[ans-1]```时，实际上这个位置是无效的，所以需要变回 $-1$；
* $(5)$ 这时候的 $ans$ 为全局最优解，直接返回即可。
*/
