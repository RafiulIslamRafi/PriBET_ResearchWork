#include "CppUnitTest.h"
#include "seal/galoiskeys.h"
#include "seal/context.h"
#include "seal/keygenerator.h"
#include "seal/util/uintcore.h"
#include "seal/defaultparams.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace seal::util;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(GaloisKeysTest)
    {
    public:
        TEST_METHOD(GaloisKeysSaveLoad)
        {
            stringstream stream;
            {
                EncryptionParameters parms(scheme_type::BFV);
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(64);
                parms.set_plain_modulus(65537);
                parms.set_coeff_modulus({ small_mods_60bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                GaloisKeys keys;
                GaloisKeys test_keys;
                Assert::AreEqual(keys.decomposition_bit_count(), 0);
                keys.save(stream);
                test_keys.load(stream);
                Assert::AreEqual(keys.size(), test_keys.size());
                Assert::IsTrue(keys.parms_id() == test_keys.parms_id());
                Assert::AreEqual(keys.decomposition_bit_count(), test_keys.decomposition_bit_count());
                Assert::AreEqual(0, keys.size());

                keys = keygen.galois_keys(1);
                Assert::AreEqual(keys.decomposition_bit_count(), 1);
                keys.save(stream);
                test_keys.load(stream);
                Assert::AreEqual(keys.size(), test_keys.size());
                Assert::IsTrue(keys.parms_id() == test_keys.parms_id());
                Assert::AreEqual(keys.decomposition_bit_count(), test_keys.decomposition_bit_count());
                for (int j = 0; j < test_keys.size(); j++)
                {
                    for (int i = 0; i < test_keys.data()[j].size(); i++)
                    {
                        Assert::AreEqual(keys.data()[j][i].size(), test_keys.data()[j][i].size());
                        Assert::AreEqual(keys.data()[j][i].uint64_count(), test_keys.data()[j][i].uint64_count());
                        Assert::IsTrue(is_equal_uint_uint(keys.data()[j][i].data(), test_keys.data()[j][i].data(), keys.data()[j][i].uint64_count()));
                    }
                }
                Assert::AreEqual(10, keys.size());

                keys = keygen.galois_keys(8);
                Assert::AreEqual(keys.decomposition_bit_count(), 8);
                keys.save(stream);
                test_keys.load(stream);
                Assert::AreEqual(keys.size(), test_keys.size());
                Assert::IsTrue(keys.parms_id() == test_keys.parms_id());
                Assert::AreEqual(keys.decomposition_bit_count(), test_keys.decomposition_bit_count());
                for (int j = 0; j < test_keys.size(); j++)
                {
                    for (int i = 0; i < test_keys.data()[j].size(); i++)
                    {
                        Assert::AreEqual(keys.data()[j][i].size(), test_keys.data()[j][i].size());
                        Assert::AreEqual(keys.data()[j][i].uint64_count(), test_keys.data()[j][i].uint64_count());
                        Assert::IsTrue(is_equal_uint_uint(keys.data()[j][i].data(), test_keys.data()[j][i].data(), keys.data()[j][i].uint64_count()));
                    }
                }
                Assert::AreEqual(10, keys.size());

                keys = keygen.galois_keys(60);
                Assert::AreEqual(keys.decomposition_bit_count(), 60);
                keys.save(stream);
                test_keys.load(stream);
                Assert::AreEqual(keys.size(), test_keys.size());
                Assert::IsTrue(keys.parms_id() == test_keys.parms_id());
                Assert::AreEqual(keys.decomposition_bit_count(), test_keys.decomposition_bit_count());
                for (int j = 0; j < test_keys.size(); j++)
                {
                    for (int i = 0; i < test_keys.data()[j].size(); i++)
                    {
                        Assert::AreEqual(keys.data()[j][i].size(), test_keys.data()[j][i].size());
                        Assert::AreEqual(keys.data()[j][i].uint64_count(), test_keys.data()[j][i].uint64_count());
                        Assert::IsTrue(is_equal_uint_uint(keys.data()[j][i].data(), test_keys.data()[j][i].data(), keys.data()[j][i].uint64_count()));
                    }
                }
                Assert::AreEqual(10, keys.size());
            }
            {
                EncryptionParameters parms(scheme_type::BFV);
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(256);
                parms.set_plain_modulus(65537);
                parms.set_coeff_modulus({ small_mods_60bit(0), small_mods_50bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                GaloisKeys keys;
                GaloisKeys test_keys;
                Assert::AreEqual(keys.decomposition_bit_count(), 0);
                keys.save(stream);
                test_keys.load(stream);
                Assert::AreEqual(keys.size(), test_keys.size());
                Assert::IsTrue(keys.parms_id() == test_keys.parms_id());
                Assert::AreEqual(keys.decomposition_bit_count(), test_keys.decomposition_bit_count());
                Assert::AreEqual(0, keys.size());

                keys = keygen.galois_keys(8);
                Assert::AreEqual(keys.decomposition_bit_count(), 8);
                keys.save(stream);
                test_keys.load(stream);
                Assert::AreEqual(keys.size(), test_keys.size());
                Assert::IsTrue(keys.parms_id() == test_keys.parms_id());
                Assert::AreEqual(keys.decomposition_bit_count(), test_keys.decomposition_bit_count());
                for (int j = 0; j < test_keys.size(); j++)
                {
                    for (int i = 0; i < test_keys.data()[j].size(); i++)
                    {
                        Assert::AreEqual(keys.data()[j][i].size(), test_keys.data()[j][i].size());
                        Assert::AreEqual(keys.data()[j][i].uint64_count(), test_keys.data()[j][i].uint64_count());
                        Assert::IsTrue(is_equal_uint_uint(keys.data()[j][i].data(), test_keys.data()[j][i].data(), keys.data()[j][i].uint64_count()));
                    }
                }
                Assert::AreEqual(14, keys.size());

                keys = keygen.galois_keys(60);
                Assert::AreEqual(keys.decomposition_bit_count(), 60);
                keys.save(stream);
                test_keys.load(stream);
                Assert::AreEqual(keys.size(), test_keys.size());
                Assert::IsTrue(keys.parms_id() == test_keys.parms_id());
                Assert::AreEqual(keys.decomposition_bit_count(), test_keys.decomposition_bit_count());
                for (int j = 0; j < test_keys.size(); j++)
                {
                    for (int i = 0; i < test_keys.data()[j].size(); i++)
                    {
                        Assert::AreEqual(keys.data()[j][i].size(), test_keys.data()[j][i].size());
                        Assert::AreEqual(keys.data()[j][i].uint64_count(), test_keys.data()[j][i].uint64_count());
                        Assert::IsTrue(is_equal_uint_uint(keys.data()[j][i].data(), test_keys.data()[j][i].data(), keys.data()[j][i].uint64_count()));
                    }
                }
                Assert::AreEqual(14, keys.size());
            }
        }
    };
}