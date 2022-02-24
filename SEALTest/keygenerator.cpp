#include "CppUnitTest.h"
#include "seal/context.h"
#include "seal/keygenerator.h"
#include "seal/util/polycore.h"
#include "seal/defaultparams.h"
#include "seal/encryptor.h"
#include "seal/decryptor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace seal::util;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(KeyGeneratorTest)
    {
    public:
        TEST_METHOD(FVKeyGeneration)
        {
            EncryptionParameters parms(scheme_type::BFV);
            {
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(64);
                parms.set_plain_modulus(1 << 6);
                parms.set_coeff_modulus({ small_mods_60bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                RelinKeys evk = keygen.relin_keys(60);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(2ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                evk = keygen.relin_keys(30, 1);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(4ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                evk = keygen.relin_keys(2, 2);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(60ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                GaloisKeys galks = keygen.galois_keys(60);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(2ULL, galks.key(3)[0].size());
                Assert::AreEqual(10, galks.size());

                galks = keygen.galois_keys(30);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(4ULL, galks.key(3)[0].size());
                Assert::AreEqual(10, galks.size());

                galks = keygen.galois_keys(2);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(60ULL, galks.key(3)[0].size());
                Assert::AreEqual(10, galks.size());

                galks = keygen.galois_keys(60, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(127));
                Assert::AreEqual(2ULL, galks.key(1)[0].size());
                Assert::AreEqual(2ULL, galks.key(3)[0].size());
                Assert::AreEqual(2ULL, galks.key(5)[0].size());
                Assert::AreEqual(2ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(30, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(127));
                Assert::AreEqual(4ULL, galks.key(1)[0].size());
                Assert::AreEqual(4ULL, galks.key(3)[0].size());
                Assert::AreEqual(4ULL, galks.key(5)[0].size());
                Assert::AreEqual(4ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(2, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(127));
                Assert::AreEqual(60ULL, galks.key(1)[0].size());
                Assert::AreEqual(60ULL, galks.key(3)[0].size());
                Assert::AreEqual(60ULL, galks.key(5)[0].size());
                Assert::AreEqual(60ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(30, { 1 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsFalse(galks.has_key(3));
                Assert::IsFalse(galks.has_key(127));
                Assert::AreEqual(4ULL, galks.key(1)[0].size());
                Assert::AreEqual(1, galks.size());

                galks = keygen.galois_keys(30, { 127 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsFalse(galks.has_key(1));
                Assert::IsTrue(galks.has_key(127));
                Assert::AreEqual(4ULL, galks.key(127)[0].size());
                Assert::AreEqual(1, galks.size());
            }
            {
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(256);
                parms.set_plain_modulus(1 << 6);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_30bit(0), small_mods_30bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                RelinKeys evk = keygen.relin_keys(60, 2);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(2ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                evk = keygen.relin_keys(30, 2);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(4ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                evk = keygen.relin_keys(4, 1);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(30ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                GaloisKeys galks = keygen.galois_keys(60);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(2ULL, galks.key(3)[0].size());
                Assert::AreEqual(14, galks.size());

                galks = keygen.galois_keys(30);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(4ULL, galks.key(3)[0].size());
                Assert::AreEqual(14, galks.size());

                galks = keygen.galois_keys(2);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(60ULL, galks.key(3)[0].size());
                Assert::AreEqual(14, galks.size());

                galks = keygen.galois_keys(60, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(511));
                Assert::AreEqual(2ULL, galks.key(1)[0].size());
                Assert::AreEqual(2ULL, galks.key(3)[0].size());
                Assert::AreEqual(2ULL, galks.key(5)[0].size());
                Assert::AreEqual(2ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(30, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(511));
                Assert::AreEqual(4ULL, galks.key(1)[0].size());
                Assert::AreEqual(4ULL, galks.key(3)[0].size());
                Assert::AreEqual(4ULL, galks.key(5)[0].size());
                Assert::AreEqual(4ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(2, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(511));
                Assert::AreEqual(60ULL, galks.key(1)[0].size());
                Assert::AreEqual(60ULL, galks.key(3)[0].size());
                Assert::AreEqual(60ULL, galks.key(5)[0].size());
                Assert::AreEqual(60ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(30, { 1 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsFalse(galks.has_key(3));
                Assert::IsFalse(galks.has_key(511));
                Assert::AreEqual(4ULL, galks.key(1)[0].size());
                Assert::AreEqual(1, galks.size());

                galks = keygen.galois_keys(30, { 511 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsFalse(galks.has_key(1));
                Assert::IsTrue(galks.has_key(511));
                Assert::AreEqual(4ULL, galks.key(511)[0].size());
                Assert::AreEqual(1, galks.size());
            }
        }

        TEST_METHOD(CKKSKeyGeneration)
        {
            EncryptionParameters parms(scheme_type::CKKS);
            {
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(64);
                parms.set_coeff_modulus({ small_mods_60bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                RelinKeys evk = keygen.relin_keys(60);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(2ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                evk = keygen.relin_keys(30, 1);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(4ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                evk = keygen.relin_keys(2, 2);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(60ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                GaloisKeys galks = keygen.galois_keys(60);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(2ULL, galks.key(3)[0].size());
                Assert::AreEqual(10, galks.size());

                galks = keygen.galois_keys(30);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(4ULL, galks.key(3)[0].size());
                Assert::AreEqual(10, galks.size());

                galks = keygen.galois_keys(2);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(60ULL, galks.key(3)[0].size());
                Assert::AreEqual(10, galks.size());

                galks = keygen.galois_keys(60, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(127));
                Assert::AreEqual(2ULL, galks.key(1)[0].size());
                Assert::AreEqual(2ULL, galks.key(3)[0].size());
                Assert::AreEqual(2ULL, galks.key(5)[0].size());
                Assert::AreEqual(2ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(30, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(127));
                Assert::AreEqual(4ULL, galks.key(1)[0].size());
                Assert::AreEqual(4ULL, galks.key(3)[0].size());
                Assert::AreEqual(4ULL, galks.key(5)[0].size());
                Assert::AreEqual(4ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(2, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(127));
                Assert::AreEqual(60ULL, galks.key(1)[0].size());
                Assert::AreEqual(60ULL, galks.key(3)[0].size());
                Assert::AreEqual(60ULL, galks.key(5)[0].size());
                Assert::AreEqual(60ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(30, { 1 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsFalse(galks.has_key(3));
                Assert::IsFalse(galks.has_key(127));
                Assert::AreEqual(4ULL, galks.key(1)[0].size());
                Assert::AreEqual(1, galks.size());

                galks = keygen.galois_keys(30, { 127 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsFalse(galks.has_key(1));
                Assert::IsTrue(galks.has_key(127));
                Assert::AreEqual(4ULL, galks.key(127)[0].size());
                Assert::AreEqual(1, galks.size());
            }
            {
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(256);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_30bit(0), small_mods_30bit(1) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                RelinKeys evk = keygen.relin_keys(60, 2);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(2ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                evk = keygen.relin_keys(30, 2);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(4ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                evk = keygen.relin_keys(4, 1);
                Assert::IsTrue(evk.parms_id() == parms.parms_id());
                Assert::AreEqual(30ULL, evk.key(2)[0].size());
                for (int j = 0; j < evk.size(); j++)
                {
                    for (int i = 0; i < evk.key(j + 2).size(); i++)
                    {
                        for (int k = 0; k < evk.key(j + 2)[i].size(); k++)
                        {
                            Assert::IsFalse(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                        }
                    }
                }

                GaloisKeys galks = keygen.galois_keys(60);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(2ULL, galks.key(3)[0].size());
                Assert::AreEqual(14, galks.size());

                galks = keygen.galois_keys(30);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(4ULL, galks.key(3)[0].size());
                Assert::AreEqual(14, galks.size());

                galks = keygen.galois_keys(2);
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::AreEqual(60ULL, galks.key(3)[0].size());
                Assert::AreEqual(14, galks.size());

                galks = keygen.galois_keys(60, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(511));
                Assert::AreEqual(2ULL, galks.key(1)[0].size());
                Assert::AreEqual(2ULL, galks.key(3)[0].size());
                Assert::AreEqual(2ULL, galks.key(5)[0].size());
                Assert::AreEqual(2ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(30, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(511));
                Assert::AreEqual(4ULL, galks.key(1)[0].size());
                Assert::AreEqual(4ULL, galks.key(3)[0].size());
                Assert::AreEqual(4ULL, galks.key(5)[0].size());
                Assert::AreEqual(4ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(2, { 1, 3, 5, 7 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsTrue(galks.has_key(3));
                Assert::IsTrue(galks.has_key(5));
                Assert::IsTrue(galks.has_key(7));
                Assert::IsFalse(galks.has_key(9));
                Assert::IsFalse(galks.has_key(511));
                Assert::AreEqual(60ULL, galks.key(1)[0].size());
                Assert::AreEqual(60ULL, galks.key(3)[0].size());
                Assert::AreEqual(60ULL, galks.key(5)[0].size());
                Assert::AreEqual(60ULL, galks.key(7)[0].size());
                Assert::AreEqual(4, galks.size());

                galks = keygen.galois_keys(30, { 1 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsTrue(galks.has_key(1));
                Assert::IsFalse(galks.has_key(3));
                Assert::IsFalse(galks.has_key(511));
                Assert::AreEqual(4ULL, galks.key(1)[0].size());
                Assert::AreEqual(1, galks.size());

                galks = keygen.galois_keys(30, { 511 });
                Assert::IsTrue(galks.parms_id() == parms.parms_id());
                Assert::IsFalse(galks.has_key(1));
                Assert::IsTrue(galks.has_key(511));
                Assert::AreEqual(4ULL, galks.key(511)[0].size());
                Assert::AreEqual(1, galks.size());
            }
        }

        TEST_METHOD(FVSecretKeyGeneration)
        {
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_noise_standard_deviation(3.19);
            parms.set_poly_modulus_degree(64);
            parms.set_plain_modulus(1 << 6);
            parms.set_coeff_modulus({ small_mods_60bit(0) });
            auto context = SEALContext::Create(parms);
            {
                KeyGenerator keygen(context);
                auto pk = keygen.public_key();
                auto sk = keygen.secret_key();

                KeyGenerator keygen2(context, sk);
                auto sk2 = keygen.secret_key();
                auto pk2 = keygen2.public_key();

                Assert::AreEqual(sk.data().coeff_count(), sk2.data().coeff_count());
                for (int i = 0; i < sk.data().coeff_count(); i++)
                {
                    Assert::AreEqual(sk.data()[i], sk2.data()[i]);
                }

                Assert::AreEqual(pk.data().uint64_count(), pk2.data().uint64_count());
                for (int i = 0; i < pk.data().uint64_count(); i++)
                {
                    Assert::AreNotEqual(pk.data()[i], pk2.data()[i]);
                }

                Encryptor encryptor(context, pk2);
                Decryptor decryptor(context, sk);
                Ciphertext ctxt;
                Plaintext pt1("1x^63 + 2x^33 + 3x^23 + 4x^13 + 5x^1 + 6");
                Plaintext pt2;
                encryptor.encrypt(pt1, ctxt);
                decryptor.decrypt(ctxt, pt2);
                Assert::IsTrue(pt1 == pt2);
            }
            {
                KeyGenerator keygen(context);
                auto pk = keygen.public_key();
                auto sk = keygen.secret_key();

                KeyGenerator keygen2(context, sk, pk);
                auto sk2 = keygen.secret_key();
                auto pk2 = keygen2.public_key();

                Assert::AreEqual(sk.data().coeff_count(), sk2.data().coeff_count());
                for (int i = 0; i < sk.data().coeff_count(); i++)
                {
                    Assert::AreEqual(sk.data()[i], sk2.data()[i]);
                }

                Assert::AreEqual(pk.data().uint64_count(), pk2.data().uint64_count());
                for (int i = 0; i < pk.data().uint64_count(); i++)
                {
                    Assert::AreEqual(pk.data()[i], pk2.data()[i]);
                }
            }
        }
    };
}
