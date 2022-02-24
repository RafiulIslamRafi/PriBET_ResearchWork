#include<bits/stdc++.h>
using namespace std;

bool isKthBitSet(long long int n, int k) {return (n & (1 << (k - 1)));}
vector<int> intToVector(long long int num, int BitNumber, bool rev){
	vector<int>v;
	for(int i=1;i<=BitNumber;i++){
		if(isKthBitSet(1LL*num,i)) v.push_back(1);
		else v.push_back(0);
	}

	if(!rev){
		reverse(v.begin(), v.end());
	}
	return v;
}
