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

	/// Taking Input
	int n,num;
	vector<int> SomeNumbersInteger;
	int OneNumberInteger;
	int bit=3; /// works on how many bits?
	cout<<"How many Number you want to input\t: ";
	cin>>n;
	cout<<"Input "<<n<<" Numbers\t\t\t\t: ";
	for(int i=0;i<n;i++){
		cin>>num;
		SomeNumbersInteger.push_back(num);
	}
	cout<<"Input your second Number\t\t: ";
	cin>>OneNumberInteger;

	/// integer to Binary vector conversion:
	vector<int> tempVector, AllNumbersBinaryVector, OneNumberBinaryVector, OneNumberBinaryVector_Reverse, OneVector;
	OneVector = intToVector((1LL*1 << 62) - 1, bit, false); // One Vector: cut from a 32 bit number
	for(int i=0;i<n;i++){
		tempVector = intToVector(SomeNumbersInteger[i], bit, false);
		if(AllNumbersBinaryVector.size()){
			for(int j=0;j<bit+2;j++) AllNumbersBinaryVector.push_back(0);
		}
		AllNumbersBinaryVector.insert(AllNumbersBinaryVector.end(), tempVector.begin(), tempVector.end()); ///a of 2ab of A+B-2ab: hamming distance
	}

	tempVector = intToVector(OneNumberInteger, bit, false); /// B of A+B of A+B-2ab: Hamming Distance
	for(int j=0;j<bit+2;j++) tempVector.push_back(0);
	for(int i=0;i<n;i++) OneNumberBinaryVector.insert(OneNumberBinaryVector.end(), tempVector.begin(), tempVector.end()); /// B n times

	OneNumberBinaryVector_Reverse = intToVector(OneNumberInteger, bit, true); /// b reverse: True means Reverse: b of 2ab of A+B-2ab: Hamming Distance

	/// vector to string conversion:
	string poly_a_AllNumbers = VectorToPolynomial(AllNumbersBinaryVector); /// a
	string poly_b = VectorToPolynomial(OneNumberBinaryVector); /// b
	string poly_b_Reverse = VectorToPolynomial(OneNumberBinaryVector_Reverse); /// b_reverse
	string poly_OneVector = VectorToPolynomial(OneVector); /// one vector

	/// String to Plaintext conversion:
	Plaintext Plain_a_AllNumbers = (Plaintext) poly_a_AllNumbers; // a
	Plaintext Plain_b = (Plaintext) poly_b; // b
	Plaintext Plain_b_Reverse = (Plaintext) poly_b_Reverse; // br
	Plaintext Plain_OneVector = (Plaintext) poly_OneVector; // oneVector

	cout<<"a = "<<Plain_a_AllNumbers.to_string()<<endl;
	cout<<"b = "<<Plain_b.to_string()<<endl;
	cout<<"br = "<<Plain_b_Reverse.to_string()<<endl;
	cout<<"1vec = "<<Plain_OneVector.to_string()<<endl;

	///Encryption
	clock_t begin_time = clock(); ///time starting
	Ciphertext Cipher_a_AllNumbers, Cipher_b, Cipher_b_Reverse, Cipher_OneVector;
	cout <<endl<< "Encrypting ourReversePlaintext: ";
	encryptor.encrypt(Plain_a_AllNumbers, Cipher_a_AllNumbers);
	encryptor.encrypt(Plain_b, Cipher_b);
	encryptor.encrypt(Plain_b_Reverse, Cipher_b_Reverse);
	encryptor.encrypt(Plain_OneVector, Cipher_OneVector);
	cout << "Done" << endl;
	cout <<"Encryption Time: "<<float( clock () - begin_time )/CLOCKS_PER_SEC<<" ms"<<endl<<endl; ///endting time



	///Cloud Computation:
	begin_time = clock(); ///time starting
	Ciphertext a = Cipher_a_AllNumbers, b = Cipher_b, a_Plus_b, TwoAB = Cipher_a_AllNumbers, A_plus_B_minus_2AB;
	cout<< "Cloud Computation : ";
	//a+b calculationL:
	evaluator.multiply_inplace(a, Cipher_OneVector); // a
	evaluator.multiply_inplace(b, Cipher_OneVector); // b
	a_Plus_b = a;
	evaluator.add_inplace(a_Plus_b, b); // a + b

	//ab calculation
	evaluator.multiply_inplace(TwoAB, Cipher_b_Reverse); // ab
	evaluator.add_inplace(TwoAB,TwoAB); // 2ab

	A_plus_B_minus_2AB = a_Plus_b;
	evaluator.sub_inplace(A_plus_B_minus_2AB, TwoAB); // a+b - 2ab
	cout<<"Done"<<endl;
	cout <<"Cloud Computing Time: "<<float( clock () - begin_time )/CLOCKS_PER_SEC<<" ms"<<endl<<endl; ///endting time


	///Result Decreaption:
	begin_time = clock(); ///time starting
	Plaintext result;
	cout << "Decrypting result: ";
	decryptor.decrypt(A_plus_B_minus_2AB, result);
	cout << "Done" << endl;
	cout <<"Result Decryption time: "<<float( clock () - begin_time )/CLOCKS_PER_SEC<<" ms"<<endl<<endl; ///endting time


	///Result:
	cout<<"a+b - 2ab = "<<result.to_string()<<endl<<endl;

	/// power list:
	vector<int> power;
	for(int i=n;i>0;i--){
		power.push_back((2*(i-1)*(bit+1)) + (bit+1));
	}

	///Result Count
	int ans = 0;
	for(int i=0;i<n;i++) if(!isPowerExist(result.to_string(), power[i])) ans++;
	cout<<ans<<" Number Matched"<<endl;
	return 0;
}

