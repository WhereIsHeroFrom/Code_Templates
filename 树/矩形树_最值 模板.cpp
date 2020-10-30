#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

#define inf -100000000
#define MAXN 1010
#define MAXNODES (MAXN*30) 

// 最值信息 
struct dataInfo {
	int x, y;
	int val;
	dataInfo() {
		x = y = val = inf;
	}
	dataInfo(int _x, int _y, int _val) : x(_x), y(_y), val(_val) {
	}
	void update(dataInfo &d) {
		if(d.val > val) {
			*this = d;
		}
	}

	// 当前值比传入值更优 
	bool isOptimal(const dataInfo &d) {
		return val >= d.val;
	} 
};

// 矩形树结点信息
struct treeNode {
	treeNode *son[4];
	dataInfo data;
	treeNode() {
		reset();
	}
	void reset() {
		son[0] = son[1] = son[2] = son[3] = NULL;
		data = dataInfo(inf, inf, inf);
	}
	void update(dataInfo &d) {
		data.update(d);
	}
};
typedef treeNode* treeNodePtr;

// 区间类 
struct Interval {
    int l, r;
    Interval() {}
    Interval(int _l, int _r): l(_l), r(_r) {}
    int mid();                                       // 区间中点 
    int len();                                       // 区间长度 
    Interval left();                                 // 左半区间 
    Interval right();                                // 右半区间 
    bool isIntersectWith( const Interval& other );   // 区间判交
    bool isInclude( const Interval& other );         // 区间判包含
    bool isInclude(int v);
};

int Interval::mid() {
    return (l + r) >> 1;
}
int Interval::len() {
    return r - l + 1;
}
Interval Interval::left() {
    return Interval(l, mid());
}
Interval Interval::right() {
    return Interval(mid()+1, r);
}
bool Interval::isIntersectWith( const Interval& other ) {
    return !( l > other.r || r < other.l );
} 
bool Interval::isInclude( const Interval& other ) {
    return l <= other.l && other.r <= r;
}
bool Interval::isInclude(int v) {
    return l <= v && v <= r;
}
    
class segTree2D {
	treeNodePtr root;
	treeNodePtr nodes;
	int nodeCount;	
	Interval xBound, yBound;

	treeNodePtr getNode();
	void insert(treeNodePtr& root, Interval xInt, Interval yInt, dataInfo insertVal);
	void query(treeNodePtr root, Interval xQry, Interval yQry, Interval xInt, Interval yInt, dataInfo& ansVal);
public:
	segTree2D() {
		nodes = new treeNode[MAXNODES];
	}
	~segTree2D() {
		delete [] nodes;
	}

	void build_tree(Interval xBnd, Interval yBnd);                     // 传入边界 
	void insert_tree(dataInfo insertVal);                              // 插入数据 
	void query_tree(Interval xQry, Interval yQry, dataInfo& ansVal);   // 询问区间最值 
	
	
};

treeNodePtr segTree2D::getNode() {
	treeNodePtr p = &nodes[nodeCount++];
	p->reset();
	return p;
}

void segTree2D::insert(treeNodePtr& root, Interval xInt, Interval yInt, dataInfo insertVal) {
	// 插入位置不在该矩形区域内 
	if( !xInt.isInclude(insertVal.x) || !yInt.isInclude(insertVal.y) ){
		return ;
	}
	// 结点未定义，则说明在此之前没有插入相关结点，生成新结点 
	if( root == NULL ) {
		root = getNode();
	}
	// 更新当前根结点最值信息 
	root->update(insertVal);
	
	// 叶子结点 
	if( xInt.len() == 1 && yInt.len() == 1) {
		return ;
	}
	// 递归子结点 
	insert(root->son[0], xInt.left(),   yInt.left(), insertVal);
	insert(root->son[1], xInt.right(),  yInt.left(), insertVal);
	insert(root->son[2], xInt.left(),  yInt.right(), insertVal);
	insert(root->son[3], xInt.right(), yInt.right(), insertVal);
} 

void segTree2D::query(treeNodePtr root, Interval xQry, Interval yQry, Interval xInt, Interval yInt, dataInfo& ansVal) {
	// 询问区间 和 当前矩形树表示区间无交集，直接返回
	if( !xQry.isIntersectWith(xInt) || !yQry.isIntersectWith(yInt) ) {
		return ;
	}
	// 当前结点尚未初始化，说明这块区域必然没有数据 
	if( root == NULL ) {
		return ;
	}

	// 重要剪枝！！！ 当前值更优于树根结点的值，则不需要继续往下遍历 
	if(ansVal.isOptimal(root->data)) {
		return ;
	}
	
	// 区间完全包含，直接更新最值，并返回 
	if(xQry.isInclude(xInt) && yQry.isInclude(yInt)) {
		ansVal.update( root->data );
		return ;
	}
	// 递归子结点 
	query(root->son[0], xQry, yQry, xInt.left(),   yInt.left(), ansVal);
	query(root->son[1], xQry, yQry, xInt.right(),  yInt.left(), ansVal);
	query(root->son[2], xQry, yQry, xInt.left(),  yInt.right(), ansVal);
	query(root->son[3], xQry, yQry, xInt.right(), yInt.right(), ansVal);
}

void segTree2D::build_tree(Interval xBnd, Interval yBnd) {
	nodeCount = 0;
	root = NULL;
	xBound = xBnd;
	yBound = yBnd;
}

void segTree2D::insert_tree(dataInfo insertVal) {
	insert(root, xBound, yBound, insertVal);
}

void segTree2D::query_tree(Interval xQry, Interval yQry, dataInfo& ansVal) {
	query(root, xQry, yQry, xBound, yBound, ansVal);
} 


