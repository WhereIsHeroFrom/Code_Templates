#include <iostream>
#include <cmath>
using namespace std;

#define eps 1e-6
#define LL __int64
typedef LL Type;

Type GCD(Type a, Type b) {
	return b==0? a: GCD(b, a%b);
}

// 三值函数
int threeValue(Type d) {
    if(d == 0)
		return 0;
	return d > 0 ? 1 : -1;
}

// 两线段交点类型
enum SegCrossType {
	SCT_NONE = 0,
	SCT_CROSS = 1,      // 正常相交
	SCT_ENDPOINT_ON = 2,   // 其中一条线段的端点在另一条上
};

class Point2D {
public:
    Type x, y;
	Point2D(){
	}
	Point2D(Type _x, Type _y): x(_x), y(_y) {}
	void read() {
		int in_x, in_y;
		scanf("%d %d", &in_x, &in_y);
		x = in_x;
		y = in_y;
	}
	bool operator==(const Point2D& other) const {
		return x == other.x && y == other.y;
	}
	Point2D operator+(const Point2D& other) const {
		return Point2D(x + other.x, y + other.y);
	}
	Point2D operator-(const Point2D& other) const {
		return Point2D(x - other.x, y - other.y);
	}
	// !!!!注意!!!!
	// 如果Type为int，则乘法可能导致int32溢出，小心谨慎
	Type operator*(const Point2D& other) const {
		return x*other.x + y*other.y;
	}
	Type X(const Point2D& other) const {
		return x*other.y - y*other.x;
	}
};

typedef Point2D Vector2D;

class Segment2D {
public:
	// s.x < t.x || s.x == t.x && s.y < t.y 
	Point2D s, t;

	void read() {
		s.read();
		t.read();

		if(s.x > t.x) {
			Point2D tmp = s; s = t; t= tmp;
		}
		if(s.x == t.x) {
			if(s.y > t.y) {
				Point2D tmp = s; s = t; t = tmp;
			}
		}
	}
	
	Vector2D vector() const {
		return t - s;
	}

	// 两线段共线并且相交的情况下
	// 确定是 端点相交 还是 多点覆盖
	bool getCrossEnd(const Segment2D& other, Point2D& ansPt) {
		if(s.x == t.x) {
			// 比较y方向
			if( t.y == other.s.y ) {
				ansPt = t;
				return true;
			}
			if(s.y == other.t.y ) {
				ansPt = s;
				return true;
			}
		}else {
			// 比较x方向
			if( t.x == other.s.x ) {
				ansPt = t;
				return true;
			}
			if(s.x == other.t.x ) {
				ansPt = s;
				return true;
			}
		}
		return false;
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

class fraction {
public:
	// 分数 = u/d
	Type u, d;
	
	fraction() {}
	fraction(Type _u, Type _d) {
		Type g = GCD(_u, _d);
		u = _u / g;
		d = _d / g;
		if(d < 0) {
			u *= -1;
			d *= -1;
		}
	}
	void Print(char c) {
		printf("%I64d", u);
		if(d != 1) {
			printf("/%I64d", d);
		}
		printf("%c", c);
	}
};

class line2DTriple {
public:
	// ax + by + c = 0;
	Type a, b, c;
	void getFromSegment2D(const Segment2D&);
	void getCrossPoint(const line2DTriple& otherLine, fraction& X, fraction& Y);

	fraction getXByY(fraction Y) const;

	void Print() {
		printf("%I64dx + %I64dy + %I64d = 0\n", a, b, c);
	}
};

void line2DTriple::getFromSegment2D(const Segment2D& seg) {
	Vector2D v = seg.vector();
	a = - v.y;
	b = v.x;
	c = seg.s.X(seg.t);

	Type g = GCD(a, b);
	if(c % g == 0) {
		a /= g;
		b /= g;
		c /= g;
	}

	//Print();
}

fraction line2DTriple::getXByY(fraction Y) const {
	return fraction(Y.d*c + Y.u*b, -Y.d*a);
}

void line2DTriple::getCrossPoint(const line2DTriple& otherLine, fraction& X, fraction& Y) {
	if(a == 0) {
		Y = fraction(-c, b);
	}else {
		// 请确保 this->a 和 otherLine->a 均不是0
		Y = fraction(- otherLine.c*a + c*otherLine.a, otherLine.b*a - b*otherLine.a);
	}	
	X = otherLine.getXByY(Y);
}

void solve(Segment2D& A, Segment2D& B) {
	SegCrossType scType = A.segCross(B);
	Vector2D Zero(0, 0);

	// 0交点
	if(scType == SCT_NONE) {
		printf("0\n");
		return ;
	}
	// 处理共线
	if (scType == SCT_ENDPOINT_ON) {
		// 公共交点，则相交
		// A和B叉乘==0，代表平行
		// 相交 + 平行 = 共线
		if(threeValue(A.vector().X(B.vector())) == 0) {
			Point2D p;
			int ans = -1;
			if( A.vector() == Zero ) {
				p = A.s;
				ans = 1;
			}else if(B.vector() == Zero) {
				p = B.s;
				ans = 1;
			}else if( A.getCrossEnd(B, p) ) {
				ans = 1;
			}else {
				ans = -1;
			}
			if(ans == -1)
				printf("INF\n");
			else {
				printf("%d\n%I64d %I64d\n", ans, p.x, p.y);
			}
			return ;
		}
	}
	printf("1\n");
	line2DTriple LA, LB;
	LA.getFromSegment2D(A);
	LB.getFromSegment2D(B);
	fraction X, Y;
	if(LA.a == 0) {
		LA.getCrossPoint(LB, X, Y);
	}else if(LB.a == 0) {
		LB.getCrossPoint(LA, X, Y);
	}else {
		LA.getCrossPoint(LB, X, Y);
	}
	X.Print(' ');
	Y.Print('\n');
}

int main() {
	int t;
	Segment2D A, B;
	scanf("%d", &t);

	while(t--) {
		A.read();
		B.read();
		solve(A, B);
	}
	return 0;
}