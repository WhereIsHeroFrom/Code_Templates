/*
Eratosthenes 因子筛选
	在素数筛选的时候同时将因子筛选出来

Author: WhereIsHeroFrom
Update Time: 2021-06-03
Algorithm Complexity: O(n)
*/

#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

const int maxn = 1000005;
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
vector < factor > vF[maxn];

// 厄尔多塞素数筛选法
void Eratosthenes() {
	memset(notprime, false, sizeof(notprime));
	notprime[1] = true;
	primes[0] = 0;
	for (int i = 2; i < maxn; i++) {
		if (!notprime[i]) {
			primes[++primes[0]] = i;
			vF[i].push_back(factor(i, 1));
			for (int j = i * 2; j < maxn; j += i) {
				notprime[j] = true;
				int val = j / i;
				int cnt = 1;
				while (val % i == 0) {
					val /= i;
					++cnt;
				}
				vF[j].push_back(factor(i, cnt));
			}
		}
	}
}


int main() {
	Eratosthenes();
	for (int i = 0; i < vF[252].size(); i++) {
		vF[252][i].print();
	}
	return 0;
}


