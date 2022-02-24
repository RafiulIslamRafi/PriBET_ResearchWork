#include<bits/stdc++.h>
#include "IntegerToVector.h"
#include "VectorToPoly.h"
using namespace std;

int main(){

	cout<<(1<<31)-1<<endl;
	int num1 = 6;
	int num2 = 6;
	vector<int> vec1, vec2;
	vec1 = intToVector(num1, 3, false);
	vec2 = intToVector(num1, 3, true);

	vector<int>v = intToVector((1<<32) - 1, 3, false);

	for(auto i: v) {
		cout<<i<<" ";
	}
	cout<<endl;

	string str1 = VectorToPolynomial(vec1);
	string str2 = VectorToPolynomial(vec2);
	cout<<str1<<endl<<str2<<endl;

	return 0;
}
