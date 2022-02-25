#include<bits/stdc++.h>
#include "seal/seal.h"
using namespace std;
using namespace seal;

void print_example_banner2(string title)
{
    if (!title.empty())
    {
        size_t title_length = title.length();
        size_t banner_length = title_length + 2 + 2 * 10;
        string banner_top(banner_length, '*');
        string banner_middle = string(10, '*') + " " + title + " " + string(10, '*');

        cout << endl
            << banner_top << endl
            << banner_middle << endl
            << banner_top << endl
            << endl;
    }
}

///helper function of csv_dataToPolynomial()
vector<int> numbersToBinaries(vector<int> &SomeNumbersInteger, int bit)
{
	vector<int>AllNumbersBinaryVector;
	for(int i=0; i<SomeNumbersInteger.size(); i++)
	{
		vector<int>tempVector = intToVector(SomeNumbersInteger[i], bit, false);

		AllNumbersBinaryVector.insert(AllNumbersBinaryVector.end(), tempVector.begin(), tempVector.end()); ///a of 2ab of A+B-2ab: hamming distance
		for(int j=0; j<bit+2; j++) AllNumbersBinaryVector.push_back(0);
	}
	return AllNumbersBinaryVector;
}
void ConertToSmallLatter(string &str)
{
	for(int i=0; i<str.size(); i++)
	{
		if(str[i]>='A' && str[i]<='Z') str[i] = str[i] + 32;
	}
}

vector<pair<int, string> > csv_dataToPolynomial(int bit)
{
	cout<<"txt file to polynomial\t: ";
	//freopen("Project/csv_data.txt", "r", stdin);
	string filename = "Project/csv_data.txt";
	ifstream cin(filename.c_str());

	string str;
	vector<vector<string> >vec;
	bool first = true;
	while(getline(cin,str))
	{
		vector<string>temp_vec;
		string temp_str;

		stringstream ss(str);
		while(ss>>temp_str)
		{
			temp_vec.push_back(temp_str);
		}
		vec.push_back(temp_vec);
	}

	vector<pair<int, string> > AllPoly;
	for(int n=1; n<=51; n++)
	{
		//if(n!=7) continue;
		///baad ID: 1, 2, 5, 6, 26, 28
		if(n==1 || n==2 || n==5 || n==6 || n==26 || n==28) continue;
		string poly_a_AllNumbers, CateGoryName;
		vector<int> allNumbers;
		for(int i=0; i<vec.size(); i++)
		{
			if(i==0)
			{
				CateGoryName = vec[i][n-1]; ///CSV Title
				///cout<<endl<<n<<" "<<CateGoryName<<" ";
				continue;
			}
			ConertToSmallLatter(vec[i][n]);
			string temp = vec[i][n];
			//if(i<2)cout<<temp<<endl;
			///ID: 9, 11, 13, 16 => low, normal, high, faka
			if(temp == to_string(-1)) allNumbers.push_back(0);
			else if(temp == "low") allNumbers.push_back(1);
			else if(temp == "normal") allNumbers.push_back(2);
			else if(temp == "high") allNumbers.push_back(3);

			///ID: 4 => Male, Female, Faka
			else if(temp == "m") allNumbers.push_back(1);
			else if(temp == "f") allNumbers.push_back(2);

			///ID: 24,25 => Pos, Neg, Na, Faka
			else if(temp == "pos") allNumbers.push_back(1);
			else if(temp == "neg") allNumbers.push_back(2);
			else if(temp == "na") allNumbers.push_back(3);

			///ID: 27, 30, 31, 32, 33, 34, 35, 36, 37, 39, 40, 41, 47, 50 => Yes, No
			else if(temp == "yes") allNumbers.push_back(1);
			else if(temp == "no") allNumbers.push_back(2);

			///ID: for Integer: 3, 7, 14, 21, 22, 23, 29, 38, 51
			else if(n==3 || n==7 || n==14 || n==21 || n==22 || n==23 || n==29 || n==38 || n==51)
			{
				int num = 0;
				stringstream ss(temp);
				ss>>num;
				if(num==-1) allNumbers.push_back(0);
				else {
					///cout<<num<<" ";
					allNumbers.push_back(num);
				}
			}

			///ID: for Float: 8, 10, 12, 15, 17, 18, 19, 20, 42, 43, 44, 45, 46, 48, 49
			else if(n==8 || n==10 || n==12 || n==15 || n==17 || n==18 || n==19 || n==20|| n==42 || n==43 || n==44 || n==45 || n==46 || n==48 || n==49)
			{
				double num = 0;
				stringstream ss(temp);
				ss>>num;
				if(num==-1) allNumbers.push_back(0);
				else{
					int num2 = num*100.0;
					///cout<<num2<<" ";
					allNumbers.push_back(num2);
				}
			}
			else allNumbers.push_back(0);
		}
		//cout<<"Size = "<<allNumbers.size()<<endl;
		vector<int>AllNumbersBinaryVector = numbersToBinaries(allNumbers,bit); ///Same file
		poly_a_AllNumbers = VectorToPolynomial(AllNumbersBinaryVector); /// VectorToPoly.h
		pair<int, string> pi = {n,poly_a_AllNumbers};
		AllPoly.push_back(pi);
	}
	cout<<"Done"<<endl;
	return AllPoly;
}

void outputTheSecuence()
{
	string filename = "Project/csv_data.txt";
	ifstream cin(filename.c_str());
	string str;
	getline(cin,str);
	cin.ignore();
	string temp_str;
	stringstream ss(str);
	//system("clear");
	//print_example_banner("Research Project");
	cout<<endl<<"\tID\tTestName\n\t*******************************************"<<endl;
	int i=1;
	while(ss>>temp_str)
	{
		cout<<"\t"<<i++<<"\t"<<temp_str<<endl;
	}
	cout<<endl<<endl;
}


void Computation(EncryptionParameters &parms, shared_ptr< SEALContext > &context, int bit){
	outputTheSecuence(); /// AllFunction.h
	///Need to show value list
	cout<<"Select Your Choich and value\t: ";
	int choich, value;
	cin>>choich>>value;

	IntegerEncoder encoder(parms.plain_modulus());

	//Declaration:
	string extension = ".pk";
	string folder = "Project/", user="Admin/", cloud="Cloud/";
	string str_pk, str_sk;
	PublicKey public_key;
	SecretKey secret_key;

	cout<<"Input given Public Key File Name\t: ";
	cin>>str_pk;
	str_pk = folder + user + str_pk + extension;
	cout<<str_pk<<endl;
	ifstream f_pk(str_pk, ios::binary);
	public_key.load(f_pk);
	f_pk.close();


	///value theke generation korte hobe
	int n = 413; ///413 line data ace CSV te, tai eita
	vector<int> tempVector, UserNumberBinaryVector, UserNumberBinaryVector_Reverse, OneVector;
	tempVector = intToVector(value, bit, false); /// B of A+B of A+B-2ab: Hamming Distance
	for(int j=0;j<bit+2;j++) tempVector.push_back(0);
	for(int i=0;i<n;i++) UserNumberBinaryVector.insert(UserNumberBinaryVector.end(), tempVector.begin(), tempVector.end()); /// B n times
	UserNumberBinaryVector_Reverse = intToVector(value, bit, true); ///  b reverse: True means Reverse: b of 2ab of A+B-2ab: Hamming Distance
	OneVector = intToVector((1LL*1 << 62) - 1, bit, false);

	string poly_b = VectorToPolynomial(UserNumberBinaryVector); /// poly b
	string poly_b_reverse = VectorToPolynomial(UserNumberBinaryVector_Reverse); /// poly b reverse
	string poly_OneVector = VectorToPolynomial(OneVector); /// poly OneVector

	Plaintext plain_b = (Plaintext) poly_b; /// plain b
	Plaintext plain_b_reverse = (Plaintext) poly_b_reverse; /// plain b reverse
	Plaintext plain_OneVector = (Plaintext) poly_OneVector; /// plain OneVector



	Encryptor encryptor(context, public_key);
	Ciphertext Cipher_b, Cipher_b_reverse, Cipher_OneVector;
	encryptor.encrypt(plain_b, Cipher_b);
	encryptor.encrypt(plain_b_reverse, Cipher_b_reverse);
	encryptor.encrypt(plain_OneVector, Cipher_OneVector);


	//Cyphertext read
	string str_cypher = to_string(choich); /// Cipher Read by ID dot pk
	Ciphertext Cloud_cypher;
	str_cypher = folder + cloud + str_cypher + extension;
	ifstream f_cypher(str_cypher, ios::binary);
	Cloud_cypher.load(f_cypher);
	f_cypher.close();


	//Computation:
	Evaluator evaluator(context);
	Ciphertext a = Cloud_cypher, b = Cipher_b, a_Plus_b, TwoAB = Cloud_cypher, A_plus_B_minus_2AB;
	cout<< "Cloud Computation\t: ";
	//a+b calculationL:
	evaluator.multiply_inplace(a, Cipher_OneVector); // a
	evaluator.multiply_inplace(b, Cipher_OneVector); // b
	a_Plus_b = a;
	evaluator.add_inplace(a_Plus_b, b); // a + b

	//ab calculation
	evaluator.multiply_inplace(TwoAB, Cipher_b_reverse); // ab
	evaluator.add_inplace(TwoAB,TwoAB); // 2ab

	A_plus_B_minus_2AB = a_Plus_b;
	evaluator.sub_inplace(A_plus_B_minus_2AB, TwoAB);
	cout<<"Done"<<endl;

	///User End: (sending cloud mycypher to User and Decrypt)
	//secret key read: for Decreption
	cout<<"Input Given Secret Key File Name\t: ";
	cin>>str_sk;
	str_sk = folder + user + str_sk + extension;
	ifstream f_sk(str_sk, ios::binary);
	secret_key.load(f_sk);
	f_sk.close();

	//Decryption
	Decryptor decryptor(context, secret_key);
	Plaintext plainDecrypt;
	decryptor.decrypt(A_plus_B_minus_2AB, plainDecrypt);
	///cout<<"result = "<<plainDecrypt.to_string()<<endl;


	vector<int> power;
	for(int i=n;i>0;i--){
		power.push_back((2*(i-1)*(bit+1)) + (bit+1));
	}

	///Result Count
	int ans = 0;
	for(int i=0;i<n;i++) if(!isPowerExist(plainDecrypt.to_string(), power[i])) ans++;
	cout<<ans<<" Number Matched"<<endl;



}


void CountTheMatch(vector<bool> &power2, string poly, int &ans2){
	string term, intStr;
	stringstream ss(poly);
	int i=1;
	while(ss>>term){
		i++;
		if(i&1) continue;
		for(int i=0; i<term.size(); i++){
			if(term[i]=='^') {
				intStr = term.substr(i+1,term.size()-i);
				int p = stoi(intStr);
				if(power2[p]) {
					power2[p] = false;
					ans2--;
				}
				break;
			}
		}
	}
}

pair<vector<bool>,int> Computation2(EncryptionParameters &parms, shared_ptr< SEALContext > &context, int bit, PublicKey &public_key, SecretKey &secret_key, int n, int choich, int value){///n: number of row(CSV) , choich: ID
	IntegerEncoder encoder(parms.plain_modulus());
	double time;
	double total_Time = 0;
	clock_t begin_time = clock();

	///value theke generation korte hobe
	vector<int> tempVector, UserNumberBinaryVector, UserNumberBinaryVector_Reverse, OneVector;
	tempVector = intToVector(value, bit, false); /// B of A+B of A+B-2ab: Hamming Distance
	for(int j=0;j<bit+2;j++) tempVector.push_back(0);
	for(int i=0;i<n;i++) UserNumberBinaryVector.insert(UserNumberBinaryVector.end(), tempVector.begin(), tempVector.end()); /// B n times
	UserNumberBinaryVector_Reverse = intToVector(value, bit, true); ///  b reverse: True means Reverse: b of 2ab of A+B-2ab: Hamming Distance
	OneVector = intToVector((1LL*1 << 62) - 1, bit, false);

	string poly_b = VectorToPolynomial(UserNumberBinaryVector); /// poly b
	string poly_b_reverse = VectorToPolynomial(UserNumberBinaryVector_Reverse); /// poly b reverse
	string poly_OneVector = VectorToPolynomial(OneVector); /// poly OneVector

	Plaintext plain_b = (Plaintext) poly_b; /// plain b
	Plaintext plain_b_reverse = (Plaintext) poly_b_reverse; /// plain b reverse
	Plaintext plain_OneVector = (Plaintext) poly_OneVector; /// plain OneVector

	Encryptor encryptor(context, public_key);
	Ciphertext Cipher_b, Cipher_b_reverse, Cipher_OneVector;
	cout<<"\n\nYour value and some temporary things Encryption (By Public Key)\t\t\t: ";
	begin_time = clock();
	encryptor.encrypt(plain_b, Cipher_b);
	encryptor.encrypt(plain_b_reverse, Cipher_b_reverse);
	encryptor.encrypt(plain_OneVector, Cipher_OneVector);
	time = float( clock () - begin_time )/CLOCKS_PER_SEC;
	total_Time += time;
	cout<<"Done & \nNeeded time\t: "<<time<<"s"<<endl;

	//Cyphertext read
	string extension = ".pk";
	string folder = "Project/", user="Admin/", cloud="Cloud/";
	string str_cypher = to_string(choich); /// Cipher Read by ID dot pk
	Ciphertext Cloud_cypher;
	str_cypher = folder + cloud + str_cypher + extension;
	cout<<"Reading Cipher(Based on your Category with ID)\t\t\t\t\t: ";
	begin_time = clock();
	ifstream f_cypher(str_cypher, ios::binary);
	Cloud_cypher.load(f_cypher);
	f_cypher.close();
	time = float( clock () - begin_time )/CLOCKS_PER_SEC;
	total_Time += time;
	cout<<"Done & \nNeeded time\t: "<<time<<"s"<<endl;


	//Computation:
	Evaluator evaluator(context);
	Ciphertext a = Cloud_cypher, b = Cipher_b, a_Plus_b, TwoAB = Cloud_cypher, A_plus_B_minus_2AB;
	cout<< "Cloud Computation\t\t\t\t\t\t\t\t: ";
	//a+b calculation:
	begin_time = clock();
	evaluator.multiply_inplace(a, Cipher_OneVector); // a = 413 * 32 = 13216 bits
	evaluator.multiply_inplace(b, Cipher_OneVector); // b
	a_Plus_b = a;
	evaluator.add_inplace(a_Plus_b, b); // a + b

	//ab calculation
	evaluator.multiply_inplace(TwoAB, Cipher_b_reverse); // ab
	evaluator.add_inplace(TwoAB,TwoAB); // 2ab

	A_plus_B_minus_2AB = a_Plus_b;
	evaluator.sub_inplace(A_plus_B_minus_2AB, TwoAB);
	time = float( clock () - begin_time )/CLOCKS_PER_SEC;
	total_Time += time;
	cout<<"Done & \nNeeded time\t: "<<time<<"s"<<endl;


	//Decryption
	Decryptor decryptor(context, secret_key);
	Plaintext plainDecrypt;
	cout<<"Result Decryption (by secret key)\t\t\t\t\t\t: ";
	begin_time = clock();
	decryptor.decrypt(A_plus_B_minus_2AB, plainDecrypt);
	time = float( clock () - begin_time )/CLOCKS_PER_SEC;
	total_Time += time;
	cout<<"Done & \nNeeded time\t: "<<time<<"s"<<endl;

	cout<<"Equal Checking on Polynomial\t\t\t\t\t\t\t: ";
	begin_time = clock();
	int tt = 2*n*(bit+2)+bit+2;
	vector<bool> power2(tt,false);
	int ans2 = 0;
	for(int i=n;i>0;i--){
		power2[(2*(i-1)*(bit+1)) + (bit+1)] = true;
		ans2++;
	}
	CountTheMatch(power2, plainDecrypt.to_string(), ans2); ///Same File
	double time2 = float( clock () - begin_time )/CLOCKS_PER_SEC;
	total_Time += time2;
	cout<<"Done &\nNeeded time\t: "<<time2<<"s"<<endl;
	cout<<"\nTotol time\t: "<<total_Time<<"s\n"<<endl;
	return {power2, ans2};
}



bool Subthings(EncryptionParameters &parms, shared_ptr< SEALContext > &context, int bit){
	bool exit = false;
	bool ReturnHome = false;
	PublicKey public_key;
	SecretKey secret_key;
	bool got = false;
	while(1)
	{
		system("clear");
		print_example_banner2("Research Project: Computation");

		cout<<"1\tSingle Queries"<<endl;
		cout<<"2\tConjunctive Queries"<<endl;
		cout<<"3\tDisjunctive Queries"<<endl;
		cout<<"\n4\tReturn to Home"<<endl;
		cout<<"5\tExit"<<endl;
		cout<<"\nSelect Your Choich\t: ";
		int choich, i, idx, result = 0;

		//Declaration:
		string extension = ".pk";
		string folder = "Project/", user="Admin/", cloud="Cloud/";
		string str_pk, str_sk;

		pair<vector<bool>,int> pi, pi1, pi2;
		int numOfRow = 413; ///413 line data ace CSV te, tai eita
		int choich2, value2, choich3, value3;

		cin>>choich;
		switch(choich)
		{
		case 1:
			outputTheSecuence();
			cout<<"Select Your Choich and value\t: ";
			cin>>choich2>>value2;   /// need input validation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if(!got){
				cout<<"Input given Public Key File Name\t: "; /// for encrypt the value2
				cin>>str_pk;
				str_pk = folder + user + str_pk + extension;
				ifstream f_pk(str_pk, ios::binary);
				public_key.load(f_pk);
				f_pk.close(); ///public key file read done.

				cout<<"Input Given Secret Key File Name\t: ";  /// for decrypt the Ciphertext to plain
				cin>>str_sk;
				str_sk = folder + user + str_sk + extension;
				ifstream f_sk(str_sk, ios::binary);
				secret_key.load(f_sk);
				f_sk.close(); ///secret key read done
				got = true;
			}

			pi = Computation2(parms, context, bit, public_key, secret_key, numOfRow, choich2, value2); //AllFunction.h
			cout<<"Matched\t: "<<pi.second<<endl;
			break;
		case 2:
			outputTheSecuence();
			cout<<"Select Your 1st Choich and value\t: ";
			cin>>choich2>>value2;   /// need input validation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			cout<<"Select Your 2nd Choich and value\t: ";
			cin>>choich3>>value3;   /// need input validation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if(!got){
				cout<<"Input given Public Key File Name\t: "; /// for encrypt the value2
				cin>>str_pk;
				str_pk = folder + user + str_pk + extension;
				ifstream f_pk(str_pk, ios::binary);
				public_key.load(f_pk);
				f_pk.close(); ///public key file read done.

				cout<<"Input Given Secret Key File Name\t: ";  /// for decrypt the Ciphertext to plain
				cin>>str_sk;
				str_sk = folder + user + str_sk + extension;
				ifstream f_sk(str_sk, ios::binary);
				secret_key.load(f_sk);
				f_sk.close(); ///secret key read done
				got = true;
			}
			pi1 = Computation2(parms, context, bit, public_key, secret_key, numOfRow, choich2, value2); //AllFunction.h
			cout<<"Matched\t: "<<pi1.second<<endl;
			pi2 = Computation2(parms, context, bit, public_key, secret_key, numOfRow, choich3, value3); //AllFunction.h
			cout<<"Matched\t: "<<pi2.second<<endl;

			for(i=1; i<=numOfRow; i++){
				idx = (2*(i-1)*(bit+1)) + (bit+1);
				if(pi1.first[idx] && pi2.first[idx]) result++;
			}
			cout<<"Total Matched\t: "<<result<<endl;
			break;
		case 3:
			outputTheSecuence();
			cout<<"Select Your 1st Choich and value\t: ";
			cin>>choich2>>value2;   /// need input validation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			cout<<"Select Your 2nd Choich and value\t: ";
			cin>>choich3>>value3;   /// need input validation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if(!got){
				cout<<"Input given Public Key File Name\t: "; /// for encrypt the value2
				cin>>str_pk;
				str_pk = folder + user + str_pk + extension;
				ifstream f_pk(str_pk, ios::binary);
				public_key.load(f_pk);
				f_pk.close(); ///public key file read done.

				cout<<"Input Given Secret Key File Name\t: ";  /// for decrypt the Ciphertext to plain
				cin>>str_sk;
				str_sk = folder + user + str_sk + extension;
				ifstream f_sk(str_sk, ios::binary);
				secret_key.load(f_sk);
				f_sk.close(); ///secret key read done
				got = true;
			}
			pi1 = Computation2(parms, context, bit, public_key, secret_key, numOfRow, choich2, value2); //AllFunction.h
			cout<<"Matched\t: "<<pi1.second<<endl;
			pi2 = Computation2(parms, context, bit, public_key, secret_key, numOfRow, choich3, value3); //AllFunction.h
			cout<<"Matched\t: "<<pi2.second<<endl;

			for(i=1; i<=numOfRow; i++){
				idx = (2*(i-1)*(bit+1)) + (bit+1);
				if(pi1.first[idx] || pi2.first[idx]) result++;
			}
			cout<<"Total Matched\t: "<<result<<endl;

			break;
		case 4:
			ReturnHome = true;
			cout<<"Return to Home"<<endl;
			break;
		case 5:
			exit = true;
			cout<<"Exit"<<endl;
			break;
		default:
			cout<<"Plz Insert Valid Character"<<endl;
			break;
		}
		if(exit || ReturnHome) break;
		cout<<"\n\nPress any character to Continue\t: ";
		getchar();
		cin.ignore();
	}
	return exit;
}
