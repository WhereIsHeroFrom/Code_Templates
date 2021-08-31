int search(int* nums, int numsSize, int target){
    int ans = 0, mid;
    int l = 0, r = numsSize - 1;
    while(l <= r) {
        mid = (l + r) >> 1;
        if(target <= nums[mid]) {
            r = mid - 1;
            ans = mid;
        }else  {
            l = mid + 1;
        }
    }
    return ans;
}
