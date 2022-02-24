#include "CppUnitTest.h"
#include "seal/ciphertext.h"
#include "seal/context.h"
#include "seal/keygenerator.h"
#include "seal/encryptor.h"
#include "seal/memorymanager.h"
#include "seal/defaultparams.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace seal::util;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(CiphertextTest)
    {
    public:
        TEST_METHOD(CiphertextBasics)
        {
            EncryptionParameters parms(scheme_type::BFV);

            parms.set_poly_modulus_degree(2);
            parms.set_coeff_modulus({ small_mods_30bit(0) });
            parms.set_plain_modulus(2);
            parms.set_noise_standard_deviation(1.0);
            auto context = SEALContext::Create(parms);

            Ciphertext ctxt(context);
            ctxt.reserve(10);
            Assert::AreEqual(0ULL, ctxt.size());
            Assert::AreEqual(0ULL, ctxt.uint64_count());
            Assert::AreEqual(10ULL * 2 * 1, ctxt.uint64_count_capacity());
            Assert::AreEqual(2ULL, ctxt.poly_modulus_degree());
            Assert::IsTrue(ctxt.parms_id() == parms.parms_id());
            Assert::IsFalse(ctxt.is_ntt_form());
            const uint64_t *ptr = ctxt.data();

            ctxt.reserve(5);
            Assert::AreEqual(0ULL, ctxt.size());
            Assert::AreEqual(0ULL, ctxt.uint64_count());
            Assert::AreEqual(5ULL * 2 * 1, ctxt.uint64_count_capacity());
            Assert::AreEqual(2ULL, ctxt.poly_modulus_degree());
            Assert::IsTrue(ptr != ctxt.data());
            Assert::IsTrue(ctxt.parms_id() == parms.parms_id());
            ptr = ctxt.data();

            ctxt.reserve(10);
            Assert::AreEqual(0ULL, ctxt.size());
            Assert::AreEqual(0ULL, ctxt.uint64_count());
            Assert::AreEqual(10ULL * 2 * 1, ctxt.uint64_count_capacity());
            Assert::AreEqual(2ULL, ctxt.poly_modulus_degree());
            Assert::IsTrue(ptr != ctxt.data());
            Assert::IsTrue(ctxt.parms_id() == parms.parms_id());
            Assert::IsFalse(ctxt.is_ntt_form());
            ptr = ctxt.data();

            ctxt.reserve(2);
            Assert::AreEqual(0ULL, ctxt.size());
            Assert::AreEqual(2ULL * 2 * 1, ctxt.uint64_count_capacity());
            Assert::AreEqual(0ULL, ctxt.uint64_count());
            Assert::AreEqual(2ULL, ctxt.poly_modulus_degree());
            Assert::IsTrue(ptr != ctxt.data());
            Assert::IsTrue(ctxt.parms_id() == parms.parms_id());
            Assert::IsFalse(ctxt.is_ntt_form());
            ptr = ctxt.data();

            ctxt.reserve(5);
            Assert::AreEqual(0ULL, ctxt.size());
            Assert::AreEqual(5ULL * 2 * 1, ctxt.uint64_count_capacity());
            Assert::AreEqual(0ULL, ctxt.uint64_count());
            Assert::AreEqual(2ULL, ctxt.poly_modulus_degree());
            Assert::IsTrue(ptr != ctxt.data());
            Assert::IsTrue(ctxt.parms_id() == parms.parms_id());
            Assert::IsFalse(ctxt.is_ntt_form());

            Ciphertext ctxt2{ ctxt };
            Assert::AreEqual(ctxt.coeff_mod_count(), ctxt2.coeff_mod_count());
            Assert::AreEqual(ctxt.is_ntt_form(), ctxt2.is_ntt_form());
            Assert::AreEqual(ctxt.poly_modulus_degree(), ctxt2.poly_modulus_degree());
            Assert::IsTrue(ctxt.parms_id() == ctxt2.parms_id());
            Assert::AreEqual(ctxt.poly_modulus_degree(), ctxt2.poly_modulus_degree());
            Assert::AreEqual(ctxt.size(), ctxt2.size());

            Ciphertext ctxt3;
            ctxt3 = ctxt;
            Assert::AreEqual(ctxt.coeff_mod_count(), ctxt3.coeff_mod_count());
            Assert::AreEqual(ctxt.poly_modulus_degree(), ctxt3.poly_modulus_degree());
            Assert::AreEqual(ctxt.is_ntt_form(), ctxt3.is_ntt_form());
            Assert::IsTrue(ctxt.parms_id() == ctxt3.parms_id());
            Assert::AreEqual(ctxt.poly_modulus_degree(), ctxt3.poly_modulus_degree());
            Assert::AreEqual(ctxt.size(), ctxt3.size());
        }

        TEST_METHOD(SaveLoadCiphertext)
        {
            stringstream stream;
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_poly_modulus_degree(2);
            parms.set_coeff_modulus({ small_mods_30bit(0) });
            parms.set_plain_modulus(2);
            parms.set_noise_standard_deviation(1.0);

            auto context = SEALContext::Create(parms);

            Ciphertext ctxt(context); 
            Ciphertext ctxt2;
            ctxt.save(stream);
            ctxt2.load(stream);
            Assert::IsTrue(ctxt.parms_id() == ctxt2.parms_id());
            Assert::IsFalse(ctxt.is_ntt_form());
            Assert::IsFalse(ctxt2.is_ntt_form());

            parms.set_poly_modulus_degree(1024);
            parms.set_coeff_modulus(coeff_modulus_128(1024));
            parms.set_plain_modulus(0xF0F0);
            parms.set_noise_standard_deviation(3.14159);
            context = SEALContext::Create(parms);
            KeyGenerator keygen(context);
            Encryptor encryptor(context, keygen.public_key());
            encryptor.encrypt(Plaintext("Ax^10 + 9x^9 + 8x^8 + 7x^7 + 6x^6 + 5x^5 + 4x^4 + 3x^3 + 2x^2 + 1"), ctxt);
            ctxt.save(stream);
            ctxt2.load(stream);
            Assert::IsTrue(ctxt.parms_id() == ctxt2.parms_id());
            Assert::IsFalse(ctxt.is_ntt_form());
            Assert::IsFalse(ctxt2.is_ntt_form()); 
            Assert::IsTrue(is_equal_uint_uint(ctxt.data(), ctxt2.data(),
                parms.poly_modulus_degree() * static_cast<int>(parms.coeff_modulus().size()) * 2));
            Assert::IsTrue(ctxt.data() != ctxt2.data());
        }
    };
}