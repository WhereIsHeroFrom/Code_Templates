#include <stdio.h>
#include <string.h>

#define MAXN 1000010

/*
    NULL_MATCH must be -1
 */
#define NULL_MATCH -1


/*
    str[...i] ：   表示子串 str[0...i] 的某个真后缀   （不包含str[0...i]本身）
    str[0...] ：   表示子串 str[0...i] 的某个真前缀   （不包含str[0...i]本身）
    next[i]   ：   表示满足  (str[0...] == str[...i])  时，的前缀(后缀)的最大长度减1
                   即：     (str[0 ... next[i]] == str[i-next[i] ... i])
 */
void genNext(char *str, int* next) {
    int i, j;
    int len = strlen(str);
    /*
        1个字符的时候，没有真后缀，所以 next[0] = -1
     */
    next[0] = NULL_MATCH;
    for(j = NULL_MATCH, i = 1; i < len; i++) {
        /*
            在保证 str[i-1] == str[j-1] 的情况下，如果能找到  str[i] == str[j]， 那么 next[i] = j;
        */
        while( j > NULL_MATCH && str[i] != str[j+1] ) 
            j = next[j];
        if(str[i] == str[j+1]) j ++;
        next[i] = j;
    } 
}

/* 
    src[0 ... n-1] 目标串
    match[0 ... m-1] 匹配串 
*/
int KMP(const char *src, char *match, int *next) {
    int i, j;
    int n = strlen(src);
    int m = strlen(match);
    int cnt = 0;
    for(j = NULL_MATCH, i = 0; i < n; i++) {
        while( j > NULL_MATCH && src[i] != match[j+1]) j = next[j];
        if(src[i] == match[j+1]) j++;
        if(j == m - 1) {
            cnt ++;
            j = next[j];
        }
    }
    return cnt;
}

char T[MAXN], S[MAXN];
int next[MAXN];

int main() {
    int t;
    scanf("%d", &t);
    while(t--) {
        scanf("%s %s", T, S);
        genNext(T, next);
        printf("%d\n", KMP(S, T, next) );
    }
    return 0;
}
