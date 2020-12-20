#include <iostream>
#include <cstring>

using namespace std;

const int MAXN = 7;

bool visit[MAXN];
int ans[MAXN], ansSize;

bool adj[MAXN][MAXN] = {
	{0, 1, 1, 0, 0, 0, 0},
	{1, 0, 0, 1, 1, 0, 0},
	{1, 0, 0, 0, 0, 1, 1},
	{0, 1, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 1, 0},
	{0, 0, 1, 0, 1, 0, 0},
	{0, 0, 1, 0, 0, 0, 0},
};

void dfs_init() {
	memset(visit, 0, sizeof(visit));
	ansSize = 0;
}

void dfs_add(int u) {
	ans[ansSize++] = u;
}

void dfs_print() {
	for(int i = 0; i < ansSize; ++i) {
		printf("%d ", ans[i]);
	}
	puts("");
}

void dfs(int u) {
	if(visit[u]) {
		return ;
	}
	visit[u] = true;
	dfs_add(u);
	for(int i = 0; i < MAXN; ++i) {
		int v = i;
		if(adj[u][v]) {
			dfs(v);
		}
	}
}

int main() {
	dfs_init(); 
	dfs(0);
	dfs_print();
	return 0;
} 


