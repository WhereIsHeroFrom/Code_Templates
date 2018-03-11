#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

#define eps 1e-6
#define MAXL 110

class GaussMatrix {
public:
    int r, c;                 // r个方程，c个未知数 
    double d[MAXL][MAXL];     // 增广矩阵 
    double x[MAXL];           // 解集 
    /*
    d[0][0]   * x[0] +   d[0][1] * x[1] + ... +   d[0][c-1] * x[c-1] =   d[0][c];
    d[1][0]   * x[0] +   d[1][1] * x[1] + ... +   d[1][c-1] * x[c-1] =   d[1][c];
    ...
    ...
    d[r-1][0] * x[0] + d[r-1][1] * x[1] + ... + d[r-1][c-1] * x[c-1] = d[r-1][c];
    */
    
    void swap_row(int ra, int rb) {
        int i;
        for(i = 0; i <= c; i++) {
            double tmp = d[ra][i];
            d[ra][i] = d[rb][i];
            d[rb][i] = tmp;
            
        }
    }
    
    bool zero(double v) {
        return fabs(v) < eps;
    }
    
    bool gauss() {
        int i, j, k;
        int col = 0;           // 当前枚举列 
        int maxrow;            // 第col列中绝对值最大的行号
        
        for(i = 0; i < r && col < c; i++) {
            maxrow = i;
            for(j = i+1; j < r; j++) {
                if( fabs(d[j][col]) > fabs(d[maxrow][col]) ) {
                    maxrow = j;
                }
            }
            // 将第col列最大的行maxrow和第i行交换，避免误差 
            if(i != maxrow) swap_row(i, maxrow);
            // 如果第col中最大的那行的值为0继续找下一列的 
            if( zero(d[i][col]) ) {
                col ++;
                i --;
                continue;
            }
            for(j = i+1; j < r; j++) {
                // 将第j行第col列的元素消为0 
                if( !zero(d[j][col]) ) {
                    double sub = d[j][col]/d[i][col];
                    for(k = col; k <= c; k++) {
                        d[j][k] = d[j][k] - d[i][k] * sub; // 注意：这一步是关键，精度误差就在这里出现 
                    }
                }
            }
            col++;
        }
        
        // 唯一解 回归 
        for(i = c-1; i >= 0; i--) {
            double sum = 0;
            for(j = i+1; j < c; j++) {
                sum += x[j] * d[i][j];
            }
            x[i] = (d[i][c] - sum) / d[i][i];
            if( zero(x[i]) ) x[i] = 0;
        }
        return true;
    }    
    void debug_print_x() {
        int i;
        for(i = 0; i < c; i++) {
            printf("%.3lf ", x[i]);
        }
        puts("");
    }
    
    void debug_print() {
        int i, j;
        puts("---------------------------------");
        for(i = 0; i < r; i++) {
            for(j = 0; j <= c; j++) {
                printf("%.3lf ", d[i][j]);
            }
            puts("");
        }
        puts("---------------------------------");
    } 
};

