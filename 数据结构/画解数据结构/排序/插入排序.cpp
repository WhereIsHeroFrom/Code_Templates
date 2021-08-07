#include <stdio.h>

int a[1010];

void input(int n, int *a) {
    for(int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
}

void output(int n, int *a) {
    for(int i = 0; i < n; ++i) {
        if(i)
            printf(" ");
        printf("%d", a[i]);
    }
    puts("");
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void InsertSort(int n, int *a) {       // (1)
    int i, j; 
    for(i = 1; i < n; ++i) {
        int x = a[i];                  // (2)
        for(j = i-1; j >= 0; --j) {    // (3)
            if(a[j] >= x) {            // (4)
                a[j+1] = a[j];         // (5)
            }else
                break;                 // (6)
        }
        a[j+1] = x;                    // (7)
    }
} 

int main() {
    int n;
    while(scanf("%d", &n) != EOF) {
        input(n, a);
        InsertSort(n, a);
        output(n, a);
    }
    return 0;
} 
