/*
蜂巢坐标系 模板
      记当前格子的编号记为(r, c)：
      1）行列数目。行数为2N-1，列数和行号（从0开始数）强相关。令行号r<N，则列数为r+N；否则，列数为(3N-2)-r。
      2）合法区域。如何判断一个坐标(x, y)是否在合法区域内，先判x，x的范围为[0, 2N-1)；再判y，y的范围为[0, 列数)，列数的计算参照1）。
      3）6个邻居坐标。
              a.左右邻居。   (r,c-1)和(r,c+1)。
              b.上排邻居。   行号r<N时，上排邻居坐标(r-1,c-1)和(r-1,c)；否则，上排邻居坐标(r-1,c)和(r-1,c+1)。
              c.下排邻居。    行号r<N-1时，下排邻居坐标(r+1,c)和(r-1,c+1)；否则，下排邻居坐标(r+1,c-1)和(r+1,c)。
              注：以上邻居坐标都需要通过2）的合法区域坐标检测。
       4）N = 6时的格子总数为1 + 6 + 12 + 18 + 24 + 30 = 91。 (每次加一圈，一圈个数为  6*(N-1))
Author: WhereIsHeroFrom
Update Time: 2018-3-29
Algorithm Complexity: O(N^2)
*/

struct HoneycombNode {
	int r, c;
	HoneycombNode() {}
	HoneycombNode(int _r, int _c) {
		r = _r;
		c = _c;
	}

	HoneycombNode left() {
		return HoneycombNode(r, c-1);
	}

	HoneycombNode right() {
		return HoneycombNode(r, c+1);
	}
};

// 蜂巢坐标系
#define MAXH 6

class HoneycombAxis {
	int n;
	int id[2*MAXH][2*MAXH];
	int idCount;
	HoneycombNode nodes[4*MAXH*MAXH];

public:
	void init(int _n) {
		n = _n;
		int i, j;
		idCount = 0;
		for(i = 0; i < getRowCount(); ++i) {
			for(j = 0; j < getColCount(i); ++j) {
				id[i][j] = idCount;
				nodes[idCount] = HoneycombNode(i, j);
				++idCount;
			}
		}
	}

	// 返回行数
	int getRowCount() {
		return 2*n - 1;
	}

	// 返回列数
	int getColCount(int r) {
		if(r < n) {
			return n + r;
		}else {
			return 3*n - 2 - r;
		}
	}

	// 返回格子个数
	int getIDCount() {
		return idCount;
	}

	// 返回格子连续编号
	int getNodeID(HoneycombNode& node) {
		return id[node.r][node.c];
	}

	// 通过ID返回格子
	HoneycombNode getNode(int id) {
		return nodes[id];
	}

	// 返回当前格子的编号是否合法
	bool isValid(HoneycombNode& node) {
		// 超行
		if(node.r < 0 || node.r >= getRowCount()) {
			return false;
		}
		// 超列
		if(node.c < 0 || node.c >= getColCount(node.r)) {
			return false;
		}
		return true;
	}

	void getNeighbors(HoneycombNode now, int &retSize, HoneycombNode *ret) {
		HoneycombNode tmp[6];
		int i;
		
		// 本行邻居
		tmp[0] = now.left();
		tmp[1] = now.right();
		
		// 上排邻居
		if(now.r < n) {
			tmp[2] = HoneycombNode(now.r-1, now.c-1);
		}else {
			tmp[2] = HoneycombNode(now.r-1, now.c);
		}
		tmp[3] = tmp[2].right();

		// 下排邻居
		if(now.r < n-1) {
			tmp[4] = HoneycombNode(now.r+1, now.c);
		}else {
			tmp[4] = HoneycombNode(now.r+1, now.c-1);
		}
		tmp[5] = tmp[4].right();


		retSize = 0;
		for(i = 0; i < 6; ++i) {
			if( isValid( tmp[i] ) ) {
				ret[retSize++] = tmp[i];
			}
		}
	}
};
