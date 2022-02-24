#include "CppUnitTest.h"
#include "seal/plaintext.h"
#include "seal/memorymanager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace seal::util;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(PlaintextTest)
    {
    public:
        TEST_METHOD(PlaintextBasics)
        {
            Plaintext plain(2);
            Assert::AreEqual(2ULL, plain.capacity());
            Assert::AreEqual(2ULL, plain.coeff_count());
            Assert::AreEqual(0ULL, plain.significant_coeff_count());
            Assert::IsFalse(plain.is_ntt_form());

            plain[0] = 1;
            plain[1] = 2;

            plain.reserve(10);
            Assert::AreEqual(10ULL, plain.capacity());
            Assert::AreEqual(2ULL, plain.coeff_count());
            Assert::AreEqual(2ULL, plain.significant_coeff_count());
            Assert::AreEqual(1ULL, plain[0]);
            Assert::AreEqual(2ULL, plain[1]);
            Assert::IsFalse(plain.is_ntt_form());

            plain.resize(5);
            Assert::AreEqual(10ULL, plain.capacity());
            Assert::AreEqual(5ULL, plain.coeff_count());
            Assert::AreEqual(2ULL, plain.significant_coeff_count());
            Assert::AreEqual(1ULL, plain[0]);
            Assert::AreEqual(2ULL, plain[1]);            
            Assert::AreEqual(0ULL, plain[2]);
            Assert::AreEqual(0ULL, plain[3]);
            Assert::AreEqual(0ULL, plain[4]);
            Assert::IsFalse(plain.is_ntt_form());

            Plaintext plain2;
            plain2.resize(15);
            Assert::AreEqual(15ULL, plain2.capacity());
            Assert::AreEqual(15ULL, plain2.coeff_count());
            Assert::AreEqual(0ULL, plain2.significant_coeff_count());
            Assert::IsFalse(plain.is_ntt_form());

            plain2 = plain;
            Assert::AreEqual(15ULL, plain2.capacity());
            Assert::AreEqual(5ULL, plain2.coeff_count());
            Assert::AreEqual(2ULL, plain2.significant_coeff_count());
            Assert::AreEqual(1ULL, plain2[0]);
            Assert::AreEqual(2ULL, plain2[1]);
            Assert::AreEqual(0ULL, plain2[2]);
            Assert::AreEqual(0ULL, plain2[3]);
            Assert::AreEqual(0ULL, plain2[4]);
            Assert::IsFalse(plain.is_ntt_form());

            plain.parms_id() = { 1ULL, 2ULL, 3ULL, 4ULL };
            Assert::IsTrue(plain.is_ntt_form());
            plain2 = plain;
            Assert::IsTrue(plain == plain2);
            plain2.parms_id() = parms_id_zero;
            Assert::IsFalse(plain2.is_ntt_form());
            Assert::IsFalse(plain == plain2);
            plain2.parms_id() = { 1ULL, 2ULL, 3ULL, 5ULL };
            Assert::IsFalse(plain == plain2);
        }

        TEST_METHOD(SaveLoadPlaintext)
        {
            stringstream stream;

            Plaintext plain;
            Plaintext plain2;
            plain.save(stream);
            plain2.load(stream);
            Assert::IsTrue(plain.data() == plain2.data());
            Assert::IsTrue(plain2.data() == nullptr);
            Assert::AreEqual(0ULL, plain2.capacity());
            Assert::AreEqual(0ULL, plain2.coeff_count());
            Assert::IsFalse(plain2.is_ntt_form());

            plain.reserve(20);
            plain.resize(5);
            plain[0] = 1;
            plain[1] = 2;
            plain[2] = 3;
            plain.save(stream);
            plain2.load(stream);
            Assert::IsTrue(plain.data() != plain2.data());
            Assert::AreEqual(5ULL, plain2.capacity());
            Assert::AreEqual(5ULL, plain2.coeff_count());
            Assert::AreEqual(1ULL, plain2[0]);
            Assert::AreEqual(2ULL, plain2[1]);
            Assert::AreEqual(3ULL, plain2[2]);
            Assert::AreEqual(0ULL, plain2[3]);
            Assert::AreEqual(0ULL, plain2[4]);
            Assert::IsFalse(plain2.is_ntt_form());

            plain.parms_id() = { 1, 2, 3, 4 };
            plain.save(stream);
            plain2.load(stream);
            Assert::IsTrue(plain2.is_ntt_form());
            Assert::IsTrue(plain2.parms_id() == plain.parms_id());
        }
    };
}