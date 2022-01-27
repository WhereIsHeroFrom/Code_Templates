#define maxn 1010
bool isPal[1010][1010];

void preCalc(char *s) {
    int i, j;
    int l, r;
    int len = strlen(s);
    for(i = 0; i < len; ++i) {
        // 奇数
        for(j = 0; j < len; ++j) {
            l = i-j;
            r = i+j;
            if(l < 0 || r >= len) {
                break;
            }
            if(l == r) {
                isPal[l][r] = true;
            }else
                isPal[l][r] = ( (s[l] == s[r]) && isPal[l+1][r-1] );
        }
        // 偶数
        for(j = 0; j < len; ++j) {
            l = i-j;
            r = i+1+j;
            if(l < 0 || r >= len) {
                break;
            }
            if(l + 1 == r) {
                isPal[l][r] = (s[l] == s[r]);
            }else
                isPal[l][r] = (s[l] == s[r]) && isPal[l+1][r-1];
        }
    }
}
