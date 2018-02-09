#include <iostream>

using namespace std;

#define MAXN 100010

int n, k;
int sum[MAXN];
char str[3000000];

void input() {
	getchar();
	int i, idx = 1;
	sum[0] = 0;
	while(gets(str)) {
		i = 0;
		int s = 0;
		while(str[i] != '\0' && str[i] != '\n') {
			if(str[i] >= '0' && str[i] <= '9') {
				s = s * 10 + (str[i] - '0');
			}else if(s > 0) {
				sum[idx] = sum[idx-1] + s;
				s = 0;
				++idx;
			}
			++i;
		}
		if(s > 0) {
			sum[idx] = sum[idx-1] + s;
			s = 0;
			++idx;
		}
		if(idx == n + 1) {
			break;
		}
	}
}



//////////////////////////////////////////////////////////////////
/////////////////////////斜率优化-模板////////////////////////////
//////////////////////////最大平均值//////////////////////////////

#define ll long long

// 单调队列（双端队列实现）维护 “下凸”折线，队列中存的是原数组下标 
int deq[MAXN];
// 双端队列的两个指针 
int head, tail;

bool isBigger(int i, int j, int k) {
	// i-k的斜率 K(i, k) = (sum[k] - sum[i]) / (k-i)
	// j-k的斜率 K(j, k) = (sum[k] - sum[j]) / (k-j)
	// K(i, k) >= K(j, k)
	return (ll)(sum[k] - sum[i]) * (ll)(k-j) >= (ll)(sum[k] - sum[j]) * (ll)(k-i);
}

double getMaxSlope() {
	head = tail = 0;
	double slope, maxv = 0; 
	for(int i = k; i <= n; i++) {
		// 点(i-k)需要插入单调队列，所以如果队列尾的点 deq[tail-1] 是个 “上凸”点，则进行删除 
		while(head + 1 < tail && isBigger(deq[tail-2], deq[tail-1], i-k))
			--tail;
		// 点(i-k)插入单调队列尾部		
		deq[tail++] = i-k;
		
		// 线段A = <deq[head] - i> 
		// 线段B = <deq[head+1] - i> 
		// K(A) < K(B) 删除队列首元素
		// 直到遇到  K(A) >= K(B)，则 <deq[head] - i>为整个折线的切线 
		while(head + 1 < tail && !isBigger(deq[head], deq[head+1], i))
			++head;
		
		// 利用折线的切线更新最大值 
		slope = (sum[i] - sum[deq[head]]) * 1.0 / (i - deq[head]);
		if(slope > maxv) {
			maxv = slope;
		} 
	}	 
	return maxv;
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

int main() {
	//freopen("1.in", "r", stdin);
	//freopen("1.my.out", "w", stdout);
	while( scanf("%d %d", &n, &k) != EOF ) {
		input();
		printf("%.2lf\n", getMaxSlope());
	}
	return 0;
}
/*
20 3
96 86 94 44 24 88 15 4 49 1 59 19 81 97 99 82 90 99 10 58

// 14 - 18, 97,99,82,90,99
*/
