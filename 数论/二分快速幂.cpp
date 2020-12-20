#define LL long long

LL Exp(LL a, LL n, LL Mod){
	LL ans = 1;
	if (a == 0 || a == 1) {
		return a;
	}
	while (n){
		if (n & 1) ans = ans*a%Mod;
		a = a*a%Mod;
		n >>= 1;
	}
	return ans;
}