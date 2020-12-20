#include <iostream>

using namespace std;


int dfs(unsigned int n) {
	if(n <= 1) {
		return 1;
	}
	return dfs(n-1) + dfs(n-2);
}

int main() {
	int n;
	while(scanf("%d", &n) != EOF) {
		printf("%d\n", dfs(n));
	}
	return 0;
}

