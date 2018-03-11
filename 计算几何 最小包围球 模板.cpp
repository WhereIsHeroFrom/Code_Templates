#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

#define MAXN 1000010
#define eps 1e-6
const double PI = acos(-1.0);

double sqr(double x) {
	return x * x;
}

#define MAXL 5

class GaussMatrix {
public:
    int r, c;                 // r个方程，c个未知数 
    double d[MAXL][MAXL];     // 增广矩阵 
    double x[MAXL];           // 解集 
    /*
    d[0][0]   * x[0] +   d[0][1] * x[1] + ... +   d[0][c-1] * x[c-1] =   d[0][c];
    d[1][0]   * x[0] +   d[1][1] * x[1] + ... +   d[1][c-1] * x[c-1] =   d[1][c];
    ...
    ...
    d[r-1][0] * x[0] + d[r-1][1] * x[1] + ... + d[r-1][c-1] * x[c-1] = d[r-1][c];
    */
    
    void swap_row(int ra, int rb) {
        int i;
        for(i = 0; i <= c; i++) {
            double tmp = d[ra][i];
            d[ra][i] = d[rb][i];
            d[rb][i] = tmp;
            
        }
    }
    
    bool zero(double v) {
        return fabs(v) < eps;
    }
    
    bool gauss() {
        int i, j, k;
        int col = 0;           // 当前枚举列 
        int maxrow;            // 第col列中绝对值最大的行号
        
        for(i = 0; i < r && col < c; i++) {
            maxrow = i;
            for(j = i+1; j < r; j++) {
                if( fabs(d[j][col]) > fabs(d[maxrow][col]) ) {
                    maxrow = j;
                }
            }
            // 将第col列最大的行maxrow和第i行交换，避免误差 
            if(i != maxrow) swap_row(i, maxrow);
            // 如果第col中最大的那行的值为0继续找下一列的 
            if( zero(d[i][col]) ) {
                col ++;
                i --;
                continue;
            }
            for(j = i+1; j < r; j++) {
                // 将第j行第col列的元素消为0 
                if( !zero(d[j][col]) ) {
                    double sub = d[j][col]/d[i][col];
                    for(k = col; k <= c; k++) {
                        d[j][k] = d[j][k] - d[i][k] * sub; // 注意：这一步是关键，精度误差就在这里出现 
                    }
                }
            }
            col++;
        }
        
        // 唯一解 回归 
        for(i = c-1; i >= 0; i--) {
            double sum = 0;
            for(j = i+1; j < c; j++) {
                sum += x[j] * d[i][j];
            }
            x[i] = (d[i][c] - sum) / d[i][i];
            if( zero(x[i]) ) x[i] = 0;
        }
        return true;
    }    
    void debug_print_x() {
        int i;
        for(i = 0; i < c; i++) {
            printf("%.3lf ", x[i]);
        }
        puts("");
    }
    
    void debug_print() {
        int i, j;
        puts("---------------------------------");
        for(i = 0; i < r; i++) {
            for(j = 0; j <= c; j++) {
                printf("%.3lf ", d[i][j]);
            }
            puts("");
        }
        puts("---------------------------------");
    } 
};

struct PlaneParameter {
	// Ax + By + Cz + D = 0
	double A, B, C, D;
	
	PlaneParameter() {
	}
	PlaneParameter(double _A, double _B, double _C, double _D) {
		A = _A;
		B = _B;
		C = _C;
		D = _D;
	}
};

struct Point {
	double x, y, z;
	Point(){}
	Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {
	}
	void read() {
		scanf("%lf %lf %lf", &x, &y, &z);
	}
	Point operator+(const Point& other) const {
		return Point(x + other.x, y + other.y, z + other.z);
	}
	Point operator-(const Point& other) const {
		return Point(x - other.x, y - other.y, z - other.z);
	}
	double operator*(const Point& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	Point operator/(double k) const {
		return Point(x/k, y/k, z/k);
	}
	Point X(const Point& other) const {
		return Point(y*other.z - z*other.y,
		             z*other.x - x*other.z,
		             x*other.y - y*other.x);
	}
	
	double len() const {
		return sqrt(len_sqr());
	}
	double len_sqr() const {
		return sqr(x) + sqr(y) + sqr(z);
	} 
	double dist(const Point& other) const {
		return (*this - other).len();
	}
	Point centerWith(const Point& other) const {
		return (*this + other) / 2;
	}
	
	void print() {
		printf("%lf %lf %lf\n", x, y, z);
	}
	
	Point getExcenter(const Point& A, const Point& B) {
		Point OA = A - *this;
		Point OB = B - *this;
		PlaneParameter P[3];
		P[0] = PlaneParameter(2*OA.x, 2*OA.y, 2*OA.z, len_sqr()-A.len_sqr());
		P[1] = PlaneParameter(2*OB.x, 2*OB.y, 2*OB.z, len_sqr()-B.len_sqr());
		// 法向量 
		Point N = OA.X(OB);
		P[2] = PlaneParameter(N.x, N.y, N.z, -(N * A));
		
		GaussMatrix gm;
		gm.r = gm.c = 3;
		for(int i = 0; i < 3; ++i) {
			gm.d[i][0] = P[i].A;
			gm.d[i][1] = P[i].B;
			gm.d[i][2] = P[i].C;
			gm.d[i][3] = -P[i].D;
		}
		gm.gauss();
		return Point(gm.x[0], gm.x[1], gm.x[2]);
	}
	
	Point getBallCenter(const Point& A, const Point& B, const Point& C) {
		Point OA(A - *this), OB(B - *this), OC(C - *this);
		PlaneParameter P[3];
		P[0] = PlaneParameter(2*OA.x, 2*OA.y, 2*OA.z, len_sqr()-A.len_sqr());
		P[1] = PlaneParameter(2*OB.x, 2*OB.y, 2*OB.z, len_sqr()-B.len_sqr());
		P[2] = PlaneParameter(2*OC.x, 2*OC.y, 2*OC.z, len_sqr()-C.len_sqr());
		
		GaussMatrix gm;
		gm.r = gm.c = 3;
		for(int i = 0; i < 3; ++i) {
			gm.d[i][0] = P[i].A;
			gm.d[i][1] = P[i].B;
			gm.d[i][2] = P[i].C;
			gm.d[i][3] = -P[i].D;
		}
		gm.gauss();
		return Point(gm.x[0], gm.x[1], gm.x[2]);
	} 
}; 

struct Ball {
	Point center;
	double radius;

	Ball(){}
	Ball(Point c, double r): center(c), radius(r) {
	}

	bool isPointInBall(const Point& pt) {
		return (pt - center).len() <= radius;
	}
};

struct PointSet {
	int n;
	Point p[MAXN];

	// 随机增量法，打乱所有点的顺序
	void randomSuffle() {
		int i;
		for(i = 0; i < n; ++i) {
			// 交换p[i]和p[ridx]
			int ridx = rand() % n;
			Point tmp = p[i];
			p[i] = p[ridx];
			p[ridx] = tmp;
		}
	}
	
	// 获取前idx1个点的最小包围球(并且必须包含三个点p[idx1]和p[idx2]和p[idx3])
	// 如果当前点在球内，则不处理；否则，... 
	Ball getMinSurroundBallInclude3(int idx1, int idx2, int idx3) {
		int i;
		Point nowc = p[idx1].getExcenter(p[idx2], p[idx3]);
		double nowr = (nowc - p[idx3]).len();
		Ball now(nowc, nowr);
		for(i = 0; i < idx1; ++i) {
			if(!now.isPointInBall(p[i])) {
				Point nextc = p[i].getBallCenter(p[idx1], p[idx2], p[idx3]);
				double nextr = (nextc - p[idx3]).len();
				if(nextr > nowr) {
					nowc = nextc;
					nowr = nextr;
					now = Ball(nowc, nowr);
				}
			}
		}
		return now;
	}

	// 获取前idx1个点的最小包围球(并且必须包含两个点p[idx1]和p[idx2])
	// 如果当前点在球内，则不处理；否则，... 
	Ball getMinSurroundBallInclude2(int idx1, int idx2) {
		int i;
		Point nowc = p[idx1].centerWith(p[idx2]);
		double nowr = (nowc - p[idx2]).len();
		Ball now(nowc, nowr);
		
		for(i = 0; i < idx1; ++i) {
			if(!now.isPointInBall(p[i])) {
				now = getMinSurroundBallInclude3(i, idx1, idx2);			
			}
		}
		return now;
	}

	// 获取前idx个点的最小包围球(并且必须包含一个点p[idx])
	Ball getMinSurroundBallInclude1(int idx) {
		int i;
		// 1.做一个p[idx]为圆心，半径为0的球
		Ball now(p[idx], 0);

		// 2.枚举前idx个点，如果第i个点不在当前球体中，则生成一个新的球：
		//     这个球必须经过p[idx]和p[i]，并且包含前面所有点
		for(i = 0; i < idx; ++i) {
			if(!now.isPointInBall(p[i])) {
				now = getMinSurroundBallInclude2(i, idx);
			}
		}
		return now;
	}
	
	// 获取点集的最小包围球 
	Ball getMinSurroundBall() {
		int i;
		// 1.随机增量法，打乱所有点的顺序
		randomSuffle();
		
		Ball now(p[0], 0);
		// 2.枚举n个点，如果第i个点不在当前球体中，则生成一个新的球：
		//    这个球必须经过p[i]，并且包含前面所有点
		for(i = 1; i < n; ++i) {
			if(!now.isPointInBall(p[i])) {
				now = getMinSurroundBallInclude1(i);
			}
		}
		return now;
	}
};

PointSet ps;

int main() {
	int t;
	int i;
	while( scanf("%d", &ps.n) != EOF && ps.n ) {
		for(i = 0; i < ps.n; ++i) {
			ps.p[i].read();
		}
		Ball c = ps.getMinSurroundBall();
		printf("%.7lf\n", c.radius);
	}
	return 0;
}

/*

4
10.00000 10.00000 10.00000
10.00000 50.00000 50.00000
50.00000 10.00000 50.00000
50.00000 50.00000 10.000004



4
10.00000 10.00000 10.00000
20.00000 10.00000 10.00000
20.00000 20.00000 10.00000
10.00000 20.00000 10.00000
4
10.00000 10.00000 10.00000
10.00000 50.00000 50.00000
50.00000 10.00000 50.00000
50.00000 50.00000 10.00000

5
0 0 0
1 0 0
2 0 0
3 0 0
4 0 0

5
0 0 0 
0 1 0
0 2 0
0 3 0
0 4 0

*/
