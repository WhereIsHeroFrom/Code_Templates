#include <stdio.h>

int isGreen(int val, int x) {
    return val > x;
} 

int findFirstBiggerThan(int *arr, int arrSize, int x) {
    int i;
    for(i = 0; i < arrSize; ++i) {
        if( isGreen(arr[i], x) ) {
            return i;
        }
    }
    return arrSize;
}

int findFirstBiggerEqualThan(int *arr, int arrSize, int x) {
    int i;
    for(i = 0; i < arrSize; ++i) {
        if(arr[i] >= x) {
            return i;
        }
    }
    return arrSize;
}

int isGreenX(int val, int x) {
    return val >= x;
} 

int findLastSmallThan(int *arr, int arrSize, int x) {
    int i;
    for(i = 0; i < arrSize; ++i) {
        if( isGreenX(arr[i], x) ) {
            return i - 1;
        }
    }
    return -1;
}

int findLastSmallEqualThan(int *arr, int arrSize, int x) {
    int i;
    for(i = arrSize-1; i >= 0; --i) {
        if(arr[i] <= x) {
            return i;
        }
    }
    return -1;
}



int main() {
    int arrSize = 9;
    int arr[] = {1, 3, 4, 6, 6, 6, 7, 8, 9};
    
    int f1 = findFirstBiggerThan(arr, arrSize, 6);
    int f2 = findFirstBiggerEqualThan(arr, arrSize, 6);
    int f3 = findLastSmallThan(arr, arrSize, 6);
    int f4 = findLastSmallEqualThan(arr, arrSize, 6);
    
    printf("%d %d %d %d\n", f1, f2, f3, f4); 
    
    return 0;
} 
