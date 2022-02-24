#include<bits/stdc++.h>
#include "HexaDeciamlToDecimal.h"

using namespace std;

#define Coefficient 32609
pair<int, string> f(string &term){
	for(int i=0; i<term.size(); i++){
		if(term[i]=='^') {
			///cout<<i<<endl;
			///cout<<term.substr(i+1,term.size()-i)<<endl;
			if(term.substr(i+1,term.size()-i) == to_string(Coefficient)){
				///cout<<term.substr(0,i-1)<<endl;
				///cout<<term<<endl;
				return {true, term.substr(0,i-1)};
			}
			else return {false,"0"};
		}
	}
	return {false,"0"};
}

int CoefficientforDocument(string str){
	string term;
	stringstream ss(str);
	///ss>>term;
	///cout<<term<<endl;
	while(ss>>term){
		//cout<<term<<endl;
		pair<int, string> pi = f(term);
		if(pi.first) return HexaDecimaltoDecimal(pi.second);
	}
	return 0;
}
