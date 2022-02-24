#include "CppUnitTest.h"
#include "seal/publickey.h"
#include "seal/context.h"
#include "seal/defaultparams.h"
#include "seal/keygenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(PublicKeyTest)
    {
    public:
        TEST_METHOD(SaveLoadPublicKey)
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

                PublicKey pk = keygen.public_key();
                Assert::IsTrue(pk.parms_id() == parms.parms_id());
                pk.save(stream);

                PublicKey pk2;
                pk2.load(stream);

                Assert::AreEqual(pk.data().uint64_count(), pk2.data().uint64_count());
                for (int i = 0; i < pk.data().uint64_count(); i++)
                {
                    Assert::AreEqual(pk.data().data()[i], pk2.data().data()[i]);
                }
                Assert::IsTrue(pk.parms_id() == pk2.parms_id());
            }
            {
                EncryptionParameters parms(scheme_type::BFV);
                parms.set_noise_standard_deviation(3.19);
                parms.set_poly_modulus_degree(256);
                parms.set_plain_modulus(1 << 20);
                parms.set_coeff_modulus({ small_mods_30bit(0), small_mods_40bit(0) });
                auto context = SEALContext::Create(parms);
                KeyGenerator keygen(context);

                PublicKey pk = keygen.public_key();
                Assert::IsTrue(pk.parms_id() == parms.parms_id());
                pk.save(stream);

                PublicKey pk2;
                pk2.load(stream);

                Assert::AreEqual(pk.data().uint64_count(), pk2.data().uint64_count());
                for (int i = 0; i < pk.data().uint64_count(); i++)
                {
                    Assert::AreEqual(pk.data().data()[i], pk2.data().data()[i]);
                }
                Assert::IsTrue(pk.parms_id() == pk2.parms_id());
            }
        }
    };
}