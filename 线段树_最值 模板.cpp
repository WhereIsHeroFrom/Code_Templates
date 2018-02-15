#include <iostream>

using namespace std;

#define MAXN 100010
#define MAXNODES (1<<18)

/////////////////////////////////////////////////////
//////////////////线段树-区间最值//////////////////// 
/////////////////////////////////////////////////////

typedef int TypeVal;
struct Node {
	Node* son[2];
	TypeVal min, max;
	
	Node() {
		reset();
	} 
	
	void reset() {
		min =  100000000;
		max = -100000000;
		son[0] = son[1] = NULL;
	}
	
	void update(TypeVal v) {
		if(v < min) {
			min = v;
		}
		if(v > max) {
			max = v;
		}
	}
	
	void update(Node* val) {
		if(val->min < min) {
			min = val->min;
		}
		if(val->max > max) {
			max = val->max;
		}
	}
};
typedef Node* NodePtr;

class SegTree {
	NodePtr nodes;
	NodePtr treeRoot;
	int nodeCount;
	int intervalLength;
	
	NodePtr get_node(); 
	void insert(NodePtr&, int, int, int, int, TypeVal);
	void query(NodePtr&, int, int, int, int, NodePtr);
public:
	
	SegTree () {
		// MAXNODES为比MAXN大，且最小2的幂的两倍 
		nodes = new Node[MAXNODES];
	}
	
	~SegTree() {
		delete [] nodes; 
	} 
	void build_tree(int len);
	void insert_tree(int l, int r, TypeVal val);
	void query_tree(int l, int r, NodePtr ans);
	
	static SegTree& Instance() {
		static SegTree inst;
		return inst;
	}
};

NodePtr SegTree::get_node() {
	return &nodes[nodeCount++];
}

/*
	将区间[l, r]插入到 以root为根的子树（区间为 [treeL, treeR]）  
*/
void SegTree::insert(NodePtr& root, int treeL, int treeR, int l, int r, TypeVal val) {
	// 区间无交集，非法插入 
	if(treeR < l || r < treeL) {
		return ;
	}
	// 如果这个结点未定义，则生成新结点 
	if(root == NULL) {
		root = get_node();
		root->reset();
	}
	// 更新最优解 
	root->update(val);
	
	// 叶子结点 
	if(treeL == l && treeR == r) {
		return ;
	}
	
	int treeMid = (treeL + treeR) >> 1;
	insert(root->son[0], treeL, treeMid, l, r, val);
	insert(root->son[1], treeMid+1, treeR, l, r, val);
}

void SegTree::query(NodePtr& root, int treeL, int treeR, int l, int r, NodePtr ans) {
	// 区间无交集，非法插入 
	if(treeR < l || r < treeL) {
		return ;
	}
	// 当前区间中无数据，无法更新 
	if(root == NULL) {
		return ;
	}
	//if(root->max <= ans->max && root->min >= ans->min) {
	//	return ;
	//}
	
	// 完全包含，更新最值 
	if(l <= treeL && treeR <= r) {
		ans->update(root);
		return ;
	}
	int treeMid = (treeL + treeR) >> 1;
	query(root->son[0], treeL, treeMid, l, r, ans);
	query(root->son[1], treeMid+1, treeR, l, r, ans);
}

void SegTree::build_tree(int len) {
	intervalLength = len;
	nodeCount = 0;
	treeRoot = NULL;
}

void SegTree::insert_tree(int l, int r, TypeVal val) {
	insert(treeRoot, 1, intervalLength, l, r, val);
}

void SegTree::query_tree(int l, int r, Node* ans) {
	query(treeRoot, 1, intervalLength, l, r, ans);
}

int n, m;

int main() {
	int i;
	while(scanf("%d %d", &n, &m) != EOF) {
		SegTree& st = SegTree::Instance();
		st.build_tree(n);
		for(i = 1; i <= n; i++) {
			int x;
			scanf("%d", &x);
			st.insert_tree(i, i, x);
		}
		while(m--) {
			int x, y;
			scanf("%d %d", &x, &y);
			if(x > y) {
				int tmp = x; x = y; y = tmp;
			}
			Node node;
			st.query_tree(x, y, &node);
			printf("%d\n", node.max - node.min); 
		}
	}
	return 0;
} 
