#include<bits/stdc++.h>
using namespace std;

string DecimalToHexaDecimal(int &n){
	string ans="";
	int temp;
	while(n){
		char temp;
		if(n%16 < 10) temp = 48 + n%16;
		else temp = 55 + n%16;
		ans = temp + ans;
		n /= 16;
	}
	return ans;
}
