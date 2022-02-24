#include<bits/stdc++.h>
using namespace std;

string VectorToPolynomial(vector<int>&v){
	string str;
	for(int i = 0; i < v.size(); i++){
		if(!v[i]) continue;
		if(!str.size()) str = to_string(v[i]) + "x^" + to_string(i+1);
		else str = to_string(v[i]) + "x^" + to_string(i+1) + " + " + str;
	}
	return str;
}
