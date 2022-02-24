#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <memory>
#include <limits>
#include <complex>
#include <iterator>

#include "seal/seal.h"

///My Header File:
#include "Document/Document.h"
using namespace std;
using namespace seal;


void print_example_banner(string title)
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

void print_parameters(const shared_ptr<SEALContext> &context)
{
    auto &context_data = *context->context_data();

    /*
    Which scheme are we using?
    */
    string scheme_name;
    switch (context_data.parms().scheme())
    {
    case scheme_type::BFV:
        scheme_name = "BFV";
        break;
    case scheme_type::CKKS:
        scheme_name = "CKKS";
        break;
    default:
        throw invalid_argument("unsupported scheme");
    }

    cout << "/ Encryption parameters:" << endl;
    cout << "| scheme: " << scheme_name << endl;
    cout << "| poly_modulus_degree: " <<
        context_data.parms().poly_modulus_degree() << endl;

    /*
    Print the size of the true (product) coefficient modulus.
    */
    cout << "| coeff_modulus size: " << context_data.
        total_coeff_modulus().significant_bit_count() << " bits" << endl;

    /*
    For the BFV scheme print the plain_modulus parameter.
    */
    if (context_data.parms().scheme() == scheme_type::BFV)
    {
        cout << "| plain_modulus: " << context_data.
            parms().plain_modulus().value() << endl;
    }

    cout << "\\ noise_standard_deviation: " << context_data.
        parms().noise_standard_deviation() << endl;
    cout << endl;
}

template<typename T>
void print_vector(vector<T> vec, size_t print_size = 4, int prec = 3)
{
    /*
    Save the formatting information for std::cout.
    */
    ios old_fmt(nullptr);
    old_fmt.copyfmt(cout);

    size_t slot_count = vec.size();

    cout << fixed << setprecision(prec) << endl;
    if(slot_count <= 2 * print_size)
    {
        cout << "    [";
        for (size_t i = 0; i < slot_count; i++)
        {
            cout << " " << vec[i] << ((i != slot_count - 1) ? "," : " ]\n");
        }
    }
    else
    {
        vec.resize(max(vec.size(), 2 * print_size));
        cout << "    [";
        for (size_t i = 0; i < print_size; i++)
        {
            cout << " " << vec[i] << ",";
        }
        if(vec.size() > 2 * print_size)
        {
            cout << " ...,";
        }
        for (size_t i = slot_count - print_size; i < slot_count; i++)
        {
            cout << " " << vec[i] << ((i != slot_count - 1) ? "," : " ]\n");
        }
    }
    cout << endl;

    /*
    Restore the old std::cout formatting.
    */
    cout.copyfmt(old_fmt);
}


int main()
{
	print_example_banner("Privacy Preserving Plagiarism");

	EncryptionParameters parms(scheme_type::BFV);

	parms.set_poly_modulus_degree(1<<15);  ///taking more time
	parms.set_coeff_modulus(coeff_modulus_128(1<<15)); /// taking more time
	parms.set_plain_modulus(1ll<<20);

	auto context = SEALContext::Create(parms);

	print_parameters(context);

	IntegerEncoder encoder(parms.plain_modulus());

	KeyGenerator keygen(context);
    PublicKey public_key = keygen.public_key();
    SecretKey secret_key = keygen.secret_key();

	Encryptor encryptor(context, public_key);

	Evaluator evaluator(context);

	Decryptor decryptor(context, secret_key);

	string file1, file2;
	cout<<"Enter File name From bin/Documents folder without it extension:\n1st file name: ";
	cin>>file1;
	cout<<"2nd file name: ";
	cin>>file2;
	cout<<endl;

	Plaintext ourReversePlaintext, cloudForwardPlaintext;
	///reverse:
	pair<string, long long int> FirstFile = documentToReversePolynomial("Documents/"+file1+".txt");
	ourReversePlaintext = (Plaintext) FirstFile.first;
//	cout<<ourReversePlaintext.to_string()<<endl;
	///forward
	pair<string, long long int> SecondFile =  documentToPolynomial("Documents/"+file2+".txt");
	cloudForwardPlaintext = (Plaintext) SecondFile.first;

	//clock();


	Ciphertext cloudForwardCiphertext, ourReverseCiphertext;
	cout << "Encrypting ourReversePlaintext: ";
	encryptor.encrypt(ourReversePlaintext, ourReverseCiphertext);
	cout << "Done" << endl;

	cout << "Encrypting cloudForwardPlaintext: ";
	encryptor.encrypt(cloudForwardPlaintext, cloudForwardCiphertext);
	cout << "Done" << endl<<endl;


	///Multiplication:
	cout<< "Calculaion (our reverse * cloud forward): ";
	evaluator.multiply_inplace(ourReverseCiphertext, cloudForwardCiphertext); // A.B
	cout<<"Done"<<endl<<endl;

	///Result Decreaption:
	Plaintext abPlaintext;
	cout << "Decrypting result (our reverse * cloud forward): ";
	decryptor.decrypt(ourReverseCiphertext, abPlaintext);
	cout << "Done" << endl;

	///Result:
	int AB = CoefficientforDocument(abPlaintext.to_string());
	int AA = FirstFile.second, BB = SecondFile.second;

	cout <<"\n\nResult with our Protocol: "<<endl;
	cout<<"AB = "<<AB<<", AA = "<<AA<<", BB = "<<BB<<endl;
	cout<<"CosX = "<< (AB*1.0) / (1.0 * (AA+BB-AB)) <<endl;

	cout<<"\n\nManual Result: "<<endl;
	ManualResult(file1, file2);
	cout<<endl<<endl;

	return 0;
}
