#include<bits/stdc++.h>
#include "stopwords.h"
#include "hashing.h"
#define ll long long
using namespace std;

map<int, int> mp1, mp2;

void DocumentToVector1(string filename)
{
    ///freopen(filename.c_str(),"r",stdin);
    ifstream cin(filename.c_str());
    init();
    string str;
    while(cin>>str){
		//cout<<str<<endl;
        if(!isStopWord(str)){
            if(mp1.find(wordToInt(str))!=mp1.end()){
                mp1[wordToInt(str)]++;
            }
            else mp1[wordToInt(str)] = 1;
        }
    }
    int temp = 0;
    for(auto it = mp1.begin(); it!=mp1.end(); it++) temp += it->second * it->second;
    cout<<temp<<endl;
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
            if(mp2.find(wordToInt(str))!=mp2.end()){
                mp2[wordToInt(str)]++;
            }
            else mp2[wordToInt(str)] = 1;
        }
    }
	int temp = 0;
    for(auto it = mp2.begin(); it!=mp2.end(); it++) temp += it->second * it->second;
    cout<<temp<<endl;
}

int main(){
	DocumentToVector1("Fiji2014.txt");
	DocumentToVector2("Fiji2016.txt");
	cout<<"With Hasing: (Manual)\n"<<endl;

	cout<<"count1 = "<<mp1.size()<<endl; /// 755
	cout<<"count2 = "<<mp2.size()<<endl; /// 628


	ll AA = 55735, BB = 30431, AB = 0;
	for(auto it = mp2.begin(); it != mp2.end(); it++){
		if(mp1.find(it->first) != mp1.end()){
                AB += mp1[it->first]*(it->second);
		}
	}

	cout<<"AB = "<<AB<<"\nAA = "<<AA<<"\nBB = "<<BB<<endl;  /// AB = 8981
	cout<<"\ncosX = "<<(AB*1.0 / ((AA+BB-AB)*1.0))<<endl;  /// cosX = 0.116357

	return 0;
}
