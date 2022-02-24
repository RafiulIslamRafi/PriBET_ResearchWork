#include<bits/stdc++.h>
#include "stopwords.h"
#include "hashing.h"

using namespace std;
#define FI               freopen("Document.txt","r",stdin)
#define FO               freopen("out.txt","w",stdout)
#define fastio           ios_base::sync_with_stdio(false);;cin.tie(NULL);cout.tie(NULL);

map<string, int>mp;
void documentToPolynomial(){
	string poly = "";
	for(auto it = mp.begin(); it != mp.end(); it++){
        if(!poly.size()) poly = to_string(it->second) + "x^" + to_string(wordToInt(it->first));
        else poly += " + " + to_string(it->second) + "x^" + to_string(wordToInt(it->first));

    }
    cout<<poly<<endl;
}
void documentToReversePolynomial(){
	string poly = "";
	for(auto it = mp.begin(); it != mp.end(); it++){
        if(!poly.size()) poly = to_string(it->second) + "x^" + to_string(1000000007 - wordToInt(it->first));
        else poly += " + " + to_string(it->second) + "x^" + to_string(1000000007 - wordToInt(it->first));

    }
    cout<<poly<<endl;
}
int main()
{
    FI;
    ///FO;
    fastio;
    init();
    string str;
    while(cin>>str){
        if(!isStopWord(str)){
            if(mp.find(str)!=mp.end()){
                mp[str]++;
            }
            else mp[str] = 1;
        }
    }
    for(auto it = mp.begin(); it != mp.end(); it++){
        cout<<(it->first)<<" "<<(it->second)<<" "<<wordToInt(it->first)<<endl;

    }
    documentToPolynomial();
    documentToReversePolynomial();
    return 0;
}

