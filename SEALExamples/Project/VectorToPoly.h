#include<bits/stdc++.h>
#include "DecimaltoHexaDecimal.h"
using namespace std;

string VectorToPolynomial(vector<int>&v){
	string str;
	for(int i = 0; i < v.size(); i++){
		if(!v[i]) continue;
		if(!str.size()) str = DecimalToHexaDecimal(v[i]) + "x^" + to_string(i+1);
		else str = DecimalToHexaDecimal(v[i]) + "x^" + to_string(i+1) + " + " + str;
	}
	return str;


}
