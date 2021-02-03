#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdlib>
using namespace std;

//************************* 计算几何 模板 *************************
// 包含：
// 1、凸包
// 2、线段判交
// 3、点在多边形内
// 4、多边形交判定

const int MAXP = 2010;
const double eps = 1e-10;
#define INT_POINT

// 两线段交点类型
enum SegCrossType {
    SCT_NONE = 0,
    SCT_CROSS = 1,         // 正常相交
    SCT_ENDPOINT_ON = 2,   // 其中一条线段的端点在另一条上
};

#ifdef INT_POINT
typedef int PointType;
typedef long long BigPointType;    // 由于乘法可能导致 int 溢出，所以需要定义一种乘法后的类型(平方、叉乘、点乘)
#else
typedef double PointType;
typedef double BigPointType;
#endif
typedef int PointIndex;

// 小于
bool ST(BigPointType a, BigPointType b) {
#ifdef INT_POINT
    return a < b;
#else
    return a - b < -eps;
#endif
}

// 等于
bool EQ(BigPointType a, BigPointType b) {
#ifdef INT_POINT
    return a == b;
#else
    return fabs(a - b) < eps;
#endif
}

// 大于
bool LT(BigPointType a, BigPointType b) {
    return !ST(a, b) && !EQ(a, b);
}

int TernaryFunc(double v) {
    if (EQ(v, 0)) {
        return 0;
    }
    return ST(v, 0) ? -1 : 1;
}

BigPointType SQR(BigPointType x) {
    return x * x;
}

class Point2D {
public:
    Point2D() : x_(0), y_(0) {}
    Point2D(PointType x, PointType y) : x_(x), y_(y) {}

    bool zero() const;
    Point2D operator + (const Point2D& pt) const;
    Point2D operator - (const Point2D& pt) const;
    BigPointType operator * (const Point2D& pt) const;
    BigPointType cross(const Point2D& pt) const;
    bool sameLine(const Point2D& l, const Point2D& r) const;
    bool operator < (const Point2D& pt) const;
    bool operator == (const Point2D& pt) const;


    BigPointType distSquare(const Point2D& pt) const;
    static bool angleCmp(const Point2D& a, const Point2D& b);
    void calculateAngle(const Point2D& o);

    void read(int idx);
    void print();
    PointType x() const;
    PointType y() const;
    double getAngle() const;
    int getIndex() const;

    Point2D getMinusYPoint() const;
private:
    PointType x_, y_;
    double angle_;            // 相对于左下角点的极角
    double distSqr_;          // 相对于左下角点的距离平方
    int index_;               // 在原数组的下标，方便索引用
};
typedef Point2D Vector2D;

bool Point2D::zero() const {
    return EQ(x_, 0) && EQ(y_, 0);
}

Point2D Point2D::operator + (const Point2D& pt) const {
    return Point2D(x_ + pt.x_, y_ + pt.y_);
}

Point2D Point2D::operator - (const Point2D& pt) const {
    return Point2D(x_ - pt.x_, y_ - pt.y_);
}

BigPointType Point2D::operator * (const Point2D& pt) const {
    return x_ * pt.x_ + y_ * pt.y_;
}

bool Point2D::sameLine(const Point2D& l, const Point2D& r) const {
    // 三点共线判定
    BigPointType crossRet = (*this - l).cross(*this - r);
    return TernaryFunc(crossRet) == 0;
}

BigPointType Vector2D::cross(const Vector2D& pt) const {
    return (BigPointType)x_ * pt.y_ - (BigPointType)y_ * pt.x_;
}

bool Point2D::operator<(const Point2D& pt) const {
    // 1. 第一关键字： y 小的
    // 2. 第二关键字： x 小的
    // 3. 第三关键字：下标小的 - 适用于重复点
    if (!EQ(y_, pt.y_)) {
        return ST(y_, pt.y_);
    }
    if (!EQ(x_, pt.x_)) {
        return ST(x_, pt.x_);
    }
    return index_ < pt.index_;
}

bool Point2D::operator==(const Point2D& pt) const {
    return (*this - pt).zero();
}

BigPointType Point2D::distSquare(const Point2D& pt) const {
    Point2D t = *this - pt;
    return SQR(t.x_) + SQR(t.y_);
}

bool Point2D::angleCmp(const Point2D& a, const Point2D& b) {
    if (fabs(a.angle_ - b.angle_) > eps) {
        return a.angle_ < b.angle_;
    }
    if (fabs(a.distSqr_ - b.distSqr_) > eps) {
        return a.distSqr_ < b.distSqr_;
    }
    return a.index_ < b.index_;
}

void Point2D::calculateAngle(const Point2D& o) {
    Point2D t = *this - o;
    if (t.zero()) {
        // 该情况下 atan2 是 undefined 的，需要单独处理
        angle_ = 0;
        distSqr_ = 0;
    }
    else {
        angle_ = atan2(0.0 + t.y_, 0.0 + t.x_); // 这里 y >= 0 是能保证的，所以值在 [0, PI) 之间
        distSqr_ = distSquare(o);
    }
}

void Point2D::read(int idx) {
#ifdef INT_POINT
    scanf("%d %d", &x_, &y_);
#else
    scanf("%lf %lf", &x_, &y_);
#endif
    index_ = idx;
}

void Point2D::print() {
#ifdef INT_POINT
    printf("%d %d\n", x_, y_);
#else
    printf("%lf %lf", x_, y_);
#endif
}

PointType Point2D::x() const {
    return x_;
}

PointType Point2D::y() const {
    return y_;
}

double Point2D::getAngle() const {
    return angle_;
}

int Point2D::getIndex() const {
    return index_;
}

Point2D Point2D::getMinusYPoint() const
{
    return Point2D(x_, -y_);
}

class Segment2D {
public:
    Segment2D(){}
    Segment2D(Point2D from, Point2D to) : from_(from), to_(to) {}
    BigPointType cross(const Point2D& p) const;
    bool lineCross(const Segment2D& other) const;
    bool pointOn(const Point2D& p) const;
    SegCrossType segCross(const Segment2D& other);
private:
    Point2D from_, to_;
};

// 定点叉乘
// 外部找一点p，然后计算 (p-s) X (t-s)
BigPointType Segment2D::cross(const Point2D& p) const {
    return (p - from_).cross(to_ - from_);
}

// 跨立测验
// 将当前线段作为一条很长的直线，检测线段other是否跨立在这条直线的两边
bool Segment2D::lineCross(const Segment2D& other) const {
    return TernaryFunc(cross(other.from_)) * TernaryFunc(cross(other.to_)) == -1;
}

// 点是否在线段上
bool Segment2D::pointOn(const Point2D& p) const {
    // 满足两个条件：
    //  1.叉乘为0，    (p-s)×(t-s) == 0
    //  2.点乘为-1或0，(p-s)*(p-t) <= 0
    return TernaryFunc(cross(p)) == 0 && TernaryFunc((p - from_) * (p - to_)) <= 0;
}

// 线段判交
// 1.通过跨立测验
// 2.点是否在线段上
SegCrossType Segment2D::segCross(const Segment2D& other) {
    if (this->lineCross(other) && other.lineCross(*this)) {
        // 两次跨立都成立，则必然相交与一点
        return SCT_CROSS;
    }
    // 任意一条线段的某个端点是否在其中一条线段上，四种情况
    if (pointOn(other.from_) || pointOn(other.to_) ||
        other.pointOn(from_) || other.pointOn(to_)) {
        return SCT_ENDPOINT_ON;
    }
    return SCT_NONE;
}


class Polygon {
private:
    void grahamScan_Pre();   // 计算凸包前的准备工作
    void grahamScan_Post(bool flag, Polygon& ret);  // 填充凸包的点到给定的多边形
public:
    bool isPoint() const;                      // 求完凸包以后判断是否是一个点
    bool isLine() const;                       // 求完凸包以后判断是否是一条线
    bool isPointIn(const Point2D& pt) const;   // 判断点是否在多边形内；
    bool isIntersectWithPolygon(Polygon &o);   // 判断是否和另个一多边形有交集；
    void grahamScan(bool flag, Polygon& ret);
    double area();
    double length();
    int size();
    void clear();
    void addPoint(const Point2D& pt);
public:
    int dp();       // 根据不同情况提供的开放接口
private:
    int n_;
    Point2D point_[MAXP];
    bool hash_[MAXP];                        // 凸包上的点哈希
    PointIndex stack_[MAXP];
    int top_;
};

bool Polygon::isPoint() const  {
    if (n_ <= 1) {
        return true;
    }
    return point_[n_ - 1] == point_[0];
}

bool Polygon::isLine() const {
    if (n_ <= 2) {
        return true;
    }
    return (TernaryFunc((point_[n_ - 1] - point_[0]).cross(point_[1] - point_[0])) == 0);
}


bool Polygon::isPointIn(const Point2D& pt) const {
    Point2D inf(12316543, 132543876);
    Segment2D judge(pt, inf);
    int isIn = 0;
    for (int i = 0; i < n_; ++i) {
        SegCrossType tp = Segment2D(point_[i], point_[(i + 1) % n_]).segCross(judge);
        if (tp == SCT_CROSS) {
            isIn ^= 1;
        }
        else if (tp == SCT_ENDPOINT_ON) {
            // 在多边形的一条边上
            return true;
        }
    }
    return isIn;
}

bool Polygon::isIntersectWithPolygon(Polygon& o) {
    // 1. 两个多边形的边有交集，返回 false
    for (int i = 0; i < n_; ++i) {
        Segment2D srcSeg(point_[i], point_[i + 1]);
        for (int j = 0; j < o.n_; ++j) {
            Segment2D tarSeg(o.point_[j], o.point_[j + 1]);
            if (srcSeg.segCross(tarSeg) != SCT_NONE) {
                return false;
            }
        }
    }

    // 2. 一个多边形的点在另一个多边形内
    for (int i = 0; i < n_; ++i) {
        if (o.isPointIn(point_[i])) {
            return false;
        }
    }

    for (int i = 0; i < o.n_; ++i) {
        if (isPointIn(o.point_[i])) {
            return false;
        }
    }

    return true;
}

void Polygon::grahamScan_Pre()
{
    // 1. 首先将最下面的那个点（如果y相同，则取最左边）找出来放到 point_[0] 的位置
    for (int i = 1; i < n_; ++i) {
        if (point_[i] < point_[0]) {
            swap(point_[i], point_[0]);
        }
    }
    // 2. 对 point_[0] 计算极角
    for (int i = 1; i < n_; ++i) {
        point_[i].calculateAngle(point_[0]);
    }
    // 3. 极角排序
    sort(point_ + 1, point_ + n_, Point2D::angleCmp);

    // 4. 去重
    int tmpn = 0;
    for (int i = 0; i < n_; ++i) {
        if (tmpn && point_[tmpn - 1] == point_[i]) {
            // 重复点
        }
        else {
            point_[tmpn++] = point_[i];
        }
    }
    n_ = tmpn;

    // 5. 构成回路
    point_[n_] = point_[0];
}


void Polygon::grahamScan_Post(bool flag, Polygon& ret) {
    // 1. 标记所有凸包上的点
    memset(hash_, 0, sizeof(hash_));
    for (int i = 0; i < top_; ++i) {
        hash_[stack_[i]] = true;
    }

    // 2. 开始插入点集
    int preidx = -1;
    ret.clear();
    for (int i = 0; i < n_; ++i) {
        if (hash_[i]) {
            if (flag) {
                // 2.a 插入 (preidx, i) 中的共线点
                if (preidx != -1) {
                    for (int j = preidx + 1; j < i; ++j) {
                        bool bSameLine = point_[j].sameLine(point_[preidx], point_[i]);
                        if (bSameLine) {
                            ret.addPoint(point_[j]);
                        }
                    }
                }
            }
            // 2.b 插入凸包上的点
            ret.addPoint(point_[i]);
            preidx = i;
        }
    }
    if (ret.isPoint() || ret.isLine()) {
        return;
    }

    if (flag) {
        for (int i = n_ - 1; i >= 1; --i) {
            if (point_[i].getIndex() == ret.point_[ret.n_ - 1].getIndex()) {
                continue;
            }
            bool bSameLine = point_[i].sameLine(ret.point_[0], ret.point_[ret.n_ - 1]);
            if (bSameLine) {
                ret.addPoint(point_[i]);
            }
        }
    }
    // 3. 构成回路
    ret.point_[ret.n_] = ret.point_[0];
}

// flag 是否算上边上的点、重复点
void Polygon::grahamScan(bool flag, Polygon& ret) {

    // 1. 找到极值坐标系原点，并且按照极角排序
    grahamScan_Pre();

    // 2. 栈底永远是那个极值坐标系的原点
    top_ = 0;
    stack_[top_++] = 0;

    for (int i = 1; i < n_; ++i) {
        while (top_ >= 2) {
            Point2D p1 = point_[stack_[top_ - 1]] - point_[stack_[top_ - 2]];
            Point2D p2 = point_[i] - point_[stack_[top_ - 2]];
            BigPointType crossRet = p1.cross(p2);
            // 叉乘结果等于 0 的情况：
            // 1、有重复点
            // 2、三点共线
            // 都视为不合法，进行出栈操作，grahamScan_Post 中进行特殊处理
            if (TernaryFunc(crossRet) <= 0)
                --top_;
            else
                break;
        }
        stack_[top_++] = i;
    }

    grahamScan_Post(flag, ret);
}

double Polygon::area() {
    double ans = 0;
    point_[n_] = point_[0];
    for (int i = 1; i < n_; ++i) {
        ans += (point_[i] - point_[0]).cross(point_[i + 1] - point_[0]);
    }
    return ans / 2;
}

double Polygon::length() {
    if (n_ == 1) {
        return 0;
    }
    else if (n_ == 2) {
        return sqrt(0.0 + point_[1].distSquare(point_[0])) * 2;
    }
    double ans = 0;
    point_[n_] = point_[0];
    for (int i = 0; i < n_; ++i) {
        ans += sqrt(0.0 + point_[i].distSquare(point_[i + 1]));
    }
    return ans;
}

int Polygon::size() {
    return n_;
}

void Polygon::clear() {
    n_ = 0;
    top_ = 0;
}

void Polygon::addPoint(const Point2D& pt) {
    point_[n_++] = pt;
}

//************************* 计算几何 模板 *************************

const int MAXN = 110;
const int inf = 0;
int Dp[MAXN][MAXN], opt[MAXN][MAXN];

int Max(int a, int b) {
    if (a == inf) return b;
    if (b == inf) return a;
    return a > b ? a : b;
}

int Polygon::dp()
{
    grahamScan_Pre();
    memset(opt, inf, sizeof(opt));
    memset(Dp, inf, sizeof(Dp));

    int ans = inf;
    int i, j, k;
    for (j = 1; j < n_; ++j) {
        i = j - 1;
        // 1. 处理 Oij 共线的情况
        while (i >= 1 && point_[0].sameLine(point_[i], point_[j])) {
            --i;
        }
        k = i - 1;
        while (k >= 0) {
            // 2. 处理 Oki 共线
            if (point_[0].sameLine(point_[k], point_[i])) {
                Dp[i][j] = (point_[i] - point_[0]).cross(point_[j] - point_[0]);
                ans = Max(Dp[i][j], ans);
                i = k;
                k = i - 1;
                continue;
            }

            // 3. 保证 ki X kj > 0
            BigPointType crossRet = Segment2D(point_[k], point_[j]).cross(point_[i]);
            if (crossRet >= 0) {
                Dp[i][j] = (point_[i] - point_[0]).cross(point_[j] - point_[0]) + opt[k][i];
                ans = Max(Dp[i][j], ans);
                // 4. 保证多边形内没有点
                i = k;
                k = i - 1;
            }
            else {
                --k;
            }
        }
        opt[0][j] = Dp[0][j] = 0;
        for (k = 1; k < j; ++k) {
            opt[k][j] = Max(opt[k - 1][j], Dp[k][j]);
        }
    }
    return ans;
}


Polygon P, Res;
int t;
Point2D Pt[210];
int cas;

int main() {
    int t;
    int n;
    Point2D pt;
    double l;
    cas = 0;
    scanf("%d", &t);

    while (t--) {
        int ans = 0;
        scanf("%d", &n);
        int Max = 0;
        for (int i = 0; i < n; ++i) {
            Pt[i].read(i);
        }

        for (int i = 0; i < n; ++i) {
            P.clear();
            for (int j = 0; j < n; ++j) {
                if (Pt[j] < Pt[i]) {

                }
                else {
                    P.addPoint(Pt[j]);
                }
            }
            int ans = P.dp();
            if (ans > Max) Max = ans;
        }
        printf("%.1lf\n", Max / 2.0);
    }
    return 0;
}

/*
100000

11
12 2
3 3
22 3
8 4
23 5
9 6
24 7
6 10
27 12
18 12
13 13
*/