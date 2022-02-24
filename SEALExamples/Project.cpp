#include <bits/stdc++.h>
#include "Project/SealContent.h"
using namespace std;
using namespace seal;

int main()
{
	string poly;
	cout<<endl<<"SEAL Initializing: ";
	clock_t begin_time = clock();
	EncryptionParameters parms(scheme_type::BFV);
	parms.set_poly_modulus_degree(1<<15);
	parms.set_coeff_modulus(coeff_modulus_128(1<<15));
	parms.set_plain_modulus(1 << 15);
	auto context = SEALContext::Create(parms);
	//IntegerEncoder encoder(parms.plain_modulus());
	double time = float( clock () - begin_time )/CLOCKS_PER_SEC;
	cout<<"done"<<endl<<endl;
	bool initTimeShown = false;

	int bit = 15;
	/// Main Coding Start:
	while(1)
	{
		system("clear");
		print_example_banner("Research Project");
//		print_parameters(context);
		if(!initTimeShown) {
			cout<<"SEAL initialization time = "<< time <<" s"<<endl<<endl;
			initTimeShown = true;
		}

		cout<<"1\tCSV to txt"<<endl;
		cout<<"2\tTxt to Polynomial To CypherText to Store"<<endl; ///cloud will store the Cyphertext
		cout<<"3\tComputation"<<endl;
		cout<<"4\tExit"<<endl;
		cout<<"\nSelect Your Choich\t: ";
		int choich, i, n, loop;
		cin>>choich;
		bool exit;
		vector<pair<int, string> > AllPoly;
		switch(choich)
		{
		case 1:
			begin_time = clock();
			cout<<endl;
			system("python3 Project/csvTotxt.py");
			cout <<"Txt Convert Time: "<<float( clock () - begin_time )/CLOCKS_PER_SEC<<" s"<<endl<<endl; ///endting time
			break;
		case 2:
			begin_time = clock();
			AllPoly = csv_dataToPolynomial(bit); //AllFunction.h
			cout <<"Poly Convertion Time: "<<float( clock () - begin_time )/CLOCKS_PER_SEC<<" s"<<endl<<endl; ///endting time
//			cout<<"How many poly you want to check : ";
//			cin>>loop;
//			for(i=1;i<=loop;i++){
//				cout<<endl<<"Input ("<<i<<") the poly No: ";
//				cin>>n;
//				system("clear");
//				cout<<AllPoly[n].first<<endl<<AllPoly[n].second<<endl<<endl;
//			}
			begin_time = clock();
			polyToCypherStore(AllPoly, parms, context); //poly_To_Cypher.h
			cout <<"All Cyphertext making & storing Time: "<<float( clock () - begin_time )/CLOCKS_PER_SEC<<" s"<<endl<<endl; ///endting time
			break;
		case 3:
			exit = Subthings(parms, context, bit); //AllFunction.h
			if(exit) return 0;
			break;
		case 4:
			cout<<"Exit"<<endl;
			return 0;
			break;
		default:
			cout<<"Plz Insert Valid Character"<<endl;
			break;
		}
		cout<<"Press any character to Continue\t: ";
		getchar();
		cin.ignore();
	}
	return 0;
}

