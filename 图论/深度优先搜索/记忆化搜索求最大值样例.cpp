#include <iostream>
#include <cstring>

using namespace std;

const int MAXN = 6;
int D[MAXN][MAXN];

int gold[MAXN][MAXN] = {
	{0, 0, 0, 1, 0, 0},
	{0, 0, 2, 0, 5, 0},
	{0, 3, 0, 5, 0, 0},
	{1, 0, 2, 0, 0, 0},
};

void dfs_init() {
	memset(D, -1, sizeof(D));
}



void dfs_print() {
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 6; ++j) {
			printf("%d ", D[i][j]);
		}
		puts("");
	}
}

int dfs(int i, int j) {
	if(i == 0 && j == 0) {
		return D[0][0] = gold[0][0];
	}
	if(i < 0 || j < 0) {
		return 0;
	}
	if(D[i][j] != -1) {
		return D[i][j];
	}
	return D[i][j] = gold[i][j] + max(dfs(i-1,j), dfs(i, j-1));
}

int main() {
	dfs_init(); 
	dfs(3, 5);
	dfs_print();
	return 0;
} 


