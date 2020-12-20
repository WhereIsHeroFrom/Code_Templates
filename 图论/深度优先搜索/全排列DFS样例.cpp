#include <iostream>
#include <cstring>

using namespace std;

#define MAXN 4
 
bool visit[MAXN + 1];
int ans[MAXN], ansSize;

void dfs_init() {
	memset(visit, 0, sizeof(visit));
	ansSize = 0;
}

void dfs_add(int u) {
	visit[u] = true;
	ans[ansSize] = u;
	++ansSize;
}

void dfs_dec(int u) {
	--ansSize;
	visit[u] = false;
}

void dfs_print() {
	for(int i = 0; i < ansSize; ++i) {
		printf("%d ", ans[i]);
	}
	puts("");
}

void dfs(int depth) {
	if(depth == MAXN) {
		dfs_print();
		return;
	}
	for(int i = 1; i <= MAXN; ++i) {
		int v = i;
		if(!visit[v]) {
			dfs_add(v);
			dfs(depth+1);
			dfs_dec(v);
		}
		
	}
}

int main() {
	dfs_init();
	dfs(0);
	return 0;
} 
