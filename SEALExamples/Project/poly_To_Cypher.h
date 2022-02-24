#include<bits/stdc++.h>
#include "seal/seal.h"

using namespace std;
using namespace seal;

void polyToCypherStore(vector<pair<int, string> > &AllPoly, EncryptionParameters &parms, shared_ptr< SEALContext > &context)
{
	//Declaration:
	string extension = ".pk";
	string folder = "Project/", admin="Admin/", cloud="Cloud/";
	string str_pk, str_sk;
	PublicKey public_key;
	SecretKey secret_key;

	cout<<endl<<endl<<"Making Polynomical to CypherText"<<endl;
	cout<<"***********************************"<<endl;

	cout<<"Already have your Public or Privet key? [Y/y | N/n]\t: ";
	cin.ignore();
	char ch;
	cin>>ch;
	if(ch=='y' || ch=='Y')
	{
		cout<<"Input Your Public Key File Name to Encript to polynomial to CypheText\t: ";
		cin>>str_pk;
		cout<<"Your Public Key Read: ";
		clock_t t1 = clock();
		str_pk = folder + admin + str_pk + extension;
		ifstream f_pk(str_pk, ios::binary);
		public_key.load(f_pk);
		f_pk.close();
		cout <<"Done & time: "<<float( clock () - t1 )/CLOCKS_PER_SEC<<" s"<<endl<<endl;

	}
	else
	{
		clock_t t2 = clock();
		KeyGenerator keygen(context);
		public_key = keygen.public_key();
		secret_key = keygen.secret_key();
		cout <<"Public and Secret key generation time: "<<float( clock () - t2 )/CLOCKS_PER_SEC<<" s"<<endl<<endl;

		cout<<"Give the file name of Public Key and Secret Key (Separate by Space & Without Extension)\t: ";
		cin>>str_pk;
		cout<<"Your Public Key Store: ";
		clock_t t3 = clock();
		str_pk = folder + admin + str_pk + extension;
		ofstream f_pk(str_pk, ios::binary);
		public_key.save(f_pk);
		f_pk.close();
		cout<<"done & time: "<<float( clock () - t3 )/CLOCKS_PER_SEC<<" s"<<endl<<endl;;

		///Store Public Key
		cin>>str_sk;
		cout<<"Your Secret Key Store: ";
		clock_t t4 = clock();
		str_sk = folder + admin + str_sk + extension;
		ofstream f_sk(str_sk, ios::binary);
		secret_key.save(f_sk);
		f_sk.close();
		cout<<"Done & time: "<<float( clock () - t4 )/CLOCKS_PER_SEC<<" s"<<endl<<endl;;
		//cout<<"Public and Privet Key Generated and Stored Successfully"<<endl<<endl;
	}

	Encryptor encryptor(context, public_key);
	Ciphertext cypher;
	string Cypher_file_name, str_cypher;
	Plaintext plain;
	double AllEncryptionTime = 0, AllStorageTime = 0;
	for(int i=0; i<AllPoly.size(); i++)
	{
		plain = (Plaintext) AllPoly[i].second;
		cout<<"Polynomial to CypherText Encryption: ";
		clock_t t5 = clock();
		encryptor.encrypt(plain, cypher);
		double time = float( clock () - t5 )/CLOCKS_PER_SEC;
		cout<<"Done & time: "<<time<<" s";

		cout<<" and Cyphertext store: ";
		///Cyphertext Store:
		clock_t t6 = clock();
		Cypher_file_name = to_string(AllPoly[i].first);
		str_cypher = folder + cloud + Cypher_file_name + extension;
		ofstream f_cypher(str_cypher, ios::binary);
		cypher.save(f_cypher);
		f_cypher.close();
		double time6 = float( clock () - t6 )/CLOCKS_PER_SEC;
		cout<<"Done & time: "<<time6<<" s"<<endl;
		AllEncryptionTime += time;
		AllStorageTime += time6;
	}
	cout<<"\nTotal Encryption time: "<<AllEncryptionTime<<" s and Storage time: "<<AllStorageTime<<" s"<<endl;
	cout<<"All CypherText Stored Successfully\tRemember Your Public and Secret Key file name."<<endl;
	getchar();
}


void CypherReadComputationToPoly(string &polynomial)
{
	EncryptionParameters parms(scheme_type::BFV);
	parms.set_poly_modulus_degree(1 << 15);
	parms.set_coeff_modulus(coeff_modulus_128(1 << 15));
	parms.set_plain_modulus(1 << 15);

	auto context = SEALContext::Create(parms);
	IntegerEncoder encoder(parms.plain_modulus());

	//Declaration:
	string extension = ".pk";
	string folder = "Project/";
	string str_pk, str_sk;
	PublicKey public_key;
	SecretKey secret_key;

	///User end: Sending Data time
	//public key read: for BOB's poly encryption
	cout<<"Input given Public Key File Name\t: ";
	cin>>str_pk;
	str_pk = folder + str_pk + extension;
	ifstream f_pk(str_pk, ios::binary);
	public_key.load(f_pk);
	f_pk.close();

	Plaintext plain = (Plaintext) polynomial;
	Encryptor encryptor(context, public_key);
	Ciphertext mycypher;
	encryptor.encrypt(plain, mycypher);


	///Cloud: (sending mycypher to cloud and calculation)
	//Cyphertext read
	cout<<"Input Cyphertext File Name\t: ";
	string str_cypher;
	Ciphertext cypher;
	cin>>str_cypher;
	str_cypher = folder + str_cypher + extension;
	ifstream f_cypher(str_cypher, ios::binary);
	cypher.load(f_cypher);
	f_cypher.close();

	//Computation:
	Evaluator evaluator(context);
	evaluator.add_inplace(mycypher, cypher);

	///User End: (sending cloud mycypher to User and Decrypt)
	//secret key read: for Decreption
	cout<<"Input Given Secret Key File Name\t: ";
	cin>>str_sk;
	str_sk = folder + str_sk + extension;
	ifstream f_sk(str_sk, ios::binary);
	secret_key.load(f_sk);
	f_sk.close();

	//Decryption
	Decryptor decryptor(context, secret_key);
	Plaintext plainDecrypt;
	decryptor.decrypt(mycypher, plainDecrypt);

	cout<<"result = "<<plainDecrypt.to_string()<<endl;
}




