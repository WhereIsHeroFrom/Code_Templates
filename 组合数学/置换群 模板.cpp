#define MAXL 100010

/*
	置换群模板 - 用于求一个置换群的循环节

	1 2 3 4 5
	4 3 5 1 2
	
	(1 4)(2 3 5)
	
	
	1 2 3 4 5 6 7
	4 2 6 7 1 3 5
	
	(1 4 7 5)(2)(3 6)
*/

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Cyclic {
	int size;
	int a[MAXL];
	int visit[MAXL];
	vector <int> ans[MAXL]; 
	
	struct Pair {
		int index;
		int head;
		Pair() {
		}
		Pair(int i, int h) : index(i), head(h) {
		}
		
		bool operator < (const Pair &p ) const {
			return head < p.head;
		}
		
	}anshead[MAXL];
	
	void read(int n) {
		for(int i = 1; i <= n; ++i) {
			scanf("%d", &a[i]);
		}
		size = n;
	}

	int getLoopCount() {
		int i, pos, cnt = 0;
		for(i = 1; i <= size; ++i) {
			visit[i] = 0;
			ans[i].clear();
		}
		for(i = 1; i <= size; ++i) {
			if(!visit[i]) {
				++cnt;
				pos = i;
				
				while(!visit[pos]) {
					visit[pos] = cnt;
					ans[cnt].push_back(pos);
					pos = a[pos];
				}
			}
		}
		return cnt;
	}
	
	void outputLoop() {
		int i, j;
		int csize = getLoopCount();

		for(i = 1; i <= csize; ++i) {
			anshead[i] = Pair( i, ans[i][0] );
		}
		sort(anshead+1, anshead+csize+1);
		
		for(i = 1; i <= csize; ++i) {
			vector<int> &p = ans[ anshead[i].index ];
			printf("(");
			for(j = 0; j < p.size(); ++j) {
				if(j) printf(" ");
				printf("%d", p[j]);
			}
			printf(")");
		}
		puts("");
		
	}
}C;
