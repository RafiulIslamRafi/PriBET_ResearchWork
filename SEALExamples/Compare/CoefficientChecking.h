#include<bits/stdc++.h>
using namespace std;

bool isTermExist(string &term, int power){ /// Single Term: return True if Term exist, else Return false
	for(int i=0; i<term.size(); i++){
		if(term[i]=='^') {
			if(term.substr(i+1,term.size()-i) == to_string(power)) return true;
			else return false;
		}
	}
	return false;
}

bool isPowerExist(string poly, int power){ ///full poly: Return true if Coefficient Found, else false
//	cout<<power<<" th power checking"<<endl;
	string term;
	stringstream ss(poly);
	int i=1;
	while(ss>>term){
		i++;
		if(i&1) continue;
		if(isTermExist(term, power)) return true;
	}
	return false;
}
