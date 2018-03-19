/*
   因子 加减法
   任何自然数都可以表示成素数的乘积。
   大数的乘法和除法。比较耗时，可以先转换成素数乘积再进行因子合并和因子相减完成。

Author: WhereIsHeroFrom
Update Time: 2018-3-19
Algorithm Complexity: O(sqrt(n))
*/

#include <iostream> 
#include <cstdio>
#include <cstring>
#include <vector>
#include <memory.h>

using namespace std;

#define MAXP 65540
#define LL long long

#define MAX_FACTOR 200
#define MAX_FACTORS 1010
// MAX_FACTOR 必须大于 MAX_FACTORS的1/10

int primes[MAXP];
bool notprime[MAXP];

struct factor {
	int prime, count;
	factor() {
	} 
	factor(int p, int c) {
		prime = p;
		count = c;
	}
	void print() {
		printf("(%d, %d)\n", prime, count);
	}
};

struct factors {
	int factorCnt;
	factor data[MAX_FACTOR];

	factors(): factorCnt(0) {
	}

	// 清空，一般发生在素因子分解前
	void clear() {
		factorCnt = 0;
	}

	// 增加一组素因子
	void push(factor f) {
		data[factorCnt++] = f;
	}

	bool samePrime(int selfIdx, const factors& other, int otherIdx) {
		return data[selfIdx].prime == other.data[otherIdx].prime;
	}

	// 因子组合并
	factors operator+(const factors& other) {
		int i, j;
		factors ans;
		ans.clear();

		// 自己有的
		for(i = 0; i < factorCnt; ++i) {
			int p = data[i].prime;
			int cnt = data[i].count;
			for(j = 0; j < other.factorCnt; ++j) {
				if(samePrime(i, other, j)) {
					// 如果别人也有，则累加
					cnt += other.data[j].count;
					break;
				}
			}
			ans.push( factor(p, cnt) );
		}

		/// 自己没有，但别人有的 
		for(i = 0; i < other.factorCnt; ++i) {
			for(j = 0; j < factorCnt; ++j) {
				if(samePrime(j, other, i)) {
					break;
				}
			}
			if(j == factorCnt) {
				ans.push( other.data[i] );
			}
		}
		return ans;
	}

	// 因子组相减
	factors operator-(const factors& other) {
		factors ans;
		ans.clear();
		int i, j;
		for(i = 0; i < factorCnt; ++i) {
			int p = data[i].prime;
			int cnt = data[i].count;
			for(j = 0; j < other.factorCnt; ++j) {
				if(samePrime(i, other, j)) {
					cnt -= other.data[j].count;
					break;
				}
			}
			ans.push( factor(p, cnt) );
		}
		return ans;
	}

	void print() {
		for(int i = 0; i < factorCnt; ++i) {
			data[i].print();
		}
	}
};

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

// 因式分解 - 将n分解成素数幂乘积的形式
// 举例：
// 252 = (2^2) * (3^2) * (7^1) 
// 则 ans = [ (2,2), (3,2), (7,1) ]
void Factorization(int n, factors& ans) {
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
			ans.push(f);
		}
		if(n == 1) {
			return ;
		}
	}
	// 漏网之素数， 即大于MAXP的素数，最多1个 
	ans.push( factor(n, 1) );
}


factors Fac[MAX_FACTORS];

// 初始化前MAX_FACTORS个数的因子组
void initFactors() {
	int i;
	for(i = 1; i < MAX_FACTORS; ++i) {
		Factorization(i, Fac[i]);
		/*if(i < 10) {
			printf("<%d>\n", i);
			Fac[i].print();
		}*/
	}
}
