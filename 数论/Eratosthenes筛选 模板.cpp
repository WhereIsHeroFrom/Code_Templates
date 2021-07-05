/*
Eratosthenes素数筛选
    利用素数的倍数必然是合数这个性质，从小到大枚举，标记所有合数。
Author: WhereIsHeroFrom
Update Time: 2018-3-19
Algorithm Complexity: O(n)
*/

#include <iostream>
#include <cstring>
using namespace std;

const int MAXP = 65536;
#define ll long long

int primes[MAXP];
bool notprime[MAXP];
 
void Eratosthenes() {
	memset(notprime, false, sizeof(notprime));
	notprime[1] = true;
	primes[0] = 0;
	for(int i = 2; i < MAXP; i++) {
		if( !notprime[i] ) {
			primes[ ++primes[0] ] = i;
		    //需要注意i*i超出整型后变成负数的问题，所以转化成 __int64 
			for(ll j = (ll)i*i; j < MAXP; j += i) {
				notprime[j] = true;
			}
		}
	}
}

int main() {
	Eratosthenes();
	printf("%d\n", primes[0]);
	return 0;
}
