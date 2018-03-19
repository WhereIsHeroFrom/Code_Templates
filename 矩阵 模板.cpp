#define MAXN 70
#define MOD 1234567891
#define LL __int64


class Matrix {
public:
    int n, m;
    LL d[MAXN][MAXN];
    Matrix() {
        n = m = 0;
        int i, j;
        for(i = 0; i < MAXN; i++) {
            for(j = 0; j < MAXN; j++) {
                d[i][j] = 0;
            }
        }
    }
    Matrix operator *(const Matrix& other) {
        Matrix ret;
        ret.n = n;
        ret.m = other.m;
        int i, j, k;
        for(j = 0; j < ret.m; j++) {
            for(i = 0; i < ret.n; i++) {
                ret.d[i][j] = 0;
                for(k = 0; k < m; k++) {
                    ret.d[i][j] += d[i][k] * other.d[k][j];
                    if (ret.d[i][j] >= MOD)
                        ret.d[i][j] %= MOD;
                }
            }
        }
        return ret;
    }
    
    Matrix Identity(int _n) {
        Matrix I;
        I.n = I.m = _n;
        int i, j;
        for(i = 0; i < _n; i++) {
            for(j = 0; j < _n; j++) {
                I.d[i][j] = (i == j) ? 1 : 0;
            }
        }
        return I;
    }
    
    Matrix getPow(int e) {
        Matrix tmp = *this;
        Matrix ret = Identity(n);
        while(e) {
            if(e & 1) {
                ret = ret * tmp;
            }
            e >>= 1;
            tmp = tmp * tmp;
        }
        return ret;
    }

    // | A  A |
    // | O  I |
    // 扩展矩阵用于求A + A^2 + A^3 + ... + A^n
    Matrix getExtendMatrix() {
        Matrix ret;
        ret.n = ret.m = n * 2;
        ret.copyMatrix( *this, 0, 0);
        ret.copyMatrix( *this, 0, n);
        ret.copyMatrix( Identity(n), n, n);
        return ret;
    }

    // 将矩阵A拷贝到当期矩阵的(r, c)位置
    void copyMatrix(Matrix A, int r, int c) {
        for(int i = r; i < r + A.n; i++) {
            for(int j = c; j < c + A.n; j++) {
                d[i][j] = A.d[i-r][j-c];
            }
        }
    }
    
    void Print() {
        int i, j;
        for(i = 0; i < n; i++) {
            for(j = 0; j < m; j++) {
                printf("%d ", d[i][j]);
            }
            puts("");
        }
    }
};
