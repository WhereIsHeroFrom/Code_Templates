#include <iostream>

using namespace std;

const int MAXN = 3;
int m[MAXN] = {4, 6, 15};
int a[MAXN] = {2, 4, 7};

int main() {
    for(int x = 0; x < 400; ++x) {
        bool bfind = true;
        for(int i = 0; i < MAXN; ++i) {
            if( x % m[i] != a[i] ) {
                bfind = false;
                break;
            }
        }
        if(bfind) {
            printf("%d\n", x);
        }
    }	
    return 0;
} 
