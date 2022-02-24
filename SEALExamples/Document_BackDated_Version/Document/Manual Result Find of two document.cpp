#include<bits/stdc++.h>
#include "stopwords.h"
#define ll long long
using namespace std;
map<string, int> mp1, mp2;

void DocumentToVector1(string filename)
{
    ///freopen(filename.c_str(),"r",stdin);
    ifstream cin(filename.c_str());
    init();
    string str;
    while(cin>>str){
		//cout<<str<<endl;
        if(!isStopWord(str)){
            if(mp1.find(str)!=mp1.end()){
                mp1[str]++;
            }
            else mp1[str] = 1;
        }
    }
}

void DocumentToVector2(string filename)
{
    ///freopen(filename.c_str(),"r",stdin);
    ifstream cin(filename.c_str());
    init();
    string str;
    while(cin>>str){
		//cout<<str<<endl;
        if(!isStopWord(str)){
            if(mp2.find(str)!=mp2.end()){
                mp2[str]++;
            }
            else mp2[str] = 1;
        }
    }
}

int main(){
	DocumentToVector1("Fiji2014.txt");
	DocumentToVector2("Fiji2016.txt");
	cout<<"Without Hasing: (Manual)\n"<<endl;
	cout<<"Word count1 = "<<mp1.size()<<endl;
	cout<<"Word Count2 = "<<mp2.size()<<endl<<endl;


	ll AA = 0, BB = 0, AB = 0;
	for(auto it = mp1.begin(); it != mp1.end(); it++) AA += (it->second) * (it->second);
	for(auto it = mp2.begin(); it != mp2.end(); it++) BB += (it->second) * (it->second);
	for(auto it = mp2.begin(); it != mp2.end(); it++){
		if(mp1.find(it->first) != mp1.end()){
                AB += mp1[it->first]*(it->second);
		}
	}
	cout<<"AB = "<<AB<<"\nAA = "<<AA<<"\nBB = "<<BB<<endl;
	cout<<"\ncosX = "<<(AB*1.0 / ((AA+BB-AB)*1.0))<<endl;

	return 0;
}

