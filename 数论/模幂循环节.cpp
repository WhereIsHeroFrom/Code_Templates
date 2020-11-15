#define MAXN 10

int F[MAXN+1], FPos[MAXN+1];

int f(int a, int b, int c) {
	int pre;
	memset(FPos, -1, sizeof(FPos));
	F[0] = 1 % c;
	FPos[ F[0] ] = 0;
	for(int i = 1; i <= b; ++i) {
		F[i] = F[i-1] * a % c;
		int &pre =  FPos[ F[i] ];
		if( pre == -1) {
			pre = i;
		}else {
			int K = i - pre;
			int Index = ( b - pre ) % K + pre;
			return F[Index];
		}
	}
	return F[b];
}

