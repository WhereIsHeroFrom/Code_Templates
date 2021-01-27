#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;

const int MAXP = 1100;
#define eps 1e-15
#define LL __int64
typedef double Type;

class HalfPlane;
class Polygon;

// 三值函数
int threeValue(Type d) {
    if(fabs(d) < 1e-6)
		return 0;
	return d > 0 ? 1 : -1;
}

// 两线段交点类型
enum SegCrossType {
	SCT_NONE = 0,
	SCT_CROSS = 1,         // 正常相交
	SCT_ENDPOINT_ON = 2,   // 其中一条线段的端点在另一条上
};

class Point2D {
    Type x, y;

public:
	Point2D(){
	}
	Point2D(Type _x, Type _y): x(_x), y(_y) {}
	void read() {
		scanf("%lf %lf", &x, &y);
	}
	void print() {
		printf("<%lf, %lf>\n", x, y);
	}
    Point2D turnLeft();
    Point2D turnRight();
	double angle();
	Point2D operator+(const Point2D& other) const;
	Point2D operator-(const Point2D& other) const;
	Point2D operator*(const double &k) const;
	Point2D operator/(const double &k) const;
	Type operator*(const Point2D& other) const;
    bool operator <(const Point2D &p) const;
	Type X(const Point2D& other);
	double len();
	void normalize();
};

typedef Point2D Vector2D;

double Vector2D::len() {
	return sqrt(x*x + y*y);
}

void Vector2D::normalize() {
	double l = len();
	if(threeValue(l)) {
		x /= l;
		y /= l;
	}
}

Point2D Point2D::turnLeft() {
    return Point2D(-y, x);
}
Point2D Point2D::turnRight() {
    return Point2D(y, -x);
}
double Point2D::angle() {
	return atan2(y, x);
}

Point2D Point2D::operator+(const Point2D& other) const {
	return Point2D(x + other.x, y + other.y);
}

Point2D Point2D::operator-(const Point2D& other) const {
	return Point2D(x - other.x, y - other.y);
}

Point2D Point2D::operator *(const double &k) const {
    return Point2D(x * k, y * k);
}

Point2D Point2D::operator /(const double &k) const {
    return Point2D(x / k, y / k);
}

bool Point2D::operator <(const Point2D &p) const {
    return y + eps < p.y || ( y < p.y + eps && x + eps < p.x );
}

// !!!!注意!!!!
// 如果Type为int，则乘法可能导致int32溢出，小心谨慎
// 点乘
Type Point2D::operator*(const Point2D& other) const {
	return x*other.x + y*other.y;
}
// 叉乘
Type Point2D::X(const Point2D& other) {
	return x*other.y - y*other.x;
}

class Segment2D {
	Point2D s, t;
public:
	void read() {
		s.read();
		t.read();
	}

	// 定点叉乘
	// 外部找一点p，然后计算 (p-s)×(t-s)
	Type cross(const Point2D& p) const {
		return (p - s).X(t - s);
	}

	// 跨立测验
	// 将当前线段作为一条很长的直线，检测线段other是否跨立在这条直线的两边
	bool lineCross(const Segment2D& other) const;
	
	// 点是否在线段上
	bool pointOn(const Point2D& p) const;

	// 线段判交
	// 1.通过跨立测验
	// 2.点是否在线段上
	SegCrossType segCross(const Segment2D& other);
};

bool Segment2D::lineCross(const Segment2D& other) const {
	return threeValue(cross(other.s)) * threeValue(cross(other.t)) == -1;
}

bool Segment2D::pointOn(const Point2D& p) const {
	// 满足两个条件：
	//  1.叉乘为0，    (p-s)×(t-s) == 0
	//  2.点乘为-1或0，(p-s)*(p-t) <= 0
	return cross(p) == 0 && (p-s)*(p-t) <= 0;
}

SegCrossType Segment2D::segCross(const Segment2D& other) {
	if(this->lineCross(other) && other.lineCross(*this)) {
		// 两次跨立都成立，则必然相交与一点
		return SCT_CROSS;
	}
	// 任意一条线段的某个端点是否在其中一条线段上，四种情况
	if(pointOn(other.s) || pointOn(other.t) ||
		other.pointOn(s) || other.pointOn(t) ) {
			return SCT_ENDPOINT_ON;
	}
	return SCT_NONE;
}

class Line2D {
public:
	Point2D a, b;
	Line2D() {}
	Line2D(const Point2D& _a, const Point2D& _b): a(_a), b(_b) {}
	Point2D getCrossPoint(const Line2D& );
	bool isParallelTo(const Line2D& );
};

// 两条不平行的直线必有交点
// 利用叉乘求面积法的相似三角形比值得出交点
Point2D Line2D::getCrossPoint(const Line2D& other) {
	double SA = (other.a - a).X(b - a);
	double SB = (b - a).X(other.b - a);
	return (other.b * SA + other.a * SB) / (SA + SB);
}

bool Line2D::isParallelTo(const Line2D& other) {
	return !threeValue( (b-a).X(other.b - other.a) );
}

/* 半平面定义：
   沿着射线(a->b)方向的左手面定义为半平面
   所有的多边形建立的时候，需要注意，保证点按照逆时针排列
   判断是否逆时针排列可以用面积 >0 判定
*/
class HalfPlane : public Line2D {
	double angle;
public:
	HalfPlane() {}
    HalfPlane(const Point2D &_a, const Point2D& _b) {
		a = _a;
		b = _b;
		angle = (_b - _a).angle();
    }
	bool equalAngle(const HalfPlane &other) const;
	bool operator < (const HalfPlane &other) const;	
	Type X(const Point2D &p) const;
	bool isPointIn(const Point2D &p) const;
	void move(double dist);
};

class HalfPlanes {
	int n;
	HalfPlane hp[MAXP];
	void unique();

	Point2D p[MAXP];
	int que[MAXP];
	int head, tail;

	void print();
public:
	HalfPlanes() {}
	~HalfPlanes() {}
	void init();
	int size() {return n;}
	void getHalfPlane(HalfPlanes& H);
	void addHalfPlane(const HalfPlane &);
	void addRectBorder(Type lx, Type ly, Type rx, Type ry);
	bool doIntersection();
    void getConvex(Polygon &convex);

	static HalfPlanes &Instance() {
		static HalfPlanes inst;
		return inst;
	}
};

struct Polygon {
    int n;
    Point2D p[MAXP];

	void print();
	double area();
    void getConvex(Polygon &c);
    bool isConvex();
    bool isPointInConvex(const Point2D &P);
    Point2D CalcBary();
	double getPolygonIntersect(Polygon& other);
	void convertToCounterClockwise();
	int constructTriangleHalfPlane(const Point2D &A, const Point2D &B, const Point2D &C,  HalfPlane* hp);
};

void Polygon::print() {
	int i;
	printf("%d\n", n);
	for(i = 0; i < n; ++i) {
		p[i].print();
	}
}

double Polygon::area() {
    double sum = 0;
	p[n] = p[0];
    for ( int i = 0 ; i < n; i ++ )
        sum += p[i].X(p[i + 1]);
    return sum / 2;
}

// 求凸包
void Polygon::getConvex(Polygon &c) {
    sort(p, p + n);
    c.n = n;
    for (int i = 0; i < n; ++i) {
        c.p[i] = p[i];
    }
    if (n <= 2) {
        return;
    }
        
	int &top = c.n;
    top = 1;
    for ( int i = 2 ; i < n ; i ++ ) {
        while ( top && threeValue(( c.p[top] - p[i] ).X( c.p[top - 1] - p[i] )) >= 0 )
            top --;
        c.p[++top] = p[i];
    }
    int temp = top;
    c.p[++ top] = p[n - 2];
    for ( int i = n - 3 ; i >= 0 ; i -- ) {
        while ( top != temp && threeValue(( c.p[top] - p[i] ).X( c.p[top - 1] - p[i] )) >= 0 )
            top --;
        c.p[++ top] = p[i];
    }
}

// 是否凸多边形
bool Polygon::isConvex() {
    bool s[3] = { false , false , false };
    p[n] = p[0], p[n + 1] = p[1];
    for ( int i = 0 ; i < n ; i ++ ) {
        s[threeValue(( p[i + 1] - p[i] ) * ( p[i + 2] - p[i] )) + 1] = true;
		// 叉乘有左有右，肯定是凹的
        if ( s[0] && s[2] ) return false;
    }
    return true;
}

// 点是否在凸多边形内
bool Polygon::isPointInConvex(const Point2D &P) {
    bool s[3] = { false , false , false };
    p[n] = p[0];
    for ( int i = 0 ; i < n ; i ++ ) {
        s[threeValue(( p[i + 1] - P ) * ( p[i] - P )) + 1] = true;
        if ( s[0] && s[2] ) return false;
        if ( s[1] ) return true;
    }
    return true;
}

// 转成逆时针顺序
void Polygon::convertToCounterClockwise() {
	if(area() >= 0) {
		return ;
	}
	for(int i = 1; i <= n / 2; ++i) {
		Point2D tmp = p[i];
		p[i] = p[n-i];
		p[n-i] = tmp;
	}
}

// 求两任意多边形的交
// 要求两个多边形逆时针排列
double Polygon::getPolygonIntersect(Polygon& other) {
	int i, j;
	HalfPlane hp[2][3];
	Polygon g;
	
	convertToCounterClockwise();
	other.convertToCounterClockwise();

	double area = 0;
	// 枚举每个三角形，记得正负
	for(i = 0; i < n; ++i) {
		Point2D O(0, 0);
		// 两个多边形分别拆成两个有向三角形，求半平面交，再累加
		int td = constructTriangleHalfPlane(O, p[i], p[i+1], hp[0]);
		if(td == 0)
			continue;

		for(j = 0; j < other.n; ++j) {
			HalfPlanes &h = HalfPlanes::Instance();
			h.init();
			int flag = td * constructTriangleHalfPlane(O, other.p[j], other.p[j+1], hp[1]);
			if(flag) {
				h.addHalfPlane(hp[0][0]), h.addHalfPlane(hp[0][1]), h.addHalfPlane(hp[0][2]);
				h.addHalfPlane(hp[1][0]), h.addHalfPlane(hp[1][1]), h.addHalfPlane(hp[1][2]);

				if(h.doIntersection()) {
					h.getConvex(g);
					if( threeValue(g.area()) ) {
						area += flag * g.area();
					}
				}
			}
		}
	}
	// 注意！！！！这里求出来的相交 面积允许为负数
	return area;
}

int Polygon::constructTriangleHalfPlane(const Point2D &A, const Point2D &B, const Point2D &C, HalfPlane* hp) {
	int v = threeValue((B-A).X(C-A));
	if(v == 0) return 0;
	if(v < 0) {
		hp[0] = HalfPlane(A, C);
		hp[1] = HalfPlane(C, B);
		hp[2] = HalfPlane(B, A);
		return -1;
	}else {
		hp[0] = HalfPlane(A, B);
		hp[1] = HalfPlane(B, C);
		hp[2] = HalfPlane(C, A);
		return 1;
	}
}


Point2D Polygon::CalcBary() {
    Point2D ret(0, 0);
    double area = 0;
    p[n] = p[0];
    for ( int i = 0 ; i < n ; i ++ ) {
        double temp = p[i] * p[i + 1];
        if ( threeValue(temp) == 0 ) continue;
        area += temp;
        ret = ret + ( p[i] + p[i + 1] ) * ( temp / 3 );
    }
    return ret / area;
}


bool HalfPlane::equalAngle(const HalfPlane &other) const {
	return !threeValue(angle - other.angle);
}

// 极角不同则按照极角从小到大排序
// 如果极角相同，则进行如下操作：
// 向量A = (b - a) 叉乘 向量 B = (other.a - a)
// 如果大于0，说明向量A在向量B的右侧，保留A，剔除B；
bool HalfPlane::operator < (const HalfPlane &other) const {
	if( threeValue(angle - other.angle) ) {
		return angle < other.angle;
	}
	return !isPointIn(other.a);
}

// 这个函数非常有用 用于isPointIn的判断
// 叉乘结果 <= 0，则说明p点在半平面区域内
Type HalfPlane::X(const Point2D &p) const {
    return ( b - a ).X( p - a );
}

bool HalfPlane::isPointIn(const Point2D &p) const {
	return threeValue( X(p) ) >= 0;
}

// 半平面靠着左方进行缩进，缩进距离为dist
void HalfPlane::move(double dist) {
	Point2D t = (b-a).turnLeft();
	t.normalize();
	a = a + t * dist;
	b = b + t * dist;
}

void HalfPlanes::print() {
	for(int i = 0; i < n; ++i) {
		printf("<%d>\n", i);
		hp[i].a.print();
		hp[i].b.print();
	}
}

void HalfPlanes::init() {
	n = 0;
}

void HalfPlanes::addHalfPlane(const HalfPlane & h) {
	hp[n++] = h;
}

void HalfPlanes::addRectBorder(Type lx, Type ly, Type rx, Type ry) {
	addHalfPlane( HalfPlane(Point2D(lx, ly),  Point2D(rx, ly) ));
	addHalfPlane( HalfPlane(Point2D(rx, ly),  Point2D(rx, ry) ));
	addHalfPlane( HalfPlane(Point2D(rx, ry),  Point2D(lx, ry) ));
	addHalfPlane( HalfPlane(Point2D(lx, ry),  Point2D(lx, ly) ));
}

void HalfPlanes::getHalfPlane(HalfPlanes& H) {
	for(int i = 0; i < H.size(); ++i) {
		addHalfPlane(H.hp[i]);
	}
}
void HalfPlanes::unique() {
	int m = 1;
	for(int i = 1; i < n; ++i) {
		// 极角相同取其前
		if( !hp[i].equalAngle(hp[i-1]) ) {
			hp[m++] = hp[i];
		}
	}
	n = m;
}

bool HalfPlanes::doIntersection() {
	// 1.按照极角排序，剔除极角相同的半平面
	sort(hp, hp + n);
	unique();
	//print();
	// 2.将前两个半平面放入单调队列，并且计算出交点
	que[ head=0 ] = 0, que[ tail=1 ] = 1;
	p[1] = hp[0].getCrossPoint(hp[1]);
	// 3.按照极角顺序线性枚举所有平面，和队列中的半平面进行求交运算
	for(int i = 2; i < n; ++i) {
		// 前两个半平面交点p[tail]，不在当前半平面内，则删除前一个半平面
		while(head < tail && !hp[i].isPointIn(p[tail]) )
			--tail;
		// 判断另一侧的半平面交点p[head+1]，不在当前半平面内，则进行平面剔除
		while(head < tail && !hp[i].isPointIn(p[head+1]) )
			++head;
		// 如果某个时刻 两个反向半平面平行了，必然无解
		if(hp[i].isParallelTo(hp[que[tail]])) {
			return false;
		}
		// 将当前半平面压入队列
		que[ ++tail ] = i;
		p[tail] = hp[i].getCrossPoint(hp[que[tail-1]]);
	}
	// 4.队列首的那个半平面，需要满足所有点都在它的左边
	while(head < tail && !hp[que[head]].isPointIn(p[tail]))
		--tail;
	while(head < tail && !hp[que[tail]].isPointIn(p[head+1]))
		++head;
	// 至少三个半平面才能构成一个封闭区域
	return head + 1 < tail;
}

void HalfPlanes::getConvex(Polygon &convex) {
	p[head] = hp[que[head]].getCrossPoint(hp[que[tail]]);
    convex.n = tail - head + 1;
    for ( int j = head, i = 0 ; j <= tail; i ++, j ++ ) {
        convex.p[i] = p[j];
    }
	convex.p[ convex.n ] = convex.p[0];
}

// 获取多边形的核
// 
bool getPolygonKernel(Polygon &poly, Polygon& ansPoly, double movedist,  double& area) {
	int i;
	HalfPlanes &h = HalfPlanes::Instance();
	h.init();

	// 面积大于0，逆时针
	if(poly.area() > 0) {
		for(i = 0; i < poly.n; ++i) {
			HalfPlane hp(poly.p[i], poly.p[i+1]);
			hp.move(movedist);
			h.addHalfPlane(hp);
		}
	}else {
		for(i = 0; i < poly.n; ++i) {
			HalfPlane hp(poly.p[i+1], poly.p[i]);
			hp.move(movedist);
			h.addHalfPlane(hp);
		}
	}
	if( h.doIntersection() ) {
		h.getConvex(ansPoly);
		area = ansPoly.area();
		return true;
	}else {
		area = 0;
		return false;
	}
}


// 获取多边形的核
bool getPolygonKernel(double lx, double ly, double rx, double ry, Polygon &poly, HalfPlanes& planes, double& area) {
	int i;
	HalfPlanes &h = HalfPlanes::Instance();
	h.init();
	h.addRectBorder(lx, ly, rx, ry);
	h.getHalfPlane(planes);

	if( h.doIntersection() ) {
		h.getConvex(poly);
		area = poly.area();
		return true;
	}else {
		area = 0;
		return false;
	}
}

Polygon poly[2];

int main() {
	int i, j;
	while(scanf("%d %d", &poly[0].n, &poly[1].n) != EOF) {
		double tot = 0;
		for(i = 0; i < 2; ++i) {
			for(j = 0; j < poly[i].n; ++j) {
				poly[i].p[j].read();
			}
			poly[i].p[poly[i].n] = poly[i].p[0];
			tot += fabs(poly[i].area());
		}
		tot -= poly[0].getPolygonIntersect(poly[1]);
		printf("%.2lf\n", tot);
	}
	return 0;
}
