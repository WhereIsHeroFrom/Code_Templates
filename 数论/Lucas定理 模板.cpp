/*
Lucas定理

组合数c(n,m) mod p   
Lucas(n, m, p) = Lucas(n/p, m/p, p) * C(n%p, m%p, p) % p

n<p, m<p mod p
Comb(n,m,p) = n!/(m!(n-m)!) % p
令x = m!(n-m)!，x'为x对p的逆元，即x*x'%p=1，则Comb(n,m,p) = n!*x'%p
根据费马小定理a^(p-1)%p=1，则a的逆元为a^(p-2)%p，即x' = x^(p-2)%p，
Comb(n,m,p) = n! * (m!(n-m)!)^(p-2) % p
问题转化成求 n! % p。

Author: WhereIsHeroFrom
Update Time: 2018-3-21
Algorithm Complexity: O(log(n))
*/

#include <iostream>
#include <cstdio>
 
#define ll long long 
#define maxn 100010

ll FacCache[maxn];

// 二分快速幂 
ll Exp(ll a, ll n, ll Mod){
	ll ans = 1;
	while (n){
		if (n & 1) ans = ans * a % Mod;
		a = a * a % Mod;
		n >>= 1;
	}
	return ans;
}

// 费马小定理 
ll Inv(ll a, ll p) {
    return Exp(a, p-2, p);
}

// 计算阶乘 模 p，存入 FacCache
void CalcCache(int n, int p) {
    FacCache[0] = 1 % p;
    for(int i = 1; i <= n; ++i) {
        FacCache[i] = FacCache[i-1] * i;
        if(FacCache[i] >= p) FacCache[i] %= p;
    }
}

// 小组合数模 p 
// n,m < p
ll SmallComb(int n, int m, int p) {
    if(m == 0 || m == n) {
        return 1;
    }else if(m > n) {
        return 0;
    }
    // n! * m!^(-1) * (n-m)!^(-1)
    ll ans = FacCache[n] * Inv( FacCache[m], p) % p;
    return ans * Inv( FacCache[n-m], p) % p;
} 

// lucas 定理
ll Lucas (ll n, ll m, int p) {
    if(m == 0) {
        return 1;
    }
    return Lucas(n/p, m/p, p) * SmallComb(n % p, m % p, p) % p;
}
 
// 大组合数模 p
ll BigComb(ll n, ll m, int p) {
    if(p == 1) {
        return 0;
    }
    CalcCache(p, p);
    return Lucas(n, m, p);
} 


