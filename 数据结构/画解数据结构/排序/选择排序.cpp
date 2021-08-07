
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

void SelectionSort(int n, int *a) {  // (1)
    int i, j;
    for(i = 0; i < n - 1; ++i) {     // (2)
        int min = i;                 // (3)
        for(j = i+1; j < n; ++j) {   // (4)
            if(a[j] < a[min]) {
                min = j;             // (5)
            }
        }
        swap(&a[i], &a[min]);        // (6) 
    }
}

int main() {
    int n;
    while(scanf("%d", &n) != EOF) {
        input(n, a);
        SelectionSort(n, a);
        output(n, a);
    }
    return 0;
} 
