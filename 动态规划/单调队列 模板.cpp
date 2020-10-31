#include <iostream>

using namespace std;

#define MAXN 1000010

///////////////////////单调队列//////////////////////////
// 单调递增队列模板
// 单调递增队列的 data 域存的是原数组的下标，映射过去后要保证这些在 data 中的原数组数据是单调递增的

#define MAXN 1000010
#define Type int
#define NullIndex -1

class Deque {
private:
	int* data;                  // 存储 rawdata 的下标
	Type* rawdata;              // 代表源数据，用来做单调性比较
	int head, tail;             // 队列头尾指针
	int range;                  // 表示状态转移时候的区间范围

	Deque() {
		data = new int[MAXN];
	}
public:
	void init(int r, Type* rd);
	void insert(int index);
	int query();
	int size();

	static Deque& instance() {
		static Deque inst;
		return inst;
	}
};


void Deque::init(int r, Type* rd) {
	head = tail = 0;
	range = r;
	rawdata = rd;
}

/* 维护好一个单调递增的队列 (data[]单调递增) */
// dp[i] = min{ dp[i-j] | 0 < j <= range } + C
void Deque::insert(int index) {

	// 1. 当前数据的下标 - 队列头数据的下标 < range
	while (head < tail && index - data[head] >= range)
		++head;

	// 2. 维护单调递增序列（如果需要单调不降序列，则 '>=' 改成 '>'）
	while (head < tail && rawdata[data[tail - 1]] >= rawdata[index])
		--tail;

	// 3. 插入元素
	data[tail++] = index;
}

/* 取队列头元素，如果队列为空，则返回 NullIndex */
int Deque::query() {
	if (head == tail) {
		return NullIndex;
	}
	return data[head];
}

int Deque::size() {
	return tail - head;
}