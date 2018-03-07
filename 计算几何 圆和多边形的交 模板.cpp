#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;

const int MAXP = 110000;
#define PI acos(-1.0)
#define eps 1e-15
#define LL __int64
typedef double Type;

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
	Type getx() const { return x; }
	Type gety() const { return y; }
    Point2D turnLeft();
    Point2D turnRight();
	double angle();
	Point2D operator+(const Point2D& other) const;
	Point2D operator-(const Point2D& other) const;
	Point2D operator*(const double &k) const;
	Point2D operator/(const double &k) const;
	Type operator*(const Point2D& other) const;
    bool operator <(const Point2D &p) const;
	Type X(const Point2D& other) const;
	double len();
	Point2D normalize();
	bool sameSide(Point2D a, Point2D b);
};

typedef Point2D Vector2D;

double Vector2D::len() {
	return sqrt(x*x + y*y);
}

Point2D Vector2D::normalize() {
	double l = len();
	if(threeValue(l)) {
		x /= l;
		y /= l;
	}
	return *this;
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
Type Point2D::X(const Point2D& other) const {
	return x*other.y - y*other.x;
}

// 向量a和b是否在当前向量的同一边
bool Point2D::sameSide(Point2D a, Point2D b) {
	// 请保证 当前向量和a、b向量的起点一致
	return threeValue(X(a)) * threeValue(X(b)) == 1;
}

class Segment2D {
	Point2D s, t;
public:
	Segment2D() {}
	Segment2D(const Point2D& _s, const Point2D& _t) : s(_s), t(_t) {
	}
	void read() {
		s.read();
		t.read();
	}
	Type sXt() const {
		return s.X(t);
	}

	Vector2D vector() const {
		return t - s;
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
	Line2D(const Point2D& _a, const Point2D& _b): a(_a), b(_b) {
	}
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

class line2DTriple {
public:
	// ax + by + c = 0;
	Type a, b, c;

	line2DTriple(){}
	line2DTriple(const Point2D& _a, const Point2D& _b) {
		getFromSegment2D(Segment2D(_a, _b));
	}
	line2DTriple(const Segment2D& s) {
		getFromSegment2D(s);
	}
	void getFromSegment2D(const Segment2D&);
	double getPointDist(const Point2D& p);
};

void line2DTriple::getFromSegment2D(const Segment2D& seg) {
	Vector2D v = seg.vector();
	a = - v.gety();
	b = v.getx();
	c = seg.sXt();
	//Print();
}

double line2DTriple::getPointDist(const Point2D& p) {
	return fabs(a*p.getx() + b*p.gety() + c) / sqrt(a*a+b*b);
}

struct Polygon {
    int n;
    Point2D p[MAXP];

	void print();
	double area();
    void getConvex(Polygon &c);
    bool isConvex();
    bool isPointInConvex(const Point2D &P);
    Point2D CalcBary();
	void convertToCounterClockwise();
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
	if(n <= 2) {
		for(int i = 0; i < n; ++i) {
			c.p[i] = p[i];
		}
		return ;
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

// 点对
struct Point2D_Pair {
	Point2D a, b;
	Point2D_Pair() {}
	Point2D_Pair(Point2D _a, Point2D _b): a(_a), b(_b) { 
	}
	Point2D center();
	Vector2D direction();
	double len();
	void print();
};

Point2D Point2D_Pair::center() {
	return (a + b) / 2;
}

Vector2D Point2D_Pair::direction() {
	return b - a;
}

double Point2D_Pair::len() {
	return direction().len();
}

void Point2D_Pair::print() {
	printf("点对如下：\n");
	a.print();
	b.print(); 
}

class Circle {
	Point2D center;
	double radius;
public:
	Circle() {}
	Circle(Point2D c, double r): center(c), radius(r) {
	}

	double getOriginFanArea(double angle);
	double getOriginTriangleArea(Point2D A, Point2D B);
	double intersectWithPolygon(Polygon &poly);

	static int getCenterByTwoPoints(Point2D_Pair p, double r, Point2D_Pair& ret);
	static int getCenterByAngle(Point2D o, Point2D a, Point2D b, double r, Point2D& ret);
};

// 给定两个点和一个半径r，求经过这两点的圆的圆心。
// 返回值：圆心数量 （0/1/2）
int Circle::getCenterByTwoPoints(Point2D_Pair p, double r, Point2D_Pair& ret) {
	double chordal = p.len();
	int rc = threeValue(r - chordal/2);
	if(rc == -1) {
		// 1.半径小于弦长一半，无解
		return 0;
	}else if(rc == 0) {
		// 2.半径等于弦长一半，圆心唯一，为弦中点
		ret.a = ret.b = p.center();
		return 1;
	}else {
		// 3.半径大于弦长一半，则半径为斜边，弦长一半为直角边，可获得两个圆心
		double verLen = sqrt(r*r - chordal*chordal/4);
		Point2D ip = p.direction();
		ip.normalize();
		ret.a = p.center() +  ip.turnLeft()*verLen;
		ret.b = p.center() + ip.turnRight()*verLen;
		return 2;
	}
}

// 给定oa-ob这段角度，求卡在这个角内的圆的圆心
// 注意：请保证ob在oa的逆时针方向，且|oa| != |ob|
// 返回值：圆心数量 (0/1)
int Circle::getCenterByAngle(Point2D o, Point2D a, Point2D b, double r, Point2D& ret) {
	Vector2D OA = a - o;
	Vector2D OB = b - o;
	double ad = OA.angle();
	double bd = OB.angle();
	if(bd < ad) {
		bd += 2*PI;
	}
	// 两射线构成的角 >= 180度，则无法放入圆
	if( threeValue(bd - ad - PI) >= 0 ) {
		return 0;
	}
	OA.normalize(), OB.normalize();
	ret = o + (OA - OB).normalize().turnLeft() * r;
	return 1;
}

double Circle::getOriginFanArea(double angle) {
	return (angle/2)*radius*radius;
}

double Circle::getOriginTriangleArea(Point2D A, Point2D B) {
	Vector2D OA = A - center;
	Vector2D OB = B - center;
	int sign = threeValue(OA.X(OB));
	double lenA = OA.len();
	double lenB = OB.len();
	double angleAOB = acos(OA * OB / lenA / lenB);

	if(!sign) {
		// 三角形退化为直线
		return 0;
	}

	Vector2D OH = (B-A).turnLeft();
	line2DTriple lt(A, B);
	double H = lt.getPointDist(center);

	// 情况1：圆心到第三条边（不过圆心那条边）距离大于等于半径。
	if(threeValue(H-radius) >= 0) {
		return sign*getOriginFanArea(angleAOB);
	}

	int lenACmpR = threeValue(lenA - radius);
	int lenBCmpR = threeValue(lenB - radius);
	// 情况2：圆心到第三条边距离小于半径R
	
	// 2.a：OA和OB都小于等于R
	if( lenACmpR <= 0 && lenBCmpR <= 0) {
		return OA.X(OB) / 2;
	}
	// 2.bc：OA和OB都大于等于R
	if( lenACmpR >= 0 && lenBCmpR >= 0) {
		if(OH.sameSide(OA, OB)) {
			// 同边，扇形
			return sign*getOriginFanArea(angleAOB);
		}else {
			// 两边，扇形相减再补上一个三角形
			double angleCOD = 2*acos(H / radius);
			double fanArea = getOriginFanArea(angleAOB-angleCOD);
			double triangleArea = H*radius*sin(angleCOD/2);
			return sign*(fanArea + triangleArea);
		}
	}
	// 2.d：OA和OB 一条大于R，一条小于等于R
	// 让A成为小的那一条
	if(lenA > lenB) {
		Point2D tmp = A; A = B; B = tmp;
		OA = A - center; lenA = OA.len();
		OB = B - center; lenB = OB.len();
	}
	double lenAB = (B-A).len();
	// 余弦定理
	double angleBAO = acos( (lenAB*lenAB+lenA*lenA-lenB*lenB) / 2 / lenAB / lenA );
	// 正弦定理
	double angleACO = asin( sin(angleBAO)/radius*lenA );
	// 三角形内角和
	double angleAOC = PI - angleBAO - angleACO;

	return sign*(getOriginFanArea(angleAOB - angleAOC) + 1/2.0*sin(angleAOC)*radius*lenA);
}

double Circle::intersectWithPolygon(Polygon &poly) {
	// 1.多边形转换成逆时针
	poly.convertToCounterClockwise();

	// 2.面积计算
	double sum = 0;
	for(int i = 0; i < poly.n; ++i) {
		sum += getOriginTriangleArea(poly.p[i], poly.p[i+1]);
	}
	return fabs(sum);
}
