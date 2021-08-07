#include <stdio.h>

int a[1010];

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

void Swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void BubbleSort(int n, int *a) {             // (1)
    bool swapped;
    int last = n;
    do {
        swapped = false;                     // (2)
        for(int i = 0; i < last - 1; ++i) {  // (3)
            if(a[i] > a[i+1]) {              // (4)
                Swap(&a[i], &a[i+1]);        // (5)
                swapped = true;              // (6)
            }
        }
        --last;
    }while (swapped);
} 

int main() {
    int n;
    while(scanf("%d", &n) != EOF) {
        Input(n, a);
        BubbleSort(n, a);
        Output(n, a);
    }
    return 0;
} 
