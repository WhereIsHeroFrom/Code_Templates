#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

#define ll long long
const int maxn = 200010;
const double eps = 1e-8;

struct Interval {
    int l, r;  
    Interval(int _l, int _r): l(_l), r(_r) {}
    void print() {
        printf("[%d, %d]\n", l, r);
    }
};

//  g(x) = n / x     (x in [1, maxx])
// 将 g(x) 的定义域 x 分拆成 最多 2sqrt(n) 个区间，存储在 ret 返回 
void integerSplit(int maxx, int n, vector<Interval> &ret) {
    ret.clear();
    double sqrtn = sqrt(n + 0.0);
    int last = 1;
    // 1. 计算 x <= sqrt(n) 的部分； 
    for(int x = 1; x <= sqrtn; ++x) {
        if(x > maxx) {
            return;
        } 
        ret.push_back( Interval(x, x) );
        last = x;
    }
    int intsqrtn = (int)(sqrtn + eps);
    // 2. 计算 x > sqrt(n) 的部分 （即 g(x) < sqrt(n) 的情况）； 
    for(int x = intsqrtn; x >= 1; --x) {
        int now = n / x;
        
        if(last+1 > now) {
            continue;
        }
        
        if(now > maxx) {
            now = maxx;
            ret.push_back( Interval(last+1, now) );
            return ;
        }
        ret.push_back( Interval(last+1, now) );
        last = now;
    }
}
