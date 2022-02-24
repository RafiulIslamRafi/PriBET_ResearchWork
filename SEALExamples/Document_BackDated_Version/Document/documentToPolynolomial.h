#include<bits/stdc++.h>
#include "stopwords.h"
#include "hashing.h"
#include "DecimaltoHexaDecimal.h"

#define Coefficient 1<<15

using namespace std;

unordered_map<int, int>mp;
vector<pair<int,int>>v;
void DocumentToVector(string &filename)
{
    ///freopen(filename.c_str(),"r",stdin);
    ifstream cin(filename.c_str());
    init();
    string str;
    while(cin>>str){
		//cout<<str<<endl;
        if(!isStopWord(str)){
			int temp = wordToInt(str);
            if(mp.find(temp)!=mp.end()){
                mp[temp]++;
            }
            else mp[temp] = 1;
        }
    }
    for(auto it = mp.begin(); it != mp.end(); it++){
		v.push_back({it->first, it->second});
    }
    mp.clear();
}
string poly(){
	//cout<<v.size()<<endl;
	sort(v.rbegin(), v.rend());
	string poly = "";
	for(int i=0; i<v.size(); i++){
        if(!poly.size()) poly = to_string(v[i].second) + "x^" + to_string(v[i].first);
        else poly += " + " + to_string(v[i].second) + "x^" + to_string(v[i].first);
    }
    v.clear();
    return poly;
}

string documentToPolynomial(string filename){
	DocumentToVector(filename);
	return poly();
}
string documentToReversePolynomial(string filename){
	v.clear();
	mp.clear();
	DocumentToVector(filename);
	for(int i=0;i<v.size();i++) v[i].first = Coefficient - v[i].first;
	return poly();
}

pair<string, string> documentToPolynomialwithReverse(string filename){
	return {documentToPolynomial(filename), documentToReversePolynomial(filename)};
}

pair<int, string> f(string &term){
	for(int i=0; i<term.size(); i++){
		if(term[i]=='^') {
			///cout<<i<<endl;
			///cout<<term.substr(i+1,term.size()-i)<<endl;
			if(term.substr(i+1,term.size()-i) == to_string(Coefficient)){
				///cout<<term.substr(0,i-1)<<endl;
				return {true, term.substr(0,i-1)};
			}
			else return {false,0};
		}
	}
	return {false,0};
}

string CoefficientforDocument(string str){
	string term;
	stringstream ss(str);
	while(ss>>term){
		//cout<<term<<endl;
		pair<int, string> pi = f(term);
		if(pi.first) return pi.second;
	}
	return "0";
}
vector<string> ArrayOfSubpoly(string &str){
	vector<string> polys;
	string subpoly = "";
	int cnt = 0;
	string term;
	stringstream ss(str);
	while(ss>>term){
		if(term=="+") continue;
		else{
			cnt++;
			if(subpoly.size()) subpoly += " + " + term;
			else subpoly += term;
			if(cnt%18==0){
				polys.push_back(subpoly);
				subpoly = "";
			}
		}
	}
	if(subpoly.size()) polys.push_back(subpoly);
	return polys;
}
