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

///My header file =>
#include "Document/documentToPolynolomial.h"

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
	///freopen("out2.txt","w",stdout);
	print_example_banner("Example: BFV Basics I");

	EncryptionParameters parms(scheme_type::BFV);

	parms.set_poly_modulus_degree(1<<15);
	parms.set_coeff_modulus(coeff_modulus_128(1<<15));
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

	///int value1 = 5;
    ///Plaintext plain1 = encoder.encode(value1);
    ///Plaintext plain1 = (Plaintext) documentToPolynomial("Document.txt");
    Plaintext plain1 = (Plaintext) "1x^27466 + 1x^24711 + 2x^20677";

    ///cout<<documentToPolynomial("Document.txt")<<endl<<endl;
    ///cout<<documentToReversePolynomial("Document.txt")<<endl<<endl;

    cout << "Encoded 1st document as polynomial " << plain1.to_string()<< " (plain1)" << endl;

    ///int value2 = 3;
    ///Plaintext plain2 = encoder.encode(value2);
	Plaintext plain2 = (Plaintext) "2x^11932 + 1x^7898 + 1x^5143";
    cout << "Encoded 2nd document as polynomial " << plain2.to_string()<< " (plain2)" << endl;

	Ciphertext encrypted1, encrypted2;
    cout << "Encrypting plain1: ";
    encryptor.encrypt(plain1, encrypted1);
    cout << "Done (encrypted1)" << endl;

    cout << "Encrypting plain2: ";
    encryptor.encrypt(plain2, encrypted2);
    cout << "Done (encrypted2)" << endl;

    ///cout << "Noise budget in encrypted1: "<< decryptor.invariant_noise_budget(encrypted1) << " bits" << endl;
    ///cout << "Noise budget in encrypted2: "<< decryptor.invariant_noise_budget(encrypted2) << " bits" << endl;

	///evaluator.negate_inplace(encrypted1); ///negative banaice
    ///cout << "Noise budget in -encrypted1: "<< decryptor.invariant_noise_budget(encrypted1) << " bits" << endl;

	///evaluator.add_inplace(encrypted1, encrypted2); /// 2tar add first tay joma hoy
	///cout << "Noise budget in -encrypted1 + encrypted2: "<< decryptor.invariant_noise_budget(encrypted1) << " bits" << endl;

	evaluator.multiply_inplace(encrypted1, encrypted2); /// 2tar multiply 1st tay joma hoy.
	///cout << "Noise budget in (-encrypted1 + encrypted2) * encrypted2: "<< decryptor.invariant_noise_budget(encrypted1) << " bits" << endl;

	Plaintext plain_result;
    cout << "Decrypting result: ";
    decryptor.decrypt(encrypted1, plain_result);
    cout << "Done" << endl;

	cout << "Plaintext polynomial: " << plain_result.to_string() << endl;

	///cout << "Decoded integer: " << encoder.decode_int32(plain_result) << endl;

	return 0;
}
