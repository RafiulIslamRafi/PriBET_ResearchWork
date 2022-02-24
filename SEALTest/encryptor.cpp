#include "CppUnitTest.h"
#include "seal/context.h"
#include "seal/encryptor.h"
#include "seal/decryptor.h"
#include "seal/keygenerator.h"
#include "seal/batchencoder.h"
#include "seal/ckks.h"
#include "seal/encoder.h"
#include "seal/defaultparams.h"
#include <cstdint>
#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(EncryptorTest)
    {
    public:
        TEST_METHOD(FVEncryptDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_noise_standard_deviation(3.19);            
            parms.set_plain_modulus(plain_modulus);
            {
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus({ small_mods_40bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);

                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x12345678ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(1), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(1ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(2), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(2ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFD), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFDULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFE), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFEULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFF), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFFULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(314159265), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(314159265ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
            }
            {
                parms.set_poly_modulus_degree(128);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);

                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x12345678ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(1), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(1ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(2), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(2ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFD), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFDULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFE), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFEULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFF), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFFULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(314159265), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(314159265ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
            }

            {
                parms.set_poly_modulus_degree(256);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1), small_mods_40bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);

                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x12345678ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(1), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(1ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(2), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(2ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFD), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFDULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFE), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFEULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x7FFFFFFFFFFFFFFF), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(0x7FFFFFFFFFFFFFFFULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(314159265), encrypted);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(314159265ULL, encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
            }
        }

        TEST_METHOD(CKKSEncryptDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            parms.set_noise_standard_deviation(3.19);
            {
                //input consists of ones
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1), small_mods_40bit(2), small_mods_40bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 1.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1 << 16);

                encoder.encode(input, parms.parms_id(), delta, plain);
                encryptor.encrypt(plain, encrypted);

                //check correctness of encryption
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
                
                decryptor.decrypt(encrypted, plainRes);
                encoder.decode(plainRes, output);

                for (int i = 0; i < slot_size; i++)
                {
                    auto tmp = abs(input[i].real() - output[i].real());
                    Assert::IsTrue(tmp < 0.5);
                }
            }
            {
                //input consists of zeros
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1), small_mods_40bit(2), small_mods_40bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1 << 16);

                encoder.encode(input, parms.parms_id(), delta, plain);
                encryptor.encrypt(plain, encrypted);

                //check correctness of encryption
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                decryptor.decrypt(encrypted, plainRes);
                encoder.decode(plainRes, output);

                for (int i = 0; i < slot_size; i++)
                {
                    auto tmp = abs(input[i].real() - output[i].real());
                    Assert::IsTrue(tmp < 0.5);
                }
            }
            {
                // Input is a random mix of positive and negative integers
                int slot_size = 64;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2)});
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size);
                std::vector<std::complex<double>> output(slot_size);

                srand(static_cast<int>(time(NULL)));
                int input_bound = 1 << 30;
                const double delta = static_cast<double>(1ULL << 50);

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = pow(-1.0, rand() % 2) * static_cast<double>(rand() % input_bound);
                    }

                    encoder.encode(input, parms.parms_id(), delta, plain);
                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(input[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                // Input is a random mix of positive and negative integers
                int slot_size = 32;
                parms.set_poly_modulus_degree(128);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size);
                std::vector<std::complex<double>> output(slot_size);

                srand(static_cast<int>(time(NULL)));
                int input_bound = 1 << 30;
                const double delta = static_cast<double>(1ULL << 60);

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = pow(-1.0, rand() % 2) * static_cast<double>(rand() % input_bound);
                    }

                    encoder.encode(input, parms.parms_id(), delta, plain);
                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plain, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(input[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                // Encrypt at lower level
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_40bit(0), 
                    small_mods_40bit(1), small_mods_40bit(2), 
                    small_mods_40bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 1.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1 << 16);

                auto first_context_data = context->context_data();
                Assert::IsNotNull(first_context_data.get());
                auto second_context_data = first_context_data->next_context_data();
                Assert::IsNotNull(second_context_data.get());
                auto second_parms_id = second_context_data->parms().parms_id();

                encoder.encode(input, second_parms_id, delta, plain);
                encryptor.encrypt(plain, encrypted);

                // Check correctness of encryption
                Assert::IsTrue(encrypted.parms_id() == second_parms_id);

                decryptor.decrypt(encrypted, plainRes);
                encoder.decode(plainRes, output);

                for (int i = 0; i < slot_size; i++)
                {
                    auto tmp = abs(input[i].real() - output[i].real());
                    Assert::IsTrue(tmp < 0.5);
                }
            }
        }
    };
}
