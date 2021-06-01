/*
线性同余
求解同余方程第一步是转化成一般式：ax + by = c，这个方程的求解步骤如下：
      i) 首先求出a和b的最大公约数d = gcd(a, b)，那么原方程可以转化成d(ax/d + by/d) = c，容易知道(ax/d + by/d)为整数，如若d不能整除b，方程必然无解，算法结束；否则进入ii)。
      ii) 由i)可以得知，方程有解则一定可以表示成 ax + by = c = gcd(a, b)*c'，那么我们先来看如何求解d = gcd(a, b) = ax + by，根据欧几里德定理，有：
      d = gcd(a, b) = gcd(b, a%b) = bx' + (a%b)y' = bx' + [a-b*(a/b)]y' = ay' + b[x' - (a/b)y'], 于是有x = y',  y = x' - (a/b)y'。
      由于gcd(a, b)是一个递归的计算，所以在求解(x, y)时，(x', y')其实已经利用递归计算出来了，递归出口为b == 0的时候（对比辗转相除，也是b == 0的时候递归结束），那么这时方程的解x0 = 1, y0 = 0。
*/

/*
逆元
已知 a 和 b，对于同余式：   ax % b = 1  求 x
1) 如果 a 和 b 不互素，则逆元不存在；
2）可以转化成   ax + bn = 1 ，求解一个可行解 x；
3）x = (x % n + n) % n 转换成最小的正整数；
*/

#include <iostream>

using namespace std;

#define ll long long
#pragma warning(disable : 4996)

ll gcd(ll a, ll b) {
    return b ? gcd(b, a % b) : a;
}

// 扩展欧几里得
// aX + bY = 1 
ll ExpGcd(ll a, ll b, ll& X, ll& Y) {
    ll q, temp;
    if (!b) {
        q = a; X = 1; Y = 0;
        return q;
    }
    else {
        q = ExpGcd(b, a % b, X, Y);
        temp = X;
        X = Y;
        Y = temp - (a / b) * Y;
        return q;
    }
}

// a * x % b = 1
// 求 x 
ll Inv(ll a, ll b) {
    ll X, Y;
    ExpGcd(a, b, X, Y);
    X = (X % b + b) % b;
    return X;
}