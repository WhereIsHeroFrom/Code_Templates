/**************************************************|
|*                                                *|
|*         Rabin-Miller  &  Pollard-rho           *|
|*                                                *|
|**************************************************/
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;
typedef __int64 Int;
#define MAX (((Int)1)<<62)-1
Int p[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
Int X, Y;
Int exp_gcd(Int a, Int b, Int &X, Int &Y)
{
	Int q, temp;
	if (b == 0)
	{
		q = a;
		X = 1;
		Y = 0;
		return q;
	}
	else
	{

		q = exp_gcd(b, a % b, X, Y);
		temp = X;
		X = Y;
		Y = temp - (a / b) * Y;
		return q;
	}
}

Int Gcd(Int a, Int b)
{
	Int m = 1;
	if (!b)
		return a;
	while (m)
	{
		m = a % b;
		a = b;
		b = m;
	}
	return a;
}
//计算a*b%n
Int Produc_Mod(Int a, Int b, Int mod)
{
	Int sum = 0;
	while (b)
	{
		if (b & 1) sum = (sum + a) % mod;
		a = (a + a) % mod;
		b >>= 1;
	}
	return sum;
}


//计算a^b%n
Int Power(Int a, Int b, Int mod)
{
	Int sum = 1;
	while (b)
	{
		if (b & 1) sum = Produc_Mod(sum, a, mod);
		a = Produc_Mod(a, a, mod);
		b /= 2;
	}
	return sum;
}

//Rabin_Miller判素
bool Rabin_Miller(Int n)
{
	Int i, j, k = 0;
	Int u, m, buf;
	//将n-1分解为m*2^k
	if (n == 2)
		return true;
	if (n < 2 || !(n & 1))
		return false;
	m = n - 1;
	while (!(m & 1))
		k++, m /= 2;

	for (i = 0; i < 10; i++)
	{
		if (p[i] >= n)
			return true;

		u = Power(p[i], m, n);
		if (u == 1)
			continue;
		for (j = 0; j < k; j++)
		{
			buf = Produc_Mod(u, u, n);
			////看是否有非平凡因子存在

			if (buf == 1 && u != 1 && u != n - 1)
				return false;
			u = buf;
		}
		//如果p[i]^(n-1) % n != 1 那么 n为合数

		if (u != 1)
			return false;
	}
	return true;
}

//寻找n的一个因子

Int Pollard_rho(Int n)
{
	Int i = 1;
	Int x = rand() % (n - 1) + 1;
	Int y = x;
	Int k = 2;
	Int d;
	do{
		i++;
		d = Gcd(n + y - x, n);
		if (d > 1 && d < n)
			return d;
		if (i == k)
			y = x, k *= 2;
		x = (Produc_Mod(x, x, n) + n - 23) % n;
	} while (y != x);
	return n;
}