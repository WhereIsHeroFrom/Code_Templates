#include <iostream>
#include <cmath>
using namespace std;

#define eps 1e-6
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
	SCT_CROSS = 1,      // 正常相交
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
	Type X(const Point2D& other) {
		return x*other.y - y*other.x;
	}
};

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

Segment2D seg[1100];

int main() {
	int i, j, n, cnt;
	while( scanf("%d",&n) != EOF && n) {
		for(i = 0; i < n; ++i) {
			seg[i].read();
		}
        cnt = 0;
        for(i = 0; i < n; i++) {
			for(j = i+1; j < n; ++j) {
				if(seg[i].segCross(seg[j]) != SCT_NONE) {
					++cnt;
				}
			}
        }
        printf("%d\n",cnt);
    }
}