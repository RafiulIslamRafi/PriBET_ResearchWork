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
#include "Compare/IntegerToVector.h"
#include "Compare/VectorToPoly.h"
#include "Compare/CoefficientChecking.h"
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
	print_example_banner("Equiality Compare By PET protocol");
	EncryptionParameters parms(scheme_type::BFV);
	parms.set_poly_modulus_degree(2048);
	parms.set_coeff_modulus(coeff_modulus_128(2048));
	parms.set_plain_modulus(1 << 8);

	auto context = SEALContext::Create(parms);

	print_parameters(context);

	IntegerEncoder encoder(parms.plain_modulus());

	KeyGenerator keygen(context);
    PublicKey public_key = keygen.public_key();
    SecretKey secret_key = keygen.secret_key();

	Encryptor encryptor(context, public_key);

	Evaluator evaluator(context);

	Decryptor decryptor(context, secret_key);

	int num1;
	int num2;
	int bit=8; /// works on how many bits?
	cout<<"Input your first Number\t\t: ";
	cin>>num1;
	cout<<"Input your second Number\t: ";
	cin>>num2;
	/// integer to vector conversion:
	vector<int> vec1, vec2, vec3, vec4;
	vec1 = intToVector(num1, bit, false); // a
	vec2 = intToVector(num2, bit, false); // b
	vec3 = intToVector(num2, bit, true); // b reverse
	vec4 = intToVector((1LL*1 << 62) - 1, bit, false); // One Vector: cut from a 32 bit number

	/// vector to string conversion:
	string str1 = VectorToPolynomial(vec1); // a
	string str2 = VectorToPolynomial(vec2); // b
	string str3 = VectorToPolynomial(vec3); // br
	string str4 = VectorToPolynomial(vec4); // one vector

	/// String to Plaintext conversion:
	Plaintext Plain1 = (Plaintext) str1; // a
	Plaintext Plain2 = (Plaintext) str2; // b
	Plaintext Plain3 = (Plaintext) str3; // br
	Plaintext Plain4 = (Plaintext) str4; // oneVector

	cout<<"a = "<<Plain1.to_string()<<endl;
	cout<<"b = "<<Plain2.to_string()<<endl;
	cout<<"br = "<<Plain3.to_string()<<endl;
	cout<<"1vec = "<<Plain4.to_string()<<endl;


	Ciphertext a, b, br, oneVector, two;
	cout << "Encrypting ourReversePlaintext: ";
	encryptor.encrypt(Plain1, a);
	encryptor.encrypt(Plain2, b);
	encryptor.encrypt(Plain3, br);
	encryptor.encrypt(Plain4, oneVector);
	cout << "Done" << endl;


	///Cloud Computation:
	Ciphertext a_Plus_b, ab = a, TwoAB, A_plus_B_minus_2AB;
	cout<< "Calculaion : ";
	//a+b calculationL:
	evaluator.multiply_inplace(a, oneVector); // a
	a_Plus_b = a;
	evaluator.multiply_inplace(b, oneVector); // b
	evaluator.add_inplace(a_Plus_b, b); // a + b

	//ab calculation
	evaluator.multiply_inplace(ab, br); // ab
	evaluator.add_inplace(ab,ab); // 2ab

	A_plus_B_minus_2AB = a_Plus_b;
	evaluator.sub_inplace(A_plus_B_minus_2AB, ab); // a+b - 2ab
	cout<<"Done"<<endl<<endl;

	///Result Decreaption:
	Plaintext aPb, aMb, Tab, result;
	cout << "Decrypting result: ";
	decryptor.decrypt(a_Plus_b, aPb);
	decryptor.decrypt(ab, aMb);
	decryptor.decrypt(A_plus_B_minus_2AB, result);
	cout << "Done" << endl;

	///Result:
	cout<<"a+b = "<<aPb.to_string()<<endl;
	cout<<"2ab = "<<aMb.to_string()<<endl;
	cout<<endl;
	cout<<"a+b - 2ab = "<<result.to_string()<<endl;


	///Poly to Result Checking: if bit+1 th term exist, then not equal
	if(isPowerExist(result.to_string(), bit+1)) cout<<"UnEqual"<<endl;
	else cout<<"Equal"<<endl;

	return 0;
}

