#include <iostream>
#include <cstring>
#include <queue>
using namespace std;

#define MAXN 20
#define inf -1

int adj[MAXN][MAXN];
int n;
int dis[MAXN];

void bfs(int u) {
	queue <int> q;
	memset(dis, inf, sizeof(dis));
	
	dis[u] = 0;
	q.push(u);
	
	while(!q.empty()) {
		u = q.front();
		q.pop();
		
		for(int v = 1; v <= n; ++v) {
			if(!adj[u][v]) continue;
			if(dis[v] != inf) continue;
			
			dis[v] = dis[u] + 1;
			q.push(v);
		}
	}
}

void bfs_printf() {
	for(int i = 1; i <= n; ++i) {
		printf("dis[%d] = %d\n", i, dis[i]);
	}
	
} 

int main() {
	int m;
	while(scanf("%d %d", &n, &m) != EOF) {
		memset(adj, 0, sizeof(adj));
		while(m--) {
			int u, v;
			scanf("%d %d", &u, &v);
			adj[u][v] = adj[v][u] = 1;
		}
		bfs(1);
		bfs_printf();
	}
	return 0;
}

/*

10 11
2 1
1 3
1 4
1 5
7 1
10 2
10 8
8 7
7 9
4 5
5 6

*/ 
