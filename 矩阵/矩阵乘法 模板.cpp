#include <iostream>
#include <cstring>
#include <vector>
#include <windows.h>
using namespace std;


#define MAXN 500
#define LL __int64

class Matrix {
private:
	int n, m;
	LL** pkData;
public:
	Matrix() : n(0), m(0) {
		pkData = NULL;
	}

	void Alloc() {
		// 之所以把空间放在堆上，是因为当MAXN很大时，栈上分配不了这么多空间
		pkData = new LL *[MAXN];
		for (int i = 0; i < MAXN; ++i) {
			pkData[i] = new LL[MAXN];
		}
	}

	void Dealloc() {
		if (pkData) {
			// 先释放低维空间，再释放高维的
			for (int i = 0; i < MAXN; ++i) {
				delete[] pkData[i];
			}
			delete[] pkData;
			pkData = NULL;
		}
	}

	void Reset(int nn, int mm, LL data[][MAXN]) {
		n = nn;
		m = mm;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				pkData[i][j] = data[i][j];
			}
		}
	}

	void Reset(int nn, int mm) {
		n = nn;
		m = mm;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				pkData[i][j] = 0;
			}
		}
	}

	void Multiply_ijk(const Matrix& other, Matrix& ret) {
		// assert(m == other.n);
		ret.Reset(n, other.m);
		int i, j, k;
		for (i = 0; i < n; i++) {
			for (j = 0; j < other.m; j++) {
				for (k = 0; k < m; k++) {
					ret.pkData[i][j] += pkData[i][k] * other.pkData[k][j];
				}
			}
		}
	}

	void Multiply_ikj(const Matrix& other, Matrix& ret) {
		// assert(m == other.n);
		ret.Reset(n, other.m);
		int i, j, k;
		for (i = 0; i < n; i++) {
			for (k = 0; k < m; k++) {
				LL v = pkData[i][k];
				for (j = 0; j < other.m; j++) {
					ret.pkData[i][j] += v * other.pkData[k][j];
				}
			}
		}
	}

	void Multiply_kij(const Matrix& other, Matrix& ret) {
		// assert(m == other.n);
		ret.Reset(n, other.m);
		int i, j, k;
		for (k = 0; k < m; k++) {
			for (i = 0; i < n; i++) {
				LL v = pkData[i][k];
				for (j = 0; j < other.m; j++) {
					ret.pkData[i][j] += v * other.pkData[k][j];
				}
			}
		}
	}
	void Print() {
		printf("<%d, %d>\n", n, m);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				printf("%5d ", pkData[i][j]);
			}
			puts("");
		}
		puts("");
	}

};

LL AArray[MAXN][MAXN] = {
	{ 8124, 876, 12123 },
	{ 124, 425, 2136 }
};

LL BArray[MAXN][MAXN] = {
	{ 3251, 2132 },
	{ 123, 2354 },
	{ 2345, 2346 }
};

int main() {
	Matrix A, B, C;
	A.Alloc();
	B.Alloc();
	C.Alloc();

	A.Reset(MAXN, MAXN, AArray);
	B.Reset(MAXN, MAXN, BArray);

	int begin = GetTickCount();
	A.Multiply_ijk(B, C);
	printf("ijk = %d\n", GetTickCount() - begin);
	begin = GetTickCount();
	A.Multiply_ikj(B, C);
	printf("ikj = %d\n", GetTickCount() - begin);
	begin = GetTickCount();
	A.Multiply_kij(B, C);
	printf("kij = %d\n", GetTickCount() - begin);
	begin = GetTickCount();

	return 0;
}
/*

1000
ijk = 8657
ikj = 3687
kij = 3688

2000
ijk = 69547 ms
ikj = 28000 ms
kij = 29672 ms
*/