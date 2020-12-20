#include <iostream>

using namespace std;

int dfs(int n) {
	return !n ? 1 : n * dfs(n-1);
}

int main() {
	int n; 
	while(scanf("%d", &n) != EOF) {
		printf("%d\n", dfs(n));
	}
	return 0;
}

