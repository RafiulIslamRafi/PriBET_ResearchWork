#include<bits/stdc++.h>
using namespace std;

///#define MOD 16381
#define MOD 32609

int wordToInt(const string &str){
	///lowerCase(str); ///stopword baad deyar time ei koreci.
	long long int ans = 0;
	for(int i=0;i<str.size();i++){
		ans = (ans*26 + (str[i] - '0')) % MOD;
		if(ans<0) ans += MOD;
	}
	return (int)ans;
}
