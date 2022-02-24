#include<bits/stdc++.h>
using namespace std;

string decimalToHexa(int n){
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
int main()
{
	int num;
	int t = 100;
	while(t--){
		cin>>num;
		cout<<decimalToHexa(num)<<endl;
	}
	return 0;
}
