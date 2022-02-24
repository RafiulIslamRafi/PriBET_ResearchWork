#include<bits/stdc++.h>
#include "stopwords.h"
#include "hashing.h"
#include "DecimaltoHexaDecimal.h"

using namespace std;

#define Coefficient 32609

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
pair<string, long long int> poly(){
	//cout<<v.size()<<endl;
	sort(v.rbegin(), v.rend());
	string poly = "";
	long long int AA = 0;
	for(int i=0; i<v.size(); i++){
		AA += 1LL * v[i].second * v[i].second;
        if(!poly.size()) poly = DecimalToHexaDecimal(v[i].second) + "x^" + to_string(v[i].first);
        else poly += " + " + DecimalToHexaDecimal(v[i].second) + "x^" + to_string(v[i].first);
    }
    v.clear();
    return {poly, AA};
}

pair<string, long long int> documentToPolynomial(string filename){
	DocumentToVector(filename);
	return poly();
}
pair<string, long long int> documentToReversePolynomial(string filename){
	v.clear();
	mp.clear();
	DocumentToVector(filename);
	for(int i=0;i<v.size();i++) v[i].first = Coefficient - v[i].first;
	return poly();
}



