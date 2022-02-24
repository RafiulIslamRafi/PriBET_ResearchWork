#include<bits/stdc++.h>
using namespace std;

int HexaDecimaltoDecimal(string str){
	int temp = 1, ans = 0;
	for(int i = str.size() - 1; i>=0; i--){
		int V;
		if(str[i]>='A') V = 10 + str[i] - 'A';
		else V = str[i] - '0';
		ans += V * temp;
		temp *= 16;
	}
	return ans;
}
