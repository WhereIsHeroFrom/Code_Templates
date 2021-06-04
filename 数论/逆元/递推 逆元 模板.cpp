#include <iostream>
#include <cstdio>

using namespace std;

const int maxn = 3000010;
#define ll long long

int f[maxn];

int main() {
    int n, p;
    while(scanf("%d %d", &n, &p) != EOF) {
        f[1] = 1;
        for(int i = 2; i <= n; ++i) {
            f[i] = - (ll) (p/i) * f[p % i] % p;
            f[i] = (f[i] + p) % p;
        }        
        for(int i = 1; i <= n; ++i) {
            printf("%d\n", f[i]);
        } 
    }
}
