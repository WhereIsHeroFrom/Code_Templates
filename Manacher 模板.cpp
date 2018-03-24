/*
Manacher算法（求解字符串最长回文） 
   1.将字符串间隔插入一个全新字符，将字符串转换成奇数长度； 
   2.线性枚举每个字符为中心轴，计算第i个字符的最长回文半径p[i];
      a.利用之前的计算结果获得p[i]初始值；
	  b.两边扩展，更新p[i]值；
	  c.利用i+p[i]更新核心中心轴；
	  d.利用2*p[i]-1更新最长回文长度 
Author: WhereIsHeroFrom
Update Time: 2018-3-24
Algorithm Complexity: O(n)
*/

#include <iostream>
#include <cstring>

using namespace std;

#define MAXN 1000010
int p[MAXN];
char strTmp[MAXN];
 
int Min(int a, int b) {
	return a < b ? a : b;
}

void ManacherPre(char *str) {
	strcpy(strTmp, str);
	int i;
	for(i = 0; strTmp[i]; ++i) {
		str[2*i] = '$';
		str[2*i+1] = strTmp[i];
	}
	str[2*i] = '$';
	str[2*i+1] = '\0';
}

int Manacher(char *str) {
	int ct = 0, r = 0, maxLen = 1;
	p[0] = 1;
	for(int i = 1; str[i]; ++i) {
		// 1.计算p[i]初始值 
		if(i < r) {
			p[i] = Min(p[2*ct-i], r-i);
		}else {
			p[i] = 1;
		}
		// 2.扩张p[i]，以适应达到p[i]最大值 
		while(i-p[i]>=0 && str[i-p[i]] == str[i+p[i]])
			++p[i];
		
		// 3.更新ct
		if(p[i] + i > r) {
			ct = i;
			r = p[i] + i;
		}
		// 4.更新最长回文 
		if(2*p[i]-1 > maxLen) {
			maxLen = 2*p[i] - 1;
		}
	}
	return maxLen; 
}



char str[MAXN];

int main() {
	while(scanf("%s", str) != EOF) {
		ManacherPre(str);
		int len = Manacher(str);
		printf("%d\n", len/2);
	}
	return 0;
} 
