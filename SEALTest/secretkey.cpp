#include "CppUnitTest.h"
#include "seal/secretkey.h"
#include "seal/context.h"
#include "seal/keygenerator.h"
#include "seal/defaultparams.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(SecretKeyTest)
    {
    public:
        TEST_METHOD(SaveLoadSecretKey)
        {
            stringstream stream;
            {
                EncryptionParameters parms(scheme_type::BFV);
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(64);
                parms.set_plain_modulus(1 << 6);
                parms.set_coeff_modulus({ small_mods_60bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                SecretKey sk = keygen.secret_key();
                Assert::IsTrue(sk.parms_id() == parms.parms_id());
                sk.save(stream);

                SecretKey sk2;
                sk2.load(stream);

                Assert::IsTrue(sk.data() == sk2.data());
                Assert::IsTrue(sk.parms_id() == sk2.parms_id());
            }
            {
                EncryptionParameters parms(scheme_type::BFV);
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(256);
                parms.set_plain_modulus(1 << 20);
                parms.set_coeff_modulus({ small_mods_30bit(0), small_mods_40bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                SecretKey sk = keygen.secret_key();
                Assert::IsTrue(sk.parms_id() == parms.parms_id());
                sk.save(stream);

                SecretKey sk2;
                sk2.load(stream);

                Assert::IsTrue(sk.data() == sk2.data());
                Assert::IsTrue(sk.parms_id() == sk2.parms_id());
            }
        }
    };
}