/*
模拟退火
    1、初温控制 尽量取最大值
    2、初始点集 可以根据问题特点 取1（优先考虑）个或多个
    3、最小温度精度 控制在问题需要的精度的下一个数量级
    4、候选点集尽量取得能力范围之内的多者
    5、最重要：模拟退火的时候遇到较差的解，根据概率性选择较差解；当然，也要根据实际情况放弃较差解。
    
Author: WhereIsHeroFrom
Update Time: 2018-3-31
Algorithm Complexity: 取决于下降率的非2为底的log级复杂度
*/

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>

using namespace std;

#define PI acos(-1.0)
#define eps 1e-6
typedef double Type;

// 三值函数
int threeValue(Type d) {
    if(fabs(d) < 1e-6)
		return 0;
	return d > 0 ? 1 : -1;
}

class Point3D {
    Type x, y, z;

public:
	Point3D(){
	}
	Point3D(Type _x, Type _y, Type _z): x(_x), y(_y), z(_z) {}
	void read() {
		scanf("%lf %lf", &x, &y);
		z = 0;
		//scanf("%lf %lf %lf", &x, &y, &z);
	}
	void print() {
		printf("<x=%lf, y=%lf, z=%lf>\n", x, y, z);
	}
	inline Type getx() { return x; }
	inline Type gety() { return y; }
	inline Type getz() { return z; }
	bool inRange(Point3D& max) const;
	Point3D operator+(const Point3D& other) const;
	Point3D operator-(const Point3D& other) const;
	Point3D operator*(const double &k) const;
	Point3D operator/(const double &k) const;
	Type operator*(const Point3D& other) const;
	double len();
	Point3D normalize();
};

typedef Point3D Vector3D;

double Vector3D::len() {
	return sqrt(x*x + y*y + z*z);
}

Point3D Vector3D::normalize() {
	double l = len();
	if(threeValue(l)) {
		x /= l;
		y /= l;
		z /= l;
	}
	return *this;
}

bool Point3D::inRange(Point3D& max) const {
	return (0<=x&&x<=max.x) && (0<=y&&y<=max.y) && (0<=z&&z<=max.z);
}

Point3D Point3D::operator+(const Point3D& other) const {
	return Point3D(x + other.x, y + other.y, z + other.z);
}

Point3D Point3D::operator-(const Point3D& other) const {
	return Point3D(x - other.x, y - other.y, z - other.z);
}

Point3D Point3D::operator *(const double &k) const {
    return Point3D(x * k, y * k, z * k);
}

Point3D Point3D::operator /(const double &k) const {
    return (*this) * (1/k);
}

Type Point3D::operator*(const Point3D& other) const {
	return x*other.x + y*other.y + z*other.z;
}


#define MAXN 1010
#define MAXC 1000
#define INF 1000000000.0

struct Point3DSet {
	int n;
	Point3D p[MAXN];
};

/*
	模拟退火-模板
	最远的最近距离
*/
class simulatedAnnealing {
	// 稳态（最低）温度
	static const double minTemperature;
	// 温度下降率
	static const double deltaTemperature;
	// 并行候选解个数
	static const int solutionCount;
	// 每个解的迭代次数
	static const int candidateCount;
private:
	Point3D bound;
	Point3D x[MAXC];
	Point3DSet pointSet;
	double temperature;

	bool valid(const Point3D& pt);
	double randIn01();
	Point3D getRandomPoint();
	Vector3D getRandomDirection();
	Point3D getNext(const Point3D& now);
public:
	void start(double T, Point3D B, Point3DSet& pointSet);
	double evaluateFunc(const Point3D& pt);
	Point3D getSolution();
	static simulatedAnnealing& Instance();
};

// 四个调整参数
// 稳态（最低）温度
const double simulatedAnnealing::minTemperature = 1e-2;
// 温度下降率
const double simulatedAnnealing::deltaTemperature = 0.95;
// 并行候选解个数
const int simulatedAnnealing::solutionCount = 10;
// 每个解的迭代次数
const int simulatedAnnealing::candidateCount = 30;

bool simulatedAnnealing::valid(const Point3D& pt) {
	return pt.inRange(bound);
}

double simulatedAnnealing::randIn01() {
	return (rand() + 0.0) / RAND_MAX;
}

/*
	估价函数，估值越小越优
	不同问题，基本只需要修改估价函数，这个是 最远的最近距离
*/
double simulatedAnnealing::evaluateFunc(const Point3D& pt) {
	// TODO
	// 最小距离 越大越优，所以估值取相反数
	double minDist = INF;
	for(int i = 0; i < pointSet.n; ++i) {
		double dist = (pointSet.p[i] - pt).len();
		if(dist < minDist) 
			minDist = dist;
	}
	return - minDist;
}

/*
	随机一个[0 - bound]的点，如果要求有负数点，请将整个坐标轴进行平移
*/
Point3D simulatedAnnealing::getRandomPoint() {
	return Point3D(bound.getx() * randIn01(), bound.gety() * randIn01(), 0);//bound.getz() * randIn01());
}

/*
	随机一个方向，注意方向可以是任何方向，别忘了负数
*/
Vector3D simulatedAnnealing::getRandomDirection() {
	Vector3D v(randIn01()-0.5, randIn01()-0.5, 0);//randIn01()-0.5);
	return v.normalize();
}

Point3D simulatedAnnealing::getNext(const Point3D& now) {
	return now + getRandomDirection() * temperature;
}

/*
  模拟退火
*/
void simulatedAnnealing::start(double T, Point3D B, Point3DSet& PS) {
	// 0.初始化温度
	temperature = T;
	bound = B;
	pointSet = PS;
	int i, j;

	// 1.随机生成solutionCount个初始解
	for(i = 0; i < solutionCount; ++i) {
		x[i] = getRandomPoint();
	}

	while (temperature > minTemperature) {
		// 2.对每个当前解进行最优化选择
		for(i = 0; i < solutionCount; ++i) {
			double nextEval = INF;
			Point3D nextOpt;
			// 3.对于每个当前解，随机选取附近的candidateCount个点，并且将最优的那个解保留
			for(j = 0; j < candidateCount; ++j) {
				Point3D next = getNext(x[i]);
				if(!valid(next)) {
					continue;
				}
				double Eval = evaluateFunc(next);
				if(Eval < nextEval) {
					nextEval = Eval;
					nextOpt = next;
				}
			}

			// 4.没有生成可行解
			if(nextEval >= INF)
				continue;

			// 5.计算生成的最优解和原来的解进行比较
			double deltaEval = evaluateFunc(nextOpt) - evaluateFunc(x[i]);
			if(deltaEval < 0) {
				// 6.比原来的解更优，直接替换
				x[i] = nextOpt;
			}else {
				// 7.没有原来的解优，则以一定概率进行接收
				// 这个概率上限会越来越小，直到最后趋近于0
				// 理论上，这个分支也可能不考虑
				if( randIn01() < exp(-deltaEval/temperature) ) {
					x[i] = nextOpt;
				}	
			}
		}
		temperature *= deltaTemperature;
	}
	//for(i = 0; i < solutionCount; ++i) x[i].print();
}

Point3D simulatedAnnealing::getSolution() {
	int retIdx = 0;
	for (int i = 1; i < solutionCount; ++i) {
		if(evaluateFunc(x[i]) < evaluateFunc(x[retIdx])) {
			retIdx = i;
		}
	}
	return x[retIdx];
}

simulatedAnnealing& simulatedAnnealing::Instance() {
	static simulatedAnnealing inst;
	return inst;
}
