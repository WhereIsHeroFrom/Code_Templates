/*
散列HASH(滚动散列)
	将离散的大整数hash到连续的数组中，返回数组下标
	HASH_MAX 取2的幂-1，代替取模。
Author: WhereIsHeroFrom
Update Time: 2018-3-21
Algorithm Complexity: 均摊O(1)
*/

#define HASH_MAX ((1<<20)-1)
int hashCases;
bool hashKey[2][HASH_MAX+1];
int hashValue[2][HASH_MAX+1];

int HASH(int idx, int value) {
	int now = (value & HASH_MAX);
	while(1) {
		if(!hashKey[idx][now]) {
			hashKey[idx][now] = true;
			hashValue[idx][now] = value;
			return now;
		}else {
			if(hashValue[idx][now] == value) {
				return now;
			}
			now = (now + 1) & HASH_MAX;
		}
	}
}

