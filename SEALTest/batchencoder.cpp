#include "CppUnitTest.h"
#include "seal/batchencoder.h"
#include "seal/context.h"
#include "seal/defaultparams.h"
#include "seal/keygenerator.h"
#include <vector>
#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace seal::util;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(BatchEncoderTest)
    {
    public:
        TEST_METHOD(BatchUnbatchUIntVector)
        {
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_poly_modulus_degree(64);
            parms.set_coeff_modulus({ small_mods_60bit(0) });
            parms.set_plain_modulus(257);

            auto context = SEALContext::Create(parms);
            Assert::IsTrue(context->context_data()->qualifiers().using_batching);

            BatchEncoder batch_encoder(context);
            Assert::AreEqual(64ULL, batch_encoder.slot_count());
            vector<uint64_t> plain_vec;
            for (int i = 0; i < batch_encoder.slot_count(); i++)
            {
                plain_vec.push_back(i);
            }

            Plaintext plain;
            batch_encoder.encode(plain_vec, plain);
            vector<uint64_t> plain_vec2;
            batch_encoder.decode(plain, plain_vec2);
            Assert::IsTrue(plain_vec == plain_vec2);

            for (int i = 0; i < batch_encoder.slot_count(); i++)
            {
                plain_vec[i] = 5;
            }
            batch_encoder.encode(plain_vec, plain);
            Assert::IsTrue(plain.to_string() == "5");
            batch_encoder.decode(plain, plain_vec2);
            Assert::IsTrue(plain_vec == plain_vec2);

            vector<uint64_t> short_plain_vec;
            for (int i = 0; i < 20; i++)
            {
                short_plain_vec.push_back(i);
            }
            batch_encoder.encode(short_plain_vec, plain);
            vector<uint64_t> short_plain_vec2;
            batch_encoder.decode(plain, short_plain_vec2);
            Assert::AreEqual(20ULL, short_plain_vec.size());
            Assert::AreEqual(64ULL, short_plain_vec2.size());
            for (int i = 0; i < 20; i++)
            {
                Assert::AreEqual(short_plain_vec[i], short_plain_vec2[i]);
            }
            for (int i = 20; i < batch_encoder.slot_count(); i++)
            {
                Assert::AreEqual(0ULL, short_plain_vec2[i]);
            }
        }

        TEST_METHOD(BatchUnbatchIntVector)
        {
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_poly_modulus_degree(64);
            parms.set_coeff_modulus({ small_mods_60bit(0) });
            parms.set_plain_modulus(257);

            auto context = SEALContext::Create(parms);
            Assert::IsTrue(context->context_data()->qualifiers().using_batching);

            BatchEncoder batch_encoder(context);
            Assert::AreEqual(64ULL, batch_encoder.slot_count());
            vector<int64_t> plain_vec;
            for (int i = 0; i < batch_encoder.slot_count(); i++)
            {
                plain_vec.push_back(i * (1 - 2 * (i % 2)));
            }

            Plaintext plain;
            batch_encoder.encode(plain_vec, plain);
            vector<int64_t> plain_vec2;
            batch_encoder.decode(plain, plain_vec2);
            Assert::IsTrue(plain_vec == plain_vec2);

            for (int i = 0; i < batch_encoder.slot_count(); i++)
            {
                plain_vec[i] = -5;
            }
            batch_encoder.encode(plain_vec, plain);
            Assert::IsTrue(plain.to_string() == "FC");
            batch_encoder.decode(plain, plain_vec2);
            Assert::IsTrue(plain_vec == plain_vec2);

            vector<int64_t> short_plain_vec;
            for (int i = 0; i < 20; i++)
            {
                short_plain_vec.push_back(i * (1 - 2 * (i % 2)));
            }
            batch_encoder.encode(short_plain_vec, plain);
            vector<int64_t> short_plain_vec2;
            batch_encoder.decode(plain, short_plain_vec2);
            Assert::AreEqual(20ULL, short_plain_vec.size());
            Assert::AreEqual(64ULL, short_plain_vec2.size());
            for (int i = 0; i < 20; i++)
            {
                Assert::IsTrue(short_plain_vec[i] == short_plain_vec2[i]);
            }
            for (int i = 20; i < batch_encoder.slot_count(); i++)
            {
                Assert::IsTrue(0LL == short_plain_vec2[i]);
            }
        }

        TEST_METHOD(BatchUnbatchPlaintext)
        {
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_poly_modulus_degree(64);
            parms.set_coeff_modulus({ small_mods_60bit(0) });
            parms.set_plain_modulus(257);

            auto context = SEALContext::Create(parms);
            Assert::IsTrue(context->context_data()->qualifiers().using_batching);

            BatchEncoder batch_encoder(context);
            Assert::AreEqual(64ULL, batch_encoder.slot_count());
            Plaintext plain(batch_encoder.slot_count());
            for (int i = 0; i < batch_encoder.slot_count(); i++)
            {
                plain[i] = i;
            }

            batch_encoder.encode(plain);
            batch_encoder.decode(plain);
            for (int i = 0; i < batch_encoder.slot_count(); i++)
            {
                Assert::IsTrue(plain[i] == i);
            }

            for (int i = 0; i < batch_encoder.slot_count(); i++)
            {
                plain[i] = 5;
            }
            batch_encoder.encode(plain);
            Assert::IsTrue(plain.to_string() == "5");
            batch_encoder.decode(plain);
            for (int i = 0; i < batch_encoder.slot_count(); i++)
            {
                Assert::AreEqual(5ULL, plain[i]);
            }

            Plaintext short_plain(20);
            for (int i = 0; i < 20; i++)
            {
                short_plain[i] = i;
            }
            batch_encoder.encode(short_plain);
            batch_encoder.decode(short_plain);
            for (int i = 0; i < 20; i++)
            {
                Assert::IsTrue(short_plain[i] == i);
            }
            for (int i = 20; i < batch_encoder.slot_count(); i++)
            {
                Assert::IsTrue(short_plain[i] == 0);
            }
        }
    };
}