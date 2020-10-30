/*
最大子矩阵
    统计最大的矩形，满足该矩形内部全为1
    1、以矩阵的每行为基准线，统计连接基准线的最大高度，
	2、统计基准线上方的最大矩形面积，取最大者即为最大子矩阵。
	3、每条基准线上方的最大面积采用 “左右邻居扫描法”
Author: WhereIsHeroFrom
Update Time: 2018-3-22
Algorithm Complexity: O(nm)
*/

#include <iostream>

using namespace std;

#define MAXN 100010
int n;

int L[MAXN], R[MAXN];
int getLargetRectangle(int n, int *heights) {
	int i;
	int Max = 0;
	heights[0] = heights[n+1] = -1;
	for(i = 1; i <= n; ++i) {
		int l = i-1;
		// 1.往左找到第一个高度小于它的
		while(heights[l] >= heights[i]) {
			l = L[l];
		}
		L[i] = l;
	}
	for(i = n; i >= 1; --i) {
		int r = i+1;
		// 2.往右找到第一个高度小于它的
		while(heights[r] >= heights[i]) {
			r = R[r];
		}
		R[i] = r;
		
		// 3.计算以当前高度为最低高度的最大矩形面积
		int v = heights[i] * (R[i] - L[i] - 1);
		if(v > Max) 
			Max = v;
	}
	return Max;
}

#define MAX_MATRIX 1010
// 统计最大的矩形，满足该矩形内部全为1
// 以矩阵的每行为基准线，统计基准线上方的最大矩形面积，取最大者即为最大子矩阵
int getMaxMatrix(int r, int c, int mat[MAX_MATRIX][MAX_MATRIX]) {
	int i, j;
	int Max = 0;
	for(i = 1; i <= r; ++i) {
		for(j = 1; j <= c; ++j) {
			if(mat[i][j]) {
				mat[i][j] = mat[i-1][j] + 1;
			}
		}
		int v = getLargetRectangle(c, mat[i]);
		if(v > Max) Max = v;
	}
	return Max;
}
