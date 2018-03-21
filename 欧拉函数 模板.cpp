/*
欧拉函数
    n分解素因子表示为n = p1^e1 * p2^e2 * ... * pk^ek
    则n的欧拉函数为 f(n) = (p1-1)p1^(e1-1) * (p2-1)p2^(e2-1) * ... * (pk-1)pk^(ek-1)
Author: WhereIsHeroFrom
Update Time: 2018-3-21
Algorithm Complexity: O(sqrt(n))
*/

#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

#define MAXP 65540
#define LL __int64

int primes[MAXP];
bool notprime[MAXP];

// 厄尔多塞素数筛选法 
void Eratosthenes() {
	memset(notprime, false, sizeof(notprime));
	notprime[1] = true;
	primes[0] = 0;
	for(int i = 2; i < MAXP; i++) {
		if( !notprime[i] ) {
			primes[ ++primes[0] ] = i;
		    //需要注意i*i超出整型后变成负数的问题，所以转化成 __int64 
			for(LL j = (LL)i*i; j < MAXP; j += i) {
				notprime[j] = true;
			}
		}
	}
}

// 欧拉函数 - 获取小于n的数中与n互素的数的个数 
// 举例：
// Phi(10) = 4
// 即 1、3、7、9  总共4个 
int Phi(int n) {
	if(n == 1) {
		return 1;
	}
	int ans = 1; 
	// 素数试除 
	for(int i = 1; i <= primes[0]; i++) {
		int p = primes[i];
		if(n % p == 0) {
			n /= p;
			ans *= (p - 1);
			while( !(n % p) ) {
				n /= p;
				ans *= p;
			}
		}
		if(n == 1) {
			return ans;
		}
	}
	return ans * (n - 1);
}

int main() {
	Eratosthenes();
	printf("Phi(%d) = %d\n", 10, Phi(10));
	return 0;
}


