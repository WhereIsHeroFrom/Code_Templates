/*
因式分解
    素数筛选后，试除sqrt(n)以内的所有素数，层层约去。

Author: WhereIsHeroFrom
Update Time: 2018-3-21
Algorithm Complexity: O(sqrt(n))
*/

#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

const int maxn = 100005;
#define ll long long

int primes[maxn];
bool notprime[maxn];

struct factor {
	int prime, count;
	factor() :prime(0), count(0) {}
	factor(int p, int c) : prime(p), count(c) {}
	void print() {
		printf("(%d, %d)\n", prime, count);
	}
};
// 厄尔多塞素数筛选法 
void Eratosthenes() {
	memset(notprime, false, sizeof(notprime));
	notprime[1] = true;
	primes[0] = 0;
	for(int i = 2; i < maxn; i++) {
		if( !notprime[i] ) {
			primes[ ++primes[0] ] = i;
		    //需要注意i*i超出整型后变成负数的问题，所以转化成 __int64 
			for(ll j = (ll)i*i; j < maxn; j += i) {
				notprime[j] = true;
			}
		}
	}
}

// 因式分解 - 将n分解成素数幂乘积的形式
// 举例：
// 252 = (2^2) * (3^2) * (7^1) 
// 则 ans = [ (2,2), (3,2), (7,1) ]
void Factorization(int n, vector <factor>& ans) {
	ans.clear();
	if(n == 1) {
		return ;
	}
	// 素数试除 
	for(int i = 1; i <= primes[0]; i++) {
		if(n % primes[i] == 0) {
			factor f(primes[i], 0);
			while( !(n % primes[i]) ) {
				n /= primes[i];
				f.count ++;
			}
			ans.push_back(f);
		}
		if(n == 1) {
			return ;
		}
	}
	// 漏网之素数， 即大于MAXP的素数，最多1个 
	ans.push_back( factor(n, 1) );
}

int main() {
	Eratosthenes();
	vector <factor> ans;
	Factorization(252, ans);
	printf("%d = \n", 252);
	for(int i = 0; i < ans.size(); i++) {
		ans[i].print();
	}
	
	return 0;
}


