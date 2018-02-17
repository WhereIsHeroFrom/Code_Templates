#include <iostream>

using namespace std;

#define LL __int64
#define MAXN 100010

LL GCD(LL a, LL b) {
	return b ? GCD(b, a%b) : a;
}

// 扩展欧几里得
// aX + bY = 1 
LL ExpGcd(LL a, LL b, LL &X, LL &Y) {
     LL q, temp;
     if( !b ) {
         q = a; X = 1; Y = 0;
         return q;
     }else {
        q = ExpGcd(b, a % b, X, Y);
        temp = X; 
        X = Y;
        Y = temp - (a / b) * Y;
        return q;
	 }
}

// K = m[0] * x + r[0];
// K = m[1] * y + r[1];
// K = m[2] * z + r[2];

// 计算出y的表达式 m[0] * x - m[1] * y = r[1] - r[0];
// y = m[3] * v + r[3];


// 孙子定理 模板
// -1 表示无解 
LL chineseRemain(int n, LL mod[], LL rem[]) {
    LL lcm = 1;
    int i;
    // 1. 预处理，将所有的mod[]和rem[]都转化成正整数
	for(i = 0; i < n; ++i) {
		// K = x[i]*mod[i] + rem[i];     (K为最终解，x[i]为未知数)
		// a.除数取正 
		if(mod[i] < 0) {
			mod[i] = -mod[i];
		} 
		// b.余数取正
		rem[i] = (rem[i] % mod[i] + mod[i]) % mod[i];
	} 
    // 2.计算所有被模数的最小公倍数 
    for(i = 0; i < n; i++) {
        LL g = GCD(lcm, mod[i]);
        lcm = lcm / g * mod[i];
    }
    // 3.算法主流程 （令最终解为K） 
    //   枚举 i = 1 to n-1 
    //   1) 合并等式(0)和(i)， mod[0]*x[0] + rem[0]      = mod[i]*x[i] + rem[i];
    //                         mod[0]*x[0] - mod[i]*x[i] = rem[i] - rem[0];
    //                            A  *  X  +   B   * Y   = C;
    //   2) 根据扩展欧几里得，求得x[i] = Y = Y0 + A*t; 
    //   3) 将x[i]代入等式(i)，得到：   K = mod[0]*mod[i]*t + (rem[i]+mod[i]*Y0); 
    //   4) 更新等式(0)，    mod[0] = mod[0]*mod[i]
    //                       rem[0] = (rem[i]+mod[i]*Y0) % mod[0];
    //   5) n-1次迭代完毕，rem[0]就是最小非负整数解。 
    LL A, B, C, X, Y;
    for(i = 1; i < n; i++) {
        A = mod[0];
        B = -mod[i];
        C = rem[i] - rem[0];
        LL g = GCD(A, B);
        if( C % g ) {
            return -1;
        }
        // 等式两边同时除上g或-1，等式不变 
        A /= g, B /= g, C /= g;
        if(A < 0) {
        	// 这一步是为了把A和B都转化成正数 
        	A = -A, B = -B, C = -C;
        	B = ((B % A) + A) % A;
		}
        ExpGcd(A, B, X, Y);
        Y = ( (Y % A * C) % A + A ) % A;
        mod[0] = A*mod[i];
        rem[0] = (rem[i] + mod[i]*Y) % mod[0];
    }
    return rem[0];
}
 
LL m[MAXN], r[MAXN];


int main() {
    int t;
    int i;
    int n;
    while(  scanf("%d", &n) != EOF ) {
        for(i = 0; i < n; i++) {
            scanf("%I64d", &m[i]);
            scanf("%I64d", &r[i]);
        }
        printf("%I64d\n", chineseRemain(n, m, r) );    
    }
    return 0;
}

/*
3
107 1
1007 3
10007 5
796407100
*/ 
