/*
最小分数表示趋近无理数
 
	求一个分数，满足分母最大值为M，并且最接近sqrt(N)。 
*/
#include <iostream>
#include <cmath>
using namespace std;

#define LL __int64
int N, M;
LL sqr(LL x) {
    return x*x;
}

class Fraction {
	LL U, D;
public:
	Fraction() {
	}
	Fraction(LL u, LL d) {
		U = u;
		D = d;
	} 
	void reset(const Fraction &o) {
		U = o.U;
		D = o.D;
	}
	
	double value() {
		return U * 1.0 / D;
	}
	
	Fraction operator^(const Fraction& other) const {
		return Fraction(U+other.U, D+other.D);
	}
	
	// 计算分母不大于M，最接近 N^(1/2) 的分数 
	void getFraction(LL N, double val, Fraction l, Fraction r, LL maxD) {
		while(1) {
			Fraction mid = (l^r);
			if(mid.D > maxD) {
				// l < N^(1/2) < r
				// l^2 < N < r^2
				// 判断 N-l^2 < r^2-N
				// N-l^2 < r^2-N
				// l^2 + r^2 > 2N 则选择l，否则选择r 
				// 分子分母两边同时乘上 l.D*r.D 
				if(sqr(l.U*r.D) + sqr(l.D*r.U) > N*2*sqr(l.D*r.D)) {
					reset(l);
				}else {
					reset(r);
				}
				return ;
			}
			if(val*mid.D < mid.U) {
				r = mid;
			}else {
				l = mid;
			}
		}
	}

	void Print() {
		printf("%I64d/%I64d\n", U, D);
	}
};

int main() {
    int i, j;
    while(scanf("%d %d", &N, &M) != EOF) {
        int sqrtN = int(sqrt(N*1.0) + 1e-6);
        if(sqrtN*sqrtN == N) {
        	printf("%d/%d\n", sqrtN, 1);
        	continue;
		}
		Fraction ans;
		ans.getFraction(N, sqrt(N*1.0), Fraction(sqrtN, 1), Fraction(sqrtN+1, 1), M);
		ans.Print();
    }
    return 0;
} 
