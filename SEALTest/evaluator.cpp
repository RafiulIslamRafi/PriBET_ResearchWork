#include "CppUnitTest.h"
#include "seal/context.h"
#include "seal/encryptor.h"
#include "seal/decryptor.h"
#include "seal/evaluator.h"
#include "seal/keygenerator.h"
#include "seal/batchencoder.h"
#include "seal/ckks.h"
#include "seal/encoder.h"
#include "seal/defaultparams.h"
#include <cstdint>
#include <string>
#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(EvaluatorTest)
    {
    public:
        TEST_METHOD(FVEncryptNegateDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(64);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Ciphertext encrypted;
            encryptor.encrypt(encoder.encode(0x12345678), encrypted);
            evaluator.negate_inplace(encrypted);
            Plaintext plain;
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<int32_t>(-0x12345678), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted);
            evaluator.negate_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<int32_t>(0), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(1), encrypted);
            evaluator.negate_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<int32_t>(-1), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-1), encrypted);
            evaluator.negate_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<int32_t>(1), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(2), encrypted);
            evaluator.negate_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<int32_t>(-2), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-5), encrypted);
            evaluator.negate_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<int32_t>(5), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptAddDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(64);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Ciphertext encrypted1;
            encryptor.encrypt(encoder.encode(0x12345678), encrypted1);
            Ciphertext encrypted2;
            encryptor.encrypt(encoder.encode(0x54321), encrypted2);
            evaluator.add_inplace(encrypted1, encrypted2);
            Plaintext plain;
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(0x12399999), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted1);
            encryptor.encrypt(encoder.encode(0), encrypted2);
            evaluator.add_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted1);
            encryptor.encrypt(encoder.encode(5), encrypted2);
            evaluator.add_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(5), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(5), encrypted1);
            encryptor.encrypt(encoder.encode(-3), encrypted2);
            evaluator.add_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<int32_t>(2), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-7), encrypted1);
            encryptor.encrypt(encoder.encode(2), encrypted2);
            evaluator.add_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<int32_t>(-5), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            Plaintext plain1 = "2x^2 + 1x^1 + 3";
            Plaintext plain2 = "3x^3 + 4x^2 + 5x^1 + 6";
            encryptor.encrypt(plain1, encrypted1);
            encryptor.encrypt(plain2, encrypted2);
            evaluator.add_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::IsTrue(plain.to_string() == "3x^3 + 6x^2 + 6x^1 + 9");
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            plain1 = "3x^5 + 1x^4 + 4x^3 + 1";
            plain2 = "5x^2 + 9x^1 + 2";
            encryptor.encrypt(plain1, encrypted1);
            encryptor.encrypt(plain2, encrypted2);
            evaluator.add_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::IsTrue(plain.to_string() == "3x^5 + 1x^4 + 4x^3 + 5x^2 + 9x^1 + 3");
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
        }

        TEST_METHOD(CKKSEncryptAddDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            parms.set_noise_standard_deviation(3.19);
            {
                //adding two zero vectors
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus(
                    { small_mods_30bit(0), small_mods_30bit(1), 
                    small_mods_30bit(2), small_mods_30bit(3), 
                    small_mods_30bit(4) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1 << 16);
                encoder.encode(input, parms.parms_id(), delta, plain);

                encryptor.encrypt(plain, encrypted);
                evaluator.add_inplace(encrypted, encrypted);

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
                //adding two random vectors 100 times
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus(
                    { small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2)});
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 30);
                const double delta = static_cast<double>(1 << 16);

                srand(static_cast<int>(time(NULL)));

                for (int expCount = 0; expCount < 100; expCount++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] + input2[i];
                    }

                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);
                    evaluator.add_inplace(encrypted1, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
                
            }
            {
                //adding two random vectors 100 times
                int slot_size = 8;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus(
                    { small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 30);
                const double delta = static_cast<double>(1 << 16);

                srand(static_cast<int>(time(NULL)));

                for (int expCount = 0; expCount < 100; expCount++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] + input2[i];
                    }

                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);
                    evaluator.add_inplace(encrypted1, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }

            }
        }
        TEST_METHOD(CKKSEncryptAddPlainDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            parms.set_noise_standard_deviation(3.19);
            {
                //adding two zero vectors
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus(
                    { small_mods_30bit(0), small_mods_30bit(1),
                    small_mods_30bit(2), small_mods_30bit(3), small_mods_30bit(4) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1 << 16);
                encoder.encode(input, parms.parms_id(), delta, plain);

                encryptor.encrypt(plain, encrypted);
                evaluator.add_plain_inplace(encrypted, plain);

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
                //adding two random vectors 50 times
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus(
                    { small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 8);
                const double delta = static_cast<double>(1ULL << 16);

                srand(static_cast<int>(time(NULL)));

                for (int expCount = 0; expCount < 50; expCount++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] + input2[i];
                    }

                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.add_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }

            }
            {
                //adding two random vectors 50 times
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus(
                    { small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                double input2;
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 8);
                const double delta = static_cast<double>(1ULL << 16);

                srand(static_cast<int>(time(NULL)));

                for (int expCount = 0; expCount < 50; expCount++)
                {
                    input2 = static_cast<double>(rand() % (data_bound*data_bound))/data_bound;
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] + input2;
                    }

                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.add_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }

            }
            {
                //adding two random vectors 50 times
                int slot_size = 8;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus(
                    { small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                double input2;
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 8);
                const double delta = static_cast<double>(1ULL << 16);

                srand(static_cast<int>(time(NULL)));

                for (int expCount = 0; expCount < 50; expCount++)
                {
                    input2 = static_cast<double>(rand() % (data_bound*data_bound)) / data_bound;
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] + input2;
                    }

                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.add_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }

            }
        }

        TEST_METHOD(CKKSEncryptSubPlainDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            parms.set_noise_standard_deviation(3.19);
            {
                //adding two zero vectors
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus(
                    { small_mods_30bit(0), small_mods_30bit(1),
                    small_mods_30bit(2), small_mods_30bit(3), small_mods_30bit(4) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1 << 16);
                encoder.encode(input, parms.parms_id(), delta, plain);

                encryptor.encrypt(plain, encrypted);
                evaluator.add_plain_inplace(encrypted, plain);

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
                //adding two random vectors 100 times
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus(
                    { small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 8);
                const double delta = static_cast<double>(1ULL << 16);

                srand(static_cast<int>(time(NULL)));

                for (int expCount = 0; expCount < 100; expCount++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] - input2[i];
                    }

                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.sub_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }

            }
            {
                //adding two random vectors 100 times
                int slot_size = 8;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus(
                    { small_mods_60bit(0), small_mods_60bit(1), small_mods_60bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 8);
                const double delta = static_cast<double>(1ULL << 16);

                srand(static_cast<int>(time(NULL)));

                for (int expCount = 0; expCount < 100; expCount++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] - input2[i];
                    }

                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.sub_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }

            }
        }

        TEST_METHOD(FVEncryptSubDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(64);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Ciphertext encrypted1;
            encryptor.encrypt(encoder.encode(0x12345678), encrypted1);
            Ciphertext encrypted2;
            encryptor.encrypt(encoder.encode(0x54321), encrypted2);
            evaluator.sub_inplace(encrypted1, encrypted2);
            Plaintext plain;
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<int32_t>(0x122F1357), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted1);
            encryptor.encrypt(encoder.encode(0), encrypted2);
            evaluator.sub_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<int32_t>(0), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted1);
            encryptor.encrypt(encoder.encode(5), encrypted2);
            evaluator.sub_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<int32_t>(-5), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(5), encrypted1);
            encryptor.encrypt(encoder.encode(-3), encrypted2);
            evaluator.sub_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<int32_t>(8), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-7), encrypted1);
            encryptor.encrypt(encoder.encode(2), encrypted2);
            evaluator.sub_inplace(encrypted1, encrypted2);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<int32_t>(-9), encoder.decode_int32(plain));
            Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptAddPlainDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(64);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Ciphertext encrypted1;
            Ciphertext encrypted2;
            Plaintext plain;
            encryptor.encrypt(encoder.encode(0x12345678), encrypted1);
            plain = encoder.encode(0x54321);
            evaluator.add_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(0x12399999), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted1);
            plain = encoder.encode(0);
            evaluator.add_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted1);
            plain = encoder.encode(5);
            evaluator.add_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(5), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(5), encrypted1);
            plain = encoder.encode(-3);
            evaluator.add_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(2), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-7), encrypted1);
            plain = encoder.encode(7);
            evaluator.add_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptSubPlainDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(64);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Ciphertext encrypted1;
            Plaintext plain;
            encryptor.encrypt(encoder.encode(0x12345678), encrypted1);
            plain = encoder.encode(0x54321);
            evaluator.sub_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(0x122F1357), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted1);
            plain = encoder.encode(0);
            evaluator.sub_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted1);
            plain = encoder.encode(5);
            evaluator.sub_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::IsTrue(static_cast<int64_t>(-5) == encoder.decode_int64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(5), encrypted1);
            plain = encoder.encode(-3);
            evaluator.sub_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::AreEqual(static_cast<uint64_t>(8), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-7), encrypted1);
            plain = encoder.encode(2);
            evaluator.sub_plain_inplace(encrypted1, plain);
            decryptor.decrypt(encrypted1, plain);
            Assert::IsTrue(static_cast<int64_t>(-9) == encoder.decode_int64(plain));
            Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptMultiplyPlainDecrypt)
        {
            {
                EncryptionParameters parms(scheme_type::BFV);
                SmallModulus plain_modulus(1 << 6);
                parms.set_poly_modulus_degree(64);
                parms.set_plain_modulus(plain_modulus);
                parms.set_coeff_modulus({ small_mods_40bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);
                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted);
                plain = encoder.encode(0x54321);
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x5FCBBBB88D78), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted);
                plain = encoder.encode(5);
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(7), encrypted);
                plain = encoder.encode(1);
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(static_cast<uint64_t>(7), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(5), encrypted);
                plain = encoder.encode(-3);
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::IsTrue(static_cast<int64_t>(-15) == encoder.decode_int64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(-7), encrypted);
                plain = encoder.encode(2);
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::IsTrue(static_cast<int64_t>(-14) == encoder.decode_int64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
            }
            {
                EncryptionParameters parms(scheme_type::BFV);
                SmallModulus plain_modulus((1ULL << 20) - 1);
                parms.set_poly_modulus_degree(64);
                parms.set_plain_modulus(plain_modulus);
                parms.set_coeff_modulus({
                    small_mods_30bit(0),
                    small_mods_60bit(0),
                    small_mods_60bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);
                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted);
                plain = "1";
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x12345678), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                plain = "5";
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x5B05B058), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
            }
            {
                EncryptionParameters parms(scheme_type::BFV);
                SmallModulus plain_modulus((1ULL << 40) - 1);
                parms.set_poly_modulus_degree(64);
                parms.set_plain_modulus(plain_modulus);
                parms.set_coeff_modulus({ 
                    small_mods_30bit(0), 
                    small_mods_60bit(0),
                    small_mods_60bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);
                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted);
                plain = "1";
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x12345678), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                plain = "5";
                evaluator.multiply_plain_inplace(encrypted, plain);
                decryptor.decrypt(encrypted, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x5B05B058), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
            }
        }

        TEST_METHOD(FVEncryptMultiplyDecrypt)
        {
            {
                EncryptionParameters parms(scheme_type::BFV);
                SmallModulus plain_modulus(1 << 6);
                parms.set_poly_modulus_degree(64);
                parms.set_plain_modulus(plain_modulus);
                parms.set_coeff_modulus({ small_mods_40bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);
                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted1);
                encryptor.encrypt(encoder.encode(0x54321), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x5FCBBBB88D78), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted1);
                encryptor.encrypt(encoder.encode(0), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted1);
                encryptor.encrypt(encoder.encode(5), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(7), encrypted1);
                encryptor.encrypt(encoder.encode(1), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(7), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(5), encrypted1);
                encryptor.encrypt(encoder.encode(-3), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::IsTrue(static_cast<int64_t>(-15) == encoder.decode_int64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x10000), encrypted1);
                encryptor.encrypt(encoder.encode(0x100), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x1000000), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
            }
            {
                EncryptionParameters parms(scheme_type::BFV);
                SmallModulus plain_modulus((1ULL << 60) - 1);
                parms.set_poly_modulus_degree(64);
                parms.set_plain_modulus(plain_modulus);
                parms.set_coeff_modulus({ 
                    small_mods_60bit(0), 
                    small_mods_60bit(1),
                    small_mods_60bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);
                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted1);
                encryptor.encrypt(encoder.encode(0x54321), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x5FCBBBB88D78), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted1);
                encryptor.encrypt(encoder.encode(0), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted1);
                encryptor.encrypt(encoder.encode(5), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(7), encrypted1);
                encryptor.encrypt(encoder.encode(1), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(7), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(5), encrypted1);
                encryptor.encrypt(encoder.encode(-3), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::IsTrue(static_cast<int64_t>(-15) == encoder.decode_int64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x10000), encrypted1);
                encryptor.encrypt(encoder.encode(0x100), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x1000000), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
            }
            {
                EncryptionParameters parms(scheme_type::BFV);
                SmallModulus plain_modulus(1 << 6);
                parms.set_poly_modulus_degree(128);
                parms.set_plain_modulus(plain_modulus);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);
                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(0x12345678), encrypted1);
                encryptor.encrypt(encoder.encode(0x54321), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x5FCBBBB88D78), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted1);
                encryptor.encrypt(encoder.encode(0), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0), encrypted1);
                encryptor.encrypt(encoder.encode(5), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(7), encrypted1);
                encryptor.encrypt(encoder.encode(1), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(7), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(5), encrypted1);
                encryptor.encrypt(encoder.encode(-3), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::IsTrue(static_cast<int64_t>(-15) == encoder.decode_int64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(0x10000), encrypted1);
                encryptor.encrypt(encoder.encode(0x100), encrypted2);
                evaluator.multiply_inplace(encrypted1, encrypted2);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(0x1000000), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted2.parms_id() == encrypted1.parms_id());
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
            }
            {
                EncryptionParameters parms(scheme_type::BFV);
                SmallModulus plain_modulus(1 << 6);
                parms.set_poly_modulus_degree(128);
                parms.set_plain_modulus(plain_modulus);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                BalancedEncoder encoder(plain_modulus);
                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());

                Ciphertext encrypted1;
                Plaintext plain;
                encryptor.encrypt(encoder.encode(123), encrypted1);
                evaluator.multiply(encrypted1, encrypted1, encrypted1);
                evaluator.multiply(encrypted1, encrypted1, encrypted1);
                decryptor.decrypt(encrypted1, plain);
                Assert::AreEqual(static_cast<uint64_t>(228886641), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
            }
        }
        TEST_METHOD(FVRelinearize)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1), small_mods_40bit(2) });
            parms.set_noise_standard_deviation(3.19);
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            RelinKeys rlk = keygen.relin_keys(60, 3);

            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Ciphertext encrypted(context);
            Ciphertext encrypted2(context);

            Plaintext plain;
            Plaintext plain2;

            plain = 0;
            encryptor.encrypt(plain, encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.relinearize_inplace(encrypted, rlk);
            decryptor.decrypt(encrypted, plain2);
            Assert::IsTrue(plain == plain2);

            encryptor.encrypt(plain, encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.relinearize_inplace(encrypted, rlk);
            decryptor.decrypt(encrypted, plain2);
            Assert::IsTrue(plain == plain2);

            plain = "1x^10 + 2";
            encryptor.encrypt(plain, encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.relinearize_inplace(encrypted, rlk);
            decryptor.decrypt(encrypted, plain2);
            Assert::IsTrue(plain2.to_string() == "1x^20 + 4x^10 + 4");

            encryptor.encrypt(plain, encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.relinearize_inplace(encrypted, rlk);
            decryptor.decrypt(encrypted, plain2);
            Assert::IsTrue(plain2.to_string() == "1x^40 + 8x^30 + 18x^20 + 20x^10 + 10");

            // Relinearization with modulus switching
            plain = "1x^10 + 2";
            encryptor.encrypt(plain, encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.relinearize_inplace(encrypted, rlk);
            evaluator.mod_switch_to_next_inplace(encrypted);
            decryptor.decrypt(encrypted, plain2);
            Assert::IsTrue(plain2.to_string() == "1x^20 + 4x^10 + 4");

            encryptor.encrypt(plain, encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.relinearize_inplace(encrypted, rlk);
            evaluator.mod_switch_to_next_inplace(encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.relinearize_inplace(encrypted, rlk);
            evaluator.mod_switch_to_next_inplace(encrypted);
            decryptor.decrypt(encrypted, plain2);
            Assert::IsTrue(plain2.to_string() == "1x^40 + 8x^30 + 18x^20 + 20x^10 + 10");
        }
        TEST_METHOD(CKKSEncryptNaiveMultiplyDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            parms.set_noise_standard_deviation(3.19);
            {
                //multiplying two zero vectors
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_30bit(0), small_mods_30bit(1), 
                    small_mods_30bit(2) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1 << 30);
                encoder.encode(input, parms.parms_id(), delta, plain);

                encryptor.encrypt(plain, encrypted);
                evaluator.multiply_inplace(encrypted, encrypted);

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
                //multiplying two random vectors
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_60bit(1)});
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1ULL << 40);

                int data_bound = (1 << 10);
                srand(static_cast<int>(time(NULL)));

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i];
                    }
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);
                    evaluator.multiply_inplace(encrypted1, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //multiplying two random vectors
                int slot_size = 16;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_60bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                const double delta = static_cast<double>(1ULL << 40);

                int data_bound = (1 << 10);
                srand(static_cast<int>(time(NULL)));

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i];
                    }
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);
                    evaluator.multiply_inplace(encrypted1, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
        }

        TEST_METHOD(CKKSEncryptMultiplyByNumberDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            parms.set_noise_standard_deviation(3.19);
            {
                //multiplying two random vectors by an integer
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_60bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                int64_t input2;
                std::vector<std::complex<double>> expected(slot_size, 0.0);

                int data_bound = (1 << 10);
                srand(static_cast<int>(time(NULL)));

                for (int iExp = 0; iExp < 50; iExp++)
                {
                    input2 = rand() % data_bound;
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * static_cast<double>(input2);
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.multiply_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //multiplying two random vectors by an integer
                int slot_size = 8;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_60bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                int64_t input2;
                std::vector<std::complex<double>> expected(slot_size, 0.0);

                int data_bound = (1 << 10);
                srand(static_cast<int>(time(NULL)));

                for (int iExp = 0; iExp < 50; iExp++)
                {
                    input2 = rand() % data_bound;
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * static_cast<double>(input2);
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.multiply_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //multiplying two random vectors by a double
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_60bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                double input2;
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 10);
                srand(static_cast<int>(time(NULL)));

                for (int iExp = 0; iExp < 50; iExp++)
                {
                    input2 = static_cast<double>(rand() % (data_bound*data_bound))
                        /static_cast<double>(data_bound);
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2;
                    }

                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.multiply_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //multiplying two random vectors by a double
                int slot_size = 16;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_60bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                Ciphertext encrypted1;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 2.1);
                double input2;
                std::vector<std::complex<double>> expected(slot_size, 2.1);
                std::vector<std::complex<double>> output(slot_size);

                int data_bound = (1 << 10);
                srand(static_cast<int>(time(NULL)));

                for (int iExp = 0; iExp < 50; iExp++)
                {
                    input2 = static_cast<double>(rand() % (data_bound*data_bound))
                        / static_cast<double>(data_bound);
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2;
                    }

                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    evaluator.multiply_plain_inplace(encrypted1, plain2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());

                    decryptor.decrypt(encrypted1, plainRes);

                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
        }

        TEST_METHOD(CKKSEncryptMultiplyRelinDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            parms.set_noise_standard_deviation(3.19);
            {
                //multiplying two random vectors 50 times
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({
                    small_mods_60bit(0), small_mods_60bit(1)});
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Ciphertext encryptedRes;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                int data_bound = 1 << 10;

                for (int round = 0; round < 50; round++)
                {
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i];
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted2.parms_id() == parms.parms_id());

                    evaluator.multiply_inplace(encrypted1, encrypted2);
                    evaluator.relinearize_inplace(encrypted1, rlk);

                    decryptor.decrypt(encrypted1, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //multiplying two random vectors 50 times
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({
                    small_mods_30bit(0), small_mods_30bit(1),
                    small_mods_30bit(2), small_mods_30bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Ciphertext encryptedRes;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                int data_bound = 1 << 10;

                for (int round = 0; round < 50; round++)
                {
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i];
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted2.parms_id() == parms.parms_id());

                    evaluator.multiply_inplace(encrypted1, encrypted2);
                    evaluator.relinearize_inplace(encrypted1, rlk);

                    decryptor.decrypt(encrypted1, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //multiplying two random vectors 50 times
                int slot_size = 2;
                parms.set_poly_modulus_degree(8);
                parms.set_coeff_modulus({
                    small_mods_30bit(0), small_mods_30bit(1),
                    small_mods_30bit(2), small_mods_30bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Ciphertext encryptedRes;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                int data_bound = 1 << 10;
                const double delta = static_cast<double>(1ULL << 40);

                for (int round = 0; round < 50; round++)
                {
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i];
                    }

                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted2.parms_id() == parms.parms_id());

                    evaluator.multiply_inplace(encrypted1, encrypted2);
                    //evaluator.relinearize_inplace(encrypted1, rlk);

                    decryptor.decrypt(encrypted1, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
        }
        TEST_METHOD(CKKSEncryptSquareRelinDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            {
                //squaring two random vectors 100 times
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({
                    small_mods_60bit(0), small_mods_60bit(1)});
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                
                int data_bound = 1 << 7;
                srand(static_cast<int>(time(NULL)));

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input[i] * input[i];
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input, parms.parms_id(), delta, plain);

                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    //evaluator.square_inplace(encrypted);
                    evaluator.multiply_inplace(encrypted, encrypted);
                    evaluator.relinearize_inplace(encrypted, rlk);

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //squaring two random vectors 100 times
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({
                    small_mods_30bit(0), small_mods_30bit(1),
                    small_mods_30bit(2), small_mods_30bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);

                int data_bound = 1 << 7;
                srand(static_cast<int>(time(NULL)));

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input[i] * input[i];
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input, parms.parms_id(), delta, plain);

                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    //evaluator.square_inplace(encrypted);
                    evaluator.multiply_inplace(encrypted, encrypted);
                    evaluator.relinearize_inplace(encrypted, rlk);

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //squaring two random vectors 100 times
                int slot_size = 16;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus({
                    small_mods_30bit(0), small_mods_30bit(1),
                    small_mods_30bit(2), small_mods_30bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);

                int data_bound = 1 << 7;
                srand(static_cast<int>(time(NULL)));

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input[i] * input[i];
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    const double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input, parms.parms_id(), delta, plain);

                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    //evaluator.square_inplace(encrypted);
                    evaluator.multiply_inplace(encrypted, encrypted);
                    evaluator.relinearize_inplace(encrypted, rlk);

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
        }
        TEST_METHOD(CKKSEncryptMultiplyRelinRescaleDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            {
                //multiplying two random vectors 100 times
                int slot_size = 64;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ 
                    small_mods_30bit(0), small_mods_30bit(1), 
                    small_mods_30bit(2), small_mods_30bit(3) });
                int count_mod = static_cast<int>(parms.coeff_modulus().size());
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Ciphertext encryptedRes;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);

                for (int round = 0; round < 100; round++)
                {
                    int data_bound = 1 << 7;
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i];
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted2.parms_id() == parms.parms_id());

                    evaluator.multiply_inplace(encrypted1, encrypted2);
                    evaluator.relinearize_inplace(encrypted1, rlk);
                    evaluator.rescale_to_next_inplace(encrypted1);

                    //check correctness of modulo switching
                    Assert::IsTrue(encrypted1.parms_id() == next_parms_id);

                    decryptor.decrypt(encrypted1, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //multiplying two random vectors 100 times
                int slot_size = 16;
                parms.set_poly_modulus_degree(128);
                parms.set_coeff_modulus({
                    small_mods_30bit(0), small_mods_30bit(1),
                    small_mods_30bit(2), small_mods_30bit(3) });
                int count_mod = static_cast<int>(parms.coeff_modulus().size());
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Ciphertext encryptedRes;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);

                for (int round = 0; round < 100; round++)
                {
                    int data_bound = 1 << 7;
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i];
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted2.parms_id() == parms.parms_id());

                    evaluator.multiply_inplace(encrypted1, encrypted2);
                    evaluator.relinearize_inplace(encrypted1, rlk);
                    evaluator.rescale_to_next_inplace(encrypted1);

                    //check correctness of modulo switching
                    Assert::IsTrue(encrypted1.parms_id() == next_parms_id);

                    decryptor.decrypt(encrypted1, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
        }
        TEST_METHOD(CKKSEncryptSquareRelinRescaleDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            {
                //squaring two random vectors 100 times
                int slot_size = 64;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({
                    small_mods_50bit(0), small_mods_50bit(1),
                    small_mods_50bit(2) });
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                int data_bound = 1 << 8;

                for (int round = 0; round < 100; round++)
                {
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input[i] * input[i];
                    }

                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input, parms.parms_id(), delta, plain);

                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    evaluator.square_inplace(encrypted);
                    evaluator.relinearize_inplace(encrypted, rlk);
                    evaluator.rescale_to_next_inplace(encrypted);

                    //check correctness of modulo switching
                    Assert::IsTrue(encrypted.parms_id() == next_parms_id);

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //squaring two random vectors 100 times
                int slot_size = 16;
                parms.set_poly_modulus_degree(128);
                parms.set_coeff_modulus({
                    small_mods_50bit(0), small_mods_50bit(1),
                    small_mods_50bit(2) });
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                int data_bound = 1 << 8;

                for (int round = 0; round < 100; round++)
                {
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input[i] * input[i];
                    }

                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input, parms.parms_id(), delta, plain);

                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    evaluator.square_inplace(encrypted);
                    evaluator.relinearize_inplace(encrypted, rlk);
                    evaluator.rescale_to_next_inplace(encrypted);

                    //check correctness of modulo switching
                    Assert::IsTrue(encrypted.parms_id() == next_parms_id);

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
        }
        TEST_METHOD(CKKSEncryptModSwitchDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            {
                //modulo switching without rescaling for random vectors
                int slot_size = 64;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({
                    small_mods_60bit(0), small_mods_60bit(1),
                    small_mods_60bit(2), small_mods_60bit(3), small_mods_60bit(4) });
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                int data_bound = 1 << 30;
                srand(static_cast<int>(time(NULL)));

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = static_cast<double>(rand() % data_bound);
                    }

                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input, parms.parms_id(), delta, plain);

                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    evaluator.mod_switch_to_next_inplace(encrypted);

                    //check correctness of modulo switching
                    Assert::IsTrue(encrypted.parms_id() == next_parms_id);

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
                //modulo switching without rescaling for random vectors
                int slot_size = 32;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({
                    small_mods_40bit(0), small_mods_40bit(1), small_mods_40bit(2),
                    small_mods_40bit(3), small_mods_40bit(4) });
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                int data_bound = 1 << 30;
                srand(static_cast<int>(time(NULL)));

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = static_cast<double>(rand() % data_bound);
                    }

                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input, parms.parms_id(), delta, plain);

                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    evaluator.mod_switch_to_next_inplace(encrypted);

                    //check correctness of modulo switching
                    Assert::IsTrue(encrypted.parms_id() == next_parms_id);

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(input[i].real() - output[i].real());
                        if (tmp >= 0.5)
                            bool isTrap = true;
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //modulo switching without rescaling for random vectors
                int slot_size = 32;
                parms.set_poly_modulus_degree(128);
                parms.set_coeff_modulus({
                    small_mods_40bit(0), small_mods_40bit(1), small_mods_40bit(2),
                    small_mods_40bit(3), small_mods_40bit(4) });
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);

                int data_bound = 1 << 30;
                srand(static_cast<int>(time(NULL)));

                std::vector<std::complex<double>> input(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                Ciphertext encrypted;
                Plaintext plain;
                Plaintext plainRes;

                for (int round = 0; round < 100; round++)
                {
                    for (int i = 0; i < slot_size; i++)
                    {
                        input[i] = static_cast<double>(rand() % data_bound);
                    }

                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input, parms.parms_id(), delta, plain);

                    encryptor.encrypt(plain, encrypted);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

                    evaluator.mod_switch_to_next_inplace(encrypted);

                    //check correctness of modulo switching
                    Assert::IsTrue(encrypted.parms_id() == next_parms_id);

                    decryptor.decrypt(encrypted, plainRes);
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(input[i].real() - output[i].real());
                        if (tmp >= 0.5)
                            bool isTrap = true;
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
        }
        TEST_METHOD(CKKSEncryptMultiplyRelinRescaleModSwitchAddDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            {
                //multiplication and addition without rescaling for random vectors
                int slot_size = 64;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({
                    small_mods_50bit(0), small_mods_50bit(1),
                    small_mods_50bit(2) });
                int count_mod = static_cast<int>(parms.coeff_modulus().size());
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Ciphertext encrypted3;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plain3;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> input3(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);

                for (int round = 0; round < 100; round++)
                {
                    int data_bound = 1 << 8;
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i] + input3[i];
                    }

                    std::vector<std::complex<double>> output(slot_size);
                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);
                    encoder.encode(input3, parms.parms_id(), delta * delta, plain3);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);
                    encryptor.encrypt(plain3, encrypted3);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted2.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted3.parms_id() == parms.parms_id());

                    //enc1*enc2
                    evaluator.multiply_inplace(encrypted1, encrypted2);
                    evaluator.relinearize_inplace(encrypted1, rlk);
                    evaluator.rescale_to_next_inplace(encrypted1);

                    //check correctness of modulo switching with rescaling
                    Assert::IsTrue(encrypted1.parms_id() == next_parms_id);

                    //move enc3 to the level of enc1 * enc2
                    evaluator.rescale_to_inplace(encrypted3, next_parms_id);

                    //enc1*enc2 + enc3
                    evaluator.add_inplace(encrypted1, encrypted3);

                    //decryption
                    decryptor.decrypt(encrypted1, plainRes);
                    //decoding
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
            {
                //multiplication and addition without rescaling for random vectors
                int slot_size = 16;
                parms.set_poly_modulus_degree(128);
                parms.set_coeff_modulus({
                    small_mods_50bit(0), small_mods_50bit(1),
                    small_mods_50bit(2) });
                int count_mod = static_cast<int>(parms.coeff_modulus().size());
                auto context = SEALContext::Create(parms);
                auto next_parms_id = context->context_data()->
                    next_context_data()->parms().parms_id();
                KeyGenerator keygen(context);

                CKKSEncoder encoder(context);
                Encryptor encryptor(context, keygen.public_key());
                Decryptor decryptor(context, keygen.secret_key());
                Evaluator evaluator(context);
                RelinKeys rlk = keygen.relin_keys(4, 1);

                Ciphertext encrypted1;
                Ciphertext encrypted2;
                Ciphertext encrypted3;
                Plaintext plain1;
                Plaintext plain2;
                Plaintext plain3;
                Plaintext plainRes;

                std::vector<std::complex<double>> input1(slot_size, 0.0);
                std::vector<std::complex<double>> input2(slot_size, 0.0);
                std::vector<std::complex<double>> input3(slot_size, 0.0);
                std::vector<std::complex<double>> expected(slot_size, 0.0);
                std::vector<std::complex<double>> output(slot_size);

                for (int round = 0; round < 100; round++)
                {
                    int data_bound = 1 << 8;
                    srand(static_cast<int>(time(NULL)));
                    for (int i = 0; i < slot_size; i++)
                    {
                        input1[i] = static_cast<double>(rand() % data_bound);
                        input2[i] = static_cast<double>(rand() % data_bound);
                        expected[i] = input1[i] * input2[i] + input3[i];
                    }

                    double delta = static_cast<double>(1ULL << 40);
                    encoder.encode(input1, parms.parms_id(), delta, plain1);
                    encoder.encode(input2, parms.parms_id(), delta, plain2);
                    encoder.encode(input3, parms.parms_id(), delta * delta, plain3);

                    encryptor.encrypt(plain1, encrypted1);
                    encryptor.encrypt(plain2, encrypted2);
                    encryptor.encrypt(plain3, encrypted3);

                    //check correctness of encryption
                    Assert::IsTrue(encrypted1.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted2.parms_id() == parms.parms_id());
                    //check correctness of encryption
                    Assert::IsTrue(encrypted3.parms_id() == parms.parms_id());

                    //enc1*enc2
                    evaluator.multiply_inplace(encrypted1, encrypted2);
                    evaluator.relinearize_inplace(encrypted1, rlk);
                    evaluator.rescale_to_next_inplace(encrypted1);

                    //check correctness of modulo switching with rescaling
                    Assert::IsTrue(encrypted1.parms_id() == next_parms_id);

                    //move enc3 to the level of enc1 * enc2
                    evaluator.rescale_to_inplace(encrypted3, next_parms_id);

                    //enc1*enc2 + enc3
                    evaluator.add_inplace(encrypted1, encrypted3);

                    //decryption
                    decryptor.decrypt(encrypted1, plainRes);
                    //decoding
                    encoder.decode(plainRes, output);

                    for (int i = 0; i < slot_size; i++)
                    {
                        auto tmp = abs(expected[i].real() - output[i].real());
                        Assert::IsTrue(tmp < 0.5);
                    }
                }
            }
        }
        TEST_METHOD(CKKSEncryptRotateDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            {
                // maximal number of slots
                int slot_size = 4;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1), small_mods_40bit(2), small_mods_40bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);
                GaloisKeys glk = keygen.galois_keys(4);

                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());
                CKKSEncoder encoder(context);
                const double delta = static_cast<double>(1ULL << 30);

                Ciphertext encrypted;
                Plaintext plain;

                vector<std::complex<double>> input{
                    std::complex<double>(1, 1),
                    std::complex<double>(2, 2),
                    std::complex<double>(3, 3),
                    std::complex<double>(4, 4)
                };
                input.resize(slot_size);

                vector<std::complex<double>> output(slot_size, 0);

                encoder.encode(input, parms.parms_id(), delta, plain);
                int shift = 1;
                encryptor.encrypt(plain, encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(input[(i + shift) % slot_size].real(), round(output[i].real()));
                    Assert::AreEqual(input[(i + shift) % slot_size].imag(), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                shift = 2;
                encryptor.encrypt(plain, encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(input[(i + shift) % slot_size].real(), round(output[i].real()));
                    Assert::AreEqual(input[(i + shift) % slot_size].imag(), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                shift = 3;
                encryptor.encrypt(plain, encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(input[(i + shift) % slot_size].real(), round(output[i].real()));
                    Assert::AreEqual(input[(i + shift) % slot_size].imag(), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                encryptor.encrypt(plain, encrypted);
                evaluator.complex_conjugate_inplace(encrypted, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(input[i].real(), round(output[i].real()));
                    Assert::AreEqual(-input[i].imag(), round(output[i].imag()));
                }
            }
            {
                int slot_size = 32;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1), small_mods_40bit(2), small_mods_40bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);
                GaloisKeys glk = keygen.galois_keys(4);

                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());
                CKKSEncoder encoder(context);
                const double delta = static_cast<double>(1ULL << 30);

                Ciphertext encrypted;
                Plaintext plain;

                vector<std::complex<double>> input{
                    std::complex<double>(1, 1),
                    std::complex<double>(2, 2),
                    std::complex<double>(3, 3),
                    std::complex<double>(4, 4)
                };
                input.resize(slot_size);

                vector<std::complex<double>> output(slot_size, 0);

                encoder.encode(input, parms.parms_id(), delta, plain);
                int shift = 1;
                encryptor.encrypt(plain, encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (size_t i = 0; i < input.size(); i++)
                {
                    Assert::AreEqual(round(input[(i + shift) % slot_size].real()), round(output[i].real()));
                    Assert::AreEqual(round(input[(i + shift) % slot_size].imag()), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                shift = 2;
                encryptor.encrypt(plain, encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(round(input[(i + shift) % slot_size].real()), round(output[i].real()));
                    Assert::AreEqual(round(input[(i + shift) % slot_size].imag()), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                shift = 3;
                encryptor.encrypt(plain, encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(round(input[(i + shift) % slot_size].real()), round(output[i].real()));
                    Assert::AreEqual(round(input[(i + shift) % slot_size].imag()), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                encryptor.encrypt(plain, encrypted);
                evaluator.complex_conjugate_inplace(encrypted, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(round(input[i].real()), round(output[i].real()));
                    Assert::AreEqual(round(-input[i].imag()), round(output[i].imag()));
                }
            }
        }

        TEST_METHOD(CKKSEncryptRescaleRotateDecrypt)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            {
                // maximal number of slots
                int slot_size = 4;
                parms.set_poly_modulus_degree(2 * slot_size);
                parms.set_coeff_modulus({ small_mods_40bit(0), 
                    small_mods_40bit(1), small_mods_40bit(2), small_mods_40bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);
                GaloisKeys glk = keygen.galois_keys(4);

                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());
                CKKSEncoder encoder(context);
                const double delta = std::pow(2.0, 70);

                Ciphertext encrypted;
                Plaintext plain;

                vector<std::complex<double>> input{
                    std::complex<double>(1, 1),
                    std::complex<double>(2, 2),
                    std::complex<double>(3, 3),
                    std::complex<double>(4, 4)
                };
                input.resize(slot_size);

                vector<std::complex<double>> output(slot_size, 0);

                encoder.encode(input, parms.parms_id(), delta, plain);
                int shift = 1;
                encryptor.encrypt(plain, encrypted);
                evaluator.rescale_to_next_inplace(encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(input[(i + shift) % slot_size].real(), round(output[i].real()));
                    Assert::AreEqual(input[(i + shift) % slot_size].imag(), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                shift = 2;
                encryptor.encrypt(plain, encrypted);
                evaluator.rescale_to_next_inplace(encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(input[(i + shift) % slot_size].real(), round(output[i].real()));
                    Assert::AreEqual(input[(i + shift) % slot_size].imag(), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                shift = 3;
                encryptor.encrypt(plain, encrypted);
                evaluator.rescale_to_next_inplace(encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(input[(i + shift) % slot_size].real(), round(output[i].real()));
                    Assert::AreEqual(input[(i + shift) % slot_size].imag(), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                encryptor.encrypt(plain, encrypted);
                evaluator.rescale_to_next_inplace(encrypted);
                evaluator.complex_conjugate_inplace(encrypted, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(input[i].real(), round(output[i].real()));
                    Assert::AreEqual(-input[i].imag(), round(output[i].imag()));
                }
            }
            {
                int slot_size = 32;
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1), 
                    small_mods_40bit(2), small_mods_40bit(3) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);
                GaloisKeys glk = keygen.galois_keys(4);

                Encryptor encryptor(context, keygen.public_key());
                Evaluator evaluator(context);
                Decryptor decryptor(context, keygen.secret_key());
                CKKSEncoder encoder(context);
                const double delta = std::pow(2, 70);

                Ciphertext encrypted;
                Plaintext plain;

                vector<std::complex<double>> input{
                    std::complex<double>(1, 1),
                    std::complex<double>(2, 2),
                    std::complex<double>(3, 3),
                    std::complex<double>(4, 4)
                };
                input.resize(slot_size);

                vector<std::complex<double>> output(slot_size, 0);

                encoder.encode(input, parms.parms_id(), delta, plain);
                int shift = 1;
                encryptor.encrypt(plain, encrypted);
                evaluator.rescale_to_next_inplace(encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(round(input[(i + shift) % slot_size].real()), round(output[i].real()));
                    Assert::AreEqual(round(input[(i + shift) % slot_size].imag()), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                shift = 2;
                encryptor.encrypt(plain, encrypted);
                evaluator.rescale_to_next_inplace(encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(round(input[(i + shift) % slot_size].real()), round(output[i].real()));
                    Assert::AreEqual(round(input[(i + shift) % slot_size].imag()), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                shift = 3;
                encryptor.encrypt(plain, encrypted);
                evaluator.rescale_to_next_inplace(encrypted);
                evaluator.rotate_vector_inplace(encrypted, shift, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(round(input[(i + shift) % slot_size].real()), round(output[i].real()));
                    Assert::AreEqual(round(input[(i + shift) % slot_size].imag()), round(output[i].imag()));
                }

                encoder.encode(input, parms.parms_id(), delta, plain);
                encryptor.encrypt(plain, encrypted);
                evaluator.rescale_to_next_inplace(encrypted);
                evaluator.complex_conjugate_inplace(encrypted, glk);
                decryptor.decrypt(encrypted, plain);
                encoder.decode(plain, output);
                for (int i = 0; i < slot_size; i++)
                {
                    Assert::AreEqual(round(input[i].real()), round(output[i].real()));
                    Assert::AreEqual(round(-input[i].imag()), round(output[i].imag()));
                }
            }
        }

        TEST_METHOD(FVEncryptSquareDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Ciphertext encrypted;
            Plaintext plain;
            encryptor.encrypt(encoder.encode(1), encrypted);
            evaluator.square_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(1ULL, encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0), encrypted);
            evaluator.square_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(0ULL, encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-5), encrypted);
            evaluator.square_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(25ULL, encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-1), encrypted);
            evaluator.square_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(1ULL, encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(123), encrypted);
            evaluator.square_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(15129ULL, encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0x10000), encrypted);
            evaluator.square_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(0x100000000ULL, encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(123), encrypted);
            evaluator.square_inplace(encrypted);
            evaluator.square_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(228886641ULL, encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptMultiplyManyDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());
            RelinKeys rlk = keygen.relin_keys(4);

            Ciphertext encrypted1, encrypted2, encrypted3, encrypted4, product;
            Plaintext plain;
            encryptor.encrypt(encoder.encode(5), encrypted1);
            encryptor.encrypt(encoder.encode(6), encrypted2);
            encryptor.encrypt(encoder.encode(7), encrypted3);
            vector<Ciphertext> encrypteds{ encrypted1, encrypted2, encrypted3 };
                evaluator.multiply_many(encrypteds, rlk, product);
                decryptor.decrypt(product, plain);
                Assert::AreEqual(static_cast<uint64_t>(210), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted1.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted2.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted3.parms_id() == product.parms_id());
                Assert::IsTrue(product.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(-9), encrypted1);
                encryptor.encrypt(encoder.encode(-17), encrypted2);
                encrypteds = { encrypted1, encrypted2 };
                evaluator.multiply_many(encrypteds, rlk, product);
                decryptor.decrypt(product, plain);
                Assert::AreEqual(static_cast<uint64_t>(153), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted1.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted2.parms_id() == product.parms_id());
                Assert::IsTrue(product.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(2), encrypted1);
                encryptor.encrypt(encoder.encode(-31), encrypted2);
                encryptor.encrypt(encoder.encode(7), encrypted3);
                encrypteds = { encrypted1, encrypted2, encrypted3 };
                evaluator.multiply_many(encrypteds, rlk, product);
                decryptor.decrypt(product, plain);
                Assert::IsTrue(static_cast<int64_t>(-434) == encoder.decode_int64(plain));
                Assert::IsTrue(encrypted1.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted2.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted3.parms_id() == product.parms_id());
                Assert::IsTrue(product.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(1), encrypted1);
                encryptor.encrypt(encoder.encode(-1), encrypted2);
                encryptor.encrypt(encoder.encode(1), encrypted3);
                encryptor.encrypt(encoder.encode(-1), encrypted4);
                encrypteds = { encrypted1, encrypted2, encrypted3, encrypted4 };
                evaluator.multiply_many(encrypteds, rlk, product);
                decryptor.decrypt(product, plain);
                Assert::AreEqual(static_cast<uint64_t>(1), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted1.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted2.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted3.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted4.parms_id() == product.parms_id());
                Assert::IsTrue(product.parms_id() == parms.parms_id());

                encryptor.encrypt(encoder.encode(98765), encrypted1);
                encryptor.encrypt(encoder.encode(0), encrypted2);
                encryptor.encrypt(encoder.encode(12345), encrypted3);
                encryptor.encrypt(encoder.encode(34567), encrypted4);
                encrypteds = { encrypted1, encrypted2, encrypted3, encrypted4 };
                evaluator.multiply_many(encrypteds, rlk, product);
                decryptor.decrypt(product, plain);
                Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
                Assert::IsTrue(encrypted1.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted2.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted3.parms_id() == product.parms_id());
                Assert::IsTrue(encrypted4.parms_id() == product.parms_id());
                Assert::IsTrue(product.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptExponentiateDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());
            RelinKeys rlk = keygen.relin_keys(4);

            Ciphertext encrypted;
            Plaintext plain;
            encryptor.encrypt(encoder.encode(5), encrypted);
            evaluator.exponentiate_inplace(encrypted, 1, rlk);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<uint64_t>(5), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(7), encrypted);
            evaluator.exponentiate_inplace(encrypted, 2, rlk);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<uint64_t>(49), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-7), encrypted);
            evaluator.exponentiate_inplace(encrypted, 3, rlk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(static_cast<int64_t>(-343) == encoder.decode_int64(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(0x100), encrypted);
            evaluator.exponentiate_inplace(encrypted, 4, rlk);
            decryptor.decrypt(encrypted, plain);
            Assert::AreEqual(static_cast<uint64_t>(0x100000000), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted.parms_id() == encrypted.parms_id());
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptAddManyDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            BalancedEncoder encoder(plain_modulus);
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Ciphertext encrypted1, encrypted2, encrypted3, encrypted4, sum;
            Plaintext plain;
            encryptor.encrypt(encoder.encode(5), encrypted1);
            encryptor.encrypt(encoder.encode(6), encrypted2);
            encryptor.encrypt(encoder.encode(7), encrypted3);
            vector<Ciphertext> encrypteds = { encrypted1, encrypted2, encrypted3 };
            evaluator.add_many(encrypteds, sum);
            decryptor.decrypt(sum, plain);
            Assert::AreEqual(static_cast<uint64_t>(18), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted2.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted3.parms_id() == sum.parms_id());
            Assert::IsTrue(sum.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(-9), encrypted1);
            encryptor.encrypt(encoder.encode(-17), encrypted2);
            encrypteds = { encrypted1, encrypted2, };
            evaluator.add_many(encrypteds, sum);
            decryptor.decrypt(sum, plain);
            Assert::IsTrue(static_cast<int64_t>(-26) == encoder.decode_int64(plain));
            Assert::IsTrue(encrypted1.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted2.parms_id() == sum.parms_id());
            Assert::IsTrue(sum.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(2), encrypted1);
            encryptor.encrypt(encoder.encode(-31), encrypted2);
            encryptor.encrypt(encoder.encode(7), encrypted3);
            encrypteds = { encrypted1, encrypted2, encrypted3 };
            evaluator.add_many(encrypteds, sum);
            decryptor.decrypt(sum, plain);
            Assert::IsTrue(static_cast<int64_t>(-22) == encoder.decode_int64(plain));
            Assert::IsTrue(encrypted1.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted2.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted3.parms_id() == sum.parms_id());
            Assert::IsTrue(sum.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(1), encrypted1);
            encryptor.encrypt(encoder.encode(-1), encrypted2);
            encryptor.encrypt(encoder.encode(1), encrypted3);
            encryptor.encrypt(encoder.encode(-1), encrypted4);
            encrypteds = { encrypted1, encrypted2, encrypted3, encrypted4 };
            evaluator.add_many(encrypteds, sum);
            decryptor.decrypt(sum, plain);
            Assert::AreEqual(static_cast<uint64_t>(0), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted2.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted3.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted4.parms_id() == sum.parms_id());
            Assert::IsTrue(sum.parms_id() == parms.parms_id());

            encryptor.encrypt(encoder.encode(98765), encrypted1);
            encryptor.encrypt(encoder.encode(0), encrypted2);
            encryptor.encrypt(encoder.encode(12345), encrypted3);
            encryptor.encrypt(encoder.encode(34567), encrypted4);
            encrypteds = { encrypted1, encrypted2, encrypted3, encrypted4 };
            evaluator.add_many(encrypteds, sum);
            decryptor.decrypt(sum, plain);
            Assert::AreEqual(static_cast<uint64_t>(145677), encoder.decode_uint64(plain));
            Assert::IsTrue(encrypted1.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted2.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted3.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted4.parms_id() == sum.parms_id());
            Assert::IsTrue(sum.parms_id() == parms.parms_id());

            BalancedFractionalEncoder frac_encoder(plain_modulus, 128, 10, 15);
            encryptor.encrypt(frac_encoder.encode(3.1415), encrypted1);
            encryptor.encrypt(frac_encoder.encode(12.345), encrypted2);
            encryptor.encrypt(frac_encoder.encode(98.765), encrypted3);
            encryptor.encrypt(frac_encoder.encode(1.1111), encrypted4);
            encrypteds = { encrypted1, encrypted2, encrypted3, encrypted4 };
            evaluator.add_many(encrypteds, sum);
            decryptor.decrypt(sum, plain);
            Assert::IsTrue(abs(frac_encoder.decode(plain) - 115.3626) < 0.000001);
            Assert::IsTrue(encrypted1.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted2.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted3.parms_id() == sum.parms_id());
            Assert::IsTrue(encrypted4.parms_id() == sum.parms_id());
            Assert::IsTrue(sum.parms_id() == parms.parms_id());
        }

        TEST_METHOD(TransformPlainToNTT)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            Evaluator evaluator(context);
            Plaintext plain = "0";
            Assert::IsFalse(plain.is_ntt_form());
            evaluator.transform_to_ntt_inplace(plain, parms.parms_id());
            Assert::IsTrue(plain.is_zero());
            Assert::IsTrue(plain.is_ntt_form());
            Assert::IsTrue(plain.parms_id() == parms.parms_id());

            plain.release();
            plain = "0";
            Assert::IsFalse(plain.is_ntt_form());
            auto next_parms_id = context->context_data()->
                next_context_data()->parms().parms_id();
            evaluator.transform_to_ntt_inplace(plain, next_parms_id);
            Assert::IsTrue(plain.is_zero());
            Assert::IsTrue(plain.is_ntt_form());
            Assert::IsTrue(plain.parms_id() == next_parms_id);

            plain.release();
            plain = "1";
            Assert::IsFalse(plain.is_ntt_form());
            evaluator.transform_to_ntt_inplace(plain, parms.parms_id());
            for (int i = 0; i < 256; i++)
            {
                Assert::IsTrue(plain[i] == 1);
            }
            Assert::IsTrue(plain.is_ntt_form());
            Assert::IsTrue(plain.parms_id() == parms.parms_id());

            plain.release();
            plain = "1";
            Assert::IsFalse(plain.is_ntt_form());
            evaluator.transform_to_ntt_inplace(plain, next_parms_id);
            for (int i = 0; i < 128; i++)
            {
                Assert::IsTrue(plain[i] == 1);
            }
            Assert::IsTrue(plain.is_ntt_form());
            Assert::IsTrue(plain.parms_id() == next_parms_id);

            plain.release();
            plain = "2";
            Assert::IsFalse(plain.is_ntt_form());
            evaluator.transform_to_ntt_inplace(plain, parms.parms_id());
            for (int i = 0; i < 256; i++)
            {
                Assert::IsTrue(plain[i] == 2);
            }
            Assert::IsTrue(plain.is_ntt_form());
            Assert::IsTrue(plain.parms_id() == parms.parms_id());

            plain.release();
            plain = "2";
            evaluator.transform_to_ntt_inplace(plain, next_parms_id);
            for (int i = 0; i < 128; i++)
            {
                Assert::IsTrue(plain[i] == 2);
            }
            Assert::IsTrue(plain.is_ntt_form());
            Assert::IsTrue(plain.parms_id() == next_parms_id);
        }

        TEST_METHOD(TransformEncryptedToFromNTT)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Plaintext plain;
            Ciphertext encrypted;
            plain = "0";
            encryptor.encrypt(plain, encrypted);
            evaluator.transform_to_ntt_inplace(encrypted);
            evaluator.transform_from_ntt_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(plain.to_string() == "0");
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            plain = "1";
            encryptor.encrypt(plain, encrypted);
            evaluator.transform_to_ntt_inplace(encrypted);
            evaluator.transform_from_ntt_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(plain.to_string() == "1");
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            plain = "Fx^10 + Ex^9 + Dx^8 + Cx^7 + Bx^6 + Ax^5 + 1x^4 + 2x^3 + 3x^2 + 4x^1 + 5";
            encryptor.encrypt(plain, encrypted);
            evaluator.transform_to_ntt_inplace(encrypted);
            evaluator.transform_from_ntt_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(plain.to_string() == "Fx^10 + Ex^9 + Dx^8 + Cx^7 + Bx^6 + Ax^5 + 1x^4 + 2x^3 + 3x^2 + 4x^1 + 5");
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptMultiplyPlainNTTDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(1 << 6);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Plaintext plain;
            Plaintext plain_multiplier;
            Ciphertext encrypted;

            plain = 0;
            encryptor.encrypt(plain, encrypted);
            evaluator.transform_to_ntt_inplace(encrypted);
            plain_multiplier = 1;
            evaluator.transform_to_ntt_inplace(plain_multiplier, parms.parms_id());
            evaluator.multiply_plain_inplace(encrypted, plain_multiplier);
            evaluator.transform_from_ntt_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(plain.to_string() == "0");
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            plain = 2;
            encryptor.encrypt(plain, encrypted);
            evaluator.transform_to_ntt_inplace(encrypted);
            plain_multiplier.release();
            plain_multiplier = 3;
            evaluator.transform_to_ntt_inplace(plain_multiplier, parms.parms_id());
            evaluator.multiply_plain_inplace(encrypted, plain_multiplier);
            evaluator.transform_from_ntt_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(plain.to_string() == "6");
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            plain = 1;
            encryptor.encrypt(plain, encrypted);
            evaluator.transform_to_ntt_inplace(encrypted);
            plain_multiplier.release();
            plain_multiplier = "Fx^10 + Ex^9 + Dx^8 + Cx^7 + Bx^6 + Ax^5 + 1x^4 + 2x^3 + 3x^2 + 4x^1 + 5";
            evaluator.transform_to_ntt_inplace(plain_multiplier, parms.parms_id());
            evaluator.multiply_plain_inplace(encrypted, plain_multiplier);
            evaluator.transform_from_ntt_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(plain.to_string() == "Fx^10 + Ex^9 + Dx^8 + Cx^7 + Bx^6 + Ax^5 + 1x^4 + 2x^3 + 3x^2 + 4x^1 + 5");
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());

            plain = "1x^20";
            encryptor.encrypt(plain, encrypted);
            evaluator.transform_to_ntt_inplace(encrypted);
            plain_multiplier.release();
            plain_multiplier = "Fx^10 + Ex^9 + Dx^8 + Cx^7 + Bx^6 + Ax^5 + 1x^4 + 2x^3 + 3x^2 + 4x^1 + 5";
            evaluator.transform_to_ntt_inplace(plain_multiplier, parms.parms_id());
            evaluator.multiply_plain_inplace(encrypted, plain_multiplier);
            evaluator.transform_from_ntt_inplace(encrypted);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(plain.to_string() == "Fx^30 + Ex^29 + Dx^28 + Cx^27 + Bx^26 + Ax^25 + 1x^24 + 2x^23 + 3x^22 + 4x^21 + 5x^20");
            Assert::IsTrue(encrypted.parms_id() == parms.parms_id());
        }

        TEST_METHOD(FVEncryptApplyGaloisDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(257);
            parms.set_poly_modulus_degree(8);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);
            GaloisKeys glk = keygen.galois_keys(24, { 1, 3, 5, 15 });

            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());

            Plaintext plain("1");
            Ciphertext encrypted;
            encryptor.encrypt(plain, encrypted);
            evaluator.apply_galois_inplace(encrypted, 1, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 3, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 5, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 15, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1" == plain.to_string());

            plain = "1x^1";
            encryptor.encrypt(plain, encrypted);
            evaluator.apply_galois_inplace(encrypted, 1, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1x^1" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 3, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1x^3" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 5, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("100x^7" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 15, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1x^1" == plain.to_string());

            plain = "1x^2";
            encryptor.encrypt(plain, encrypted);
            evaluator.apply_galois_inplace(encrypted, 1, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1x^2" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 3, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1x^6" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 5, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("100x^6" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 15, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1x^2" == plain.to_string());

            plain = "1x^3 + 2x^2 + 1x^1 + 1";
            encryptor.encrypt(plain, encrypted);
            evaluator.apply_galois_inplace(encrypted, 1, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1x^3 + 2x^2 + 1x^1 + 1" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 3, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("2x^6 + 1x^3 + 100x^1 + 1" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 5, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("100x^7 + FFx^6 + 100x^5 + 1" == plain.to_string());
            evaluator.apply_galois_inplace(encrypted, 15, glk);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue("1x^3 + 2x^2 + 1x^1 + 1" == plain.to_string());
        }

        TEST_METHOD(FVEncryptRotateMatrixDecrypt)
        {
            EncryptionParameters parms(scheme_type::BFV);
            SmallModulus plain_modulus(257);
            parms.set_poly_modulus_degree(8);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_40bit(0), small_mods_40bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);
            GaloisKeys glk = keygen.galois_keys(24);

            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());
            BatchEncoder batch_encoder(context);

            Plaintext plain;
            vector<uint64_t> plain_vec{
                1, 2, 3, 4,
                5, 6, 7, 8
            };
            batch_encoder.encode(plain_vec, plain);
            Ciphertext encrypted;
            encryptor.encrypt(plain, encrypted);

            evaluator.rotate_columns_inplace(encrypted, glk);
            decryptor.decrypt(encrypted, plain);
            batch_encoder.decode(plain, plain_vec);
            Assert::IsTrue(plain_vec == vector<uint64_t>{
                5, 6, 7, 8,
                1, 2, 3, 4
            });

            evaluator.rotate_rows_inplace(encrypted, -1, glk);
            decryptor.decrypt(encrypted, plain);
            batch_encoder.decode(plain, plain_vec);
            Assert::IsTrue(plain_vec == vector<uint64_t>{
                8, 5, 6, 7,
                4, 1, 2, 3
            });

            evaluator.rotate_rows_inplace(encrypted, 2, glk);
            decryptor.decrypt(encrypted, plain);
            batch_encoder.decode(plain, plain_vec);
            Assert::IsTrue(plain_vec == vector<uint64_t>{
                6, 7, 8, 5,
                2, 3, 4, 1
            });

            evaluator.rotate_columns_inplace(encrypted, glk);
            decryptor.decrypt(encrypted, plain);
            batch_encoder.decode(plain, plain_vec);
            Assert::IsTrue(plain_vec == vector<uint64_t>{
                2, 3, 4, 1,
                6, 7, 8, 5
            });

            evaluator.rotate_rows_inplace(encrypted, 0, glk);
            decryptor.decrypt(encrypted, plain);
            batch_encoder.decode(plain, plain_vec);
            Assert::IsTrue(plain_vec == vector<uint64_t>{
                2, 3, 4, 1,
                6, 7, 8, 5
            });
        }
        TEST_METHOD(FVEncryptModSwitchToNextDecrypt)
        {
            // the common parameters: the plaintext and the polynomial moduli
            SmallModulus plain_modulus(1 << 6);

            // the parameters and the context of the higher level
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_30bit(0), small_mods_30bit(1), small_mods_30bit(2) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);
            SecretKey secret_key = keygen.secret_key();
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());
            auto parms_id = parms.parms_id();

            Ciphertext encrypted(context);
            Ciphertext encryptedRes;
            Plaintext plain;

            plain = 0;
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_next(encrypted, encryptedRes);
            decryptor.decrypt(encryptedRes, plain);
            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            Assert::IsTrue(encryptedRes.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "0");

            evaluator.mod_switch_to_next_inplace(encryptedRes);
            decryptor.decrypt(encryptedRes, plain);
            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            Assert::IsTrue(encryptedRes.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "0");

            parms_id = parms.parms_id();
            plain = 1;
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_next(encrypted, encryptedRes);
            decryptor.decrypt(encryptedRes, plain);
            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            Assert::IsTrue(encryptedRes.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1");

            evaluator.mod_switch_to_next_inplace(encryptedRes);
            decryptor.decrypt(encryptedRes, plain);
            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            Assert::IsTrue(encryptedRes.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1");

            parms_id = parms.parms_id();
            plain = "1x^127";
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_next(encrypted, encryptedRes);
            decryptor.decrypt(encryptedRes, plain);
            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            Assert::IsTrue(encryptedRes.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1x^127");

            evaluator.mod_switch_to_next_inplace(encryptedRes);
            decryptor.decrypt(encryptedRes, plain);
            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            Assert::IsTrue(encryptedRes.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1x^127");

            parms_id = parms.parms_id();
            plain = "5x^64 + Ax^5";
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_next(encrypted, encryptedRes);
            decryptor.decrypt(encryptedRes, plain);
            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            Assert::IsTrue(encryptedRes.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "5x^64 + Ax^5");

            evaluator.mod_switch_to_next_inplace(encryptedRes);
            decryptor.decrypt(encryptedRes, plain);
            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            Assert::IsTrue(encryptedRes.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "5x^64 + Ax^5");
        }

        TEST_METHOD(FVEncryptModSwitchToDecrypt)
        {
            // the common parameters: the plaintext and the polynomial moduli
            SmallModulus plain_modulus(1 << 6);

            // the parameters and the context of the higher level
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_poly_modulus_degree(128);
            parms.set_plain_modulus(plain_modulus);
            parms.set_coeff_modulus({ small_mods_30bit(0), small_mods_30bit(1), small_mods_30bit(2) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);
            SecretKey secret_key = keygen.secret_key();
            Encryptor encryptor(context, keygen.public_key());
            Evaluator evaluator(context);
            Decryptor decryptor(context, keygen.secret_key());
            auto parms_id = parms.parms_id();

            Ciphertext encrypted(context);
            Plaintext plain;

            plain = 0;
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "0");

            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "0");

            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "0");

            parms_id = parms.parms_id();
            encryptor.encrypt(plain, encrypted);
            parms_id = context->context_data(parms_id)->
                next_context_data()->
                next_context_data()->parms().parms_id();
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "0");

            parms_id = parms.parms_id();
            plain = 1;
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1");

            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1");

            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1");

            parms_id = parms.parms_id();
            encryptor.encrypt(plain, encrypted);
            parms_id = context->context_data(parms_id)->
                next_context_data()->
                next_context_data()->parms().parms_id();
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1");

            parms_id = parms.parms_id();
            plain = "1x^127";
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1x^127");

            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1x^127");

            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1x^127");

            parms_id = parms.parms_id();
            encryptor.encrypt(plain, encrypted);
            parms_id = context->context_data(parms_id)->
                next_context_data()->
                next_context_data()->parms().parms_id();
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "1x^127");

            parms_id = parms.parms_id();
            plain = "5x^64 + Ax^5";
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "5x^64 + Ax^5");

            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "5x^64 + Ax^5");

            parms_id = context->context_data(parms_id)->
                next_context_data()->parms().parms_id();
            encryptor.encrypt(plain, encrypted);
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "5x^64 + Ax^5");

            parms_id = parms.parms_id();
            encryptor.encrypt(plain, encrypted);
            parms_id = context->context_data(parms_id)->
                next_context_data()->
                next_context_data()->parms().parms_id();
            evaluator.mod_switch_to_inplace(encrypted, parms_id);
            decryptor.decrypt(encrypted, plain);
            Assert::IsTrue(encrypted.parms_id() == parms_id);
            Assert::IsTrue(plain.to_string() == "5x^64 + Ax^5");
        }
    };
}
