#include<bits/stdc++.h>
#include "documentToPolynolomial.h"
using namespace std;
#define FO               freopen("out2014.txt","w",stdout)
//#define FO               freopen("out2016.txt","w",stdout)

int main(){
	FO;
	cout<<"fiji2014 = \n";
	cout<<"Forward"<<endl<<documentToPolynomial("Fiji2014.txt")<<endl<<"Reverse"<<endl<<documentToReversePolynomial("Fiji2014.txt")<<endl<<endl;
//	cout<<"fiji2016 = \n";
//	cout<<"Forward"<<endl<<documentToPolynomial("Fiji2016.txt")<<endl<<"Reverse"<<endl<<documentToReversePolynomial("Fiji2016.txt")<<endl<<endl;
	return 0;
}
