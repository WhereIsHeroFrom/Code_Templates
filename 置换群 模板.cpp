#define MAXL 1010

/*
	置换群模板 - 用于求一个置换群的循环节

	1 2 3 4 5
	4 3 5 1 2
	
	(1 4)(2 3 4)
*/

struct Cyclic {
	int size;
	int a[MAXL];
	int visit[MAXL];

	int getLoopCount() {
		int i, pos, cnt = 0;
		for(i = 1; i <= size; ++i) {
			visit[i] = 0;
		}
		for(i = 1; i <= size; ++i) {
			if(!visit[i]) {
				++cnt;
				visit[i] = 1;
				pos = a[i];
				while(!visit[pos]) {
					visit[pos] = 1;
					pos = a[pos];
				}
			}
		}
		return cnt;
	}
};

