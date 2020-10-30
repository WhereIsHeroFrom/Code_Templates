#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;

#define EPS 1e-9
#define ValType double

// y = f(x)        原函数
typedef ValType(*origFunc)(ValType);
// F(x) = f'(x)    积分函数 
typedef ValType(*inteFunc)(ValType);

class IntegrationApproximation {
public:
	IntegrationApproximation() : m_kOrigFunc(NULL), m_kInteFunc(NULL) {}
	virtual ~IntegrationApproximation() {}
	inline void SetOrigFunc(origFunc oFunc) {
		m_kOrigFunc = oFunc;
	}
	inline void SetInteFunc(inteFunc iFunc) {
		m_kInteFunc = iFunc;
	}
	void    test(ValType s, ValType t);
	ValType getVal(ValType s, ValType t);
private:
	inline ValType _ladder(ValType u, ValType d, ValType h) {
		return (u + d) * h;
	}
	ValType ladder(ValType s, ValType t, ValType cnt);                                       // 梯形法则
	ValType integration(ValType s, ValType t);                                               // 定积分
	ValType simpson(ValType s, ValType t);                                                   // 辛普森
	ValType self_adapt_simpson(ValType s, ValType t, ValType simpsonval);  // 自适应辛普森

	origFunc             m_kOrigFunc;
	inteFunc             m_kInteFunc;
};


// 梯形法则
ValType IntegrationApproximation::ladder(ValType s, ValType t, ValType cnt) {
	if (m_kOrigFunc == NULL) {
		return 0;
	}
	ValType sum = 0;
	ValType step = (t - s) / cnt;
	ValType prex = s, prey = m_kOrigFunc(s);
	ValType nowx, nowy;
	for (ValType x = s; x < t; x += step) {
		nowx = x;
		nowy = m_kOrigFunc(nowx);

		sum += _ladder(prey, nowy, step);

		// 存储 prey ，减少一次 f(x) 的计算
		prex = nowx;
		prey = nowy;
	}

	nowx = t;
	nowy = m_kOrigFunc(nowx);

	sum += _ladder(prey, nowy, nowx - prex);

	return sum / 2;
}

// 定积分
ValType IntegrationApproximation::integration(ValType s, ValType t) {
	if (m_kInteFunc == NULL) {
		return 0.0;
	}
	return m_kInteFunc(t) - m_kInteFunc(s);
}

// 辛普森
ValType IntegrationApproximation::simpson(ValType s, ValType t) {
	if (m_kOrigFunc == NULL) {
		return 0.0;
	}
	ValType m = (s + t) / 2;
	return (t - s) / 6 * (m_kOrigFunc(s) + 4 * m_kOrigFunc(m) + m_kOrigFunc(t));
}

// 自适应辛普森
ValType IntegrationApproximation::self_adapt_simpson(ValType s, ValType t, ValType simpsonval) {
	ValType m = (s + t) / 2;
	ValType lval = simpson(s, m);
	ValType rval = simpson(m, t);
	ValType reval = lval + rval;
	// reval 一定比 simpsonval 更加准确
	// 区间切分越小肯定越准确，
	if (fabs(reval - simpsonval) < EPS) {
		return reval;
	}
	// 这里注意：精度也必须折半
	return self_adapt_simpson(s, m, lval) + self_adapt_simpson(m, t, rval);
}

// 自适应辛普森 对外接口 
ValType IntegrationApproximation::getVal(ValType s, ValType t) {
	return self_adapt_simpson(s, t, simpson(s, t));
}

void IntegrationApproximation::test(ValType s, ValType t) {
	printf("梯形法则(分段100):     %30.9lf\n", ladder(s, t, 100));
	printf("梯形法则(分段10000):   %30.9lf\n", ladder(s, t, 10000));
	printf("梯形法则(分段1000000): %30.9lf\n", ladder(s, t, 1000000));
	printf("辛普森算法:            %30.9lf\n", simpson(s, t));
	printf("自适应辛普森算法:      %30.9lf\n", self_adapt_simpson(s, t, simpson(s, t)));
	printf("积分:                  %30.9lf\n", integration(s, t));	
}

ValType f(ValType x) {
return 20 * sin(x / 5) + x / 3 + 20;
}

ValType f_integration(ValType x) {
return - 100 * cos(x / 5) + x * x / 6 + 20 * x;
}


/*
ValType f(ValType x) {
	return 27 * x*x - 36 * x + 65;
}

ValType f_integration(ValType x) {
	return 9*x*x*x - 18*x*x + 65*x;
}
*/
double a, b, c, d;
double l, r;

/*
ValType f(ValType x) {
	return (c*x+d) / (a*x+b);
}
*/

int main() {
	IntegrationApproximation A;
	A.SetOrigFunc(f);
	A.SetInteFunc(f_integration);
	A.test(30, 80);
	/*while (scanf("%lf %lf %lf %lf %lf %lf", &a, &b, &c, &d, &l, &r) != EOF) {
		//A.test(l, r);
		double ans = A.getVal(l, r);
		printf("%.6lf\n", ans);
	}
	*/
	return 0;
}

/*

15.720084231086
72.77300

34
10000 10000 -10000 10000
*/